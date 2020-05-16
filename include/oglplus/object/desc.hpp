/**
 *  @file oglplus/object/desc.hpp
 *  @brief Declaration of Object description
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_OBJECT_DESC_1107121519_HPP
#define OGLPLUS_OBJECT_DESC_1107121519_HPP

#include <oglplus/config/object.hpp>
#include <oglplus/object/name_tpl.hpp>
#include <oglplus/string/empty.hpp>

#if !OGLPLUS_NO_OBJECT_DESC
#include <cassert>
#include <map>
#endif

namespace oglplus {

class ObjectDesc {
private:
#if !OGLPLUS_NO_OBJECT_DESC
    std::string _str;
#endif
public:
    ObjectDesc() {}

    ObjectDesc(std::string&& str)
#if !OGLPLUS_NO_OBJECT_DESC
      : _str(std::forward<std::string>(str))
#endif
    {
        OGLPLUS_FAKE_USE(str);
    }

    ObjectDesc(ObjectDesc&& tmp)
#if !OGLPLUS_NO_OBJECT_DESC
      : _str(std::move(tmp._str))
#endif
    {
        OGLPLUS_FAKE_USE(tmp);
    }

    const std::string& Str() {
#if !OGLPLUS_NO_OBJECT_DESC
        return _str;
#else
        return EmptyStdString();
#endif
    }

#if !OGLPLUS_NO_OBJECT_DESC
    std::string&& Release() {
        return std::move(_str);
    }
#endif
};

namespace aux {

#if !OGLPLUS_NO_OBJECT_DESC
::std::map<unsigned, std::string>& ObjectDescRegistryStorage(int id);
#endif

#if !OGLPLUS_NO_OBJECT_DESC
class ObjectDescRegistryBase {
private:
    using _desc_map = ::std::map<unsigned, std::string>;

protected:
    static void _do_register_desc(
      _desc_map& storage, unsigned name, ObjectDesc&& desc);

    static void _do_unregister_desc(_desc_map& storage, unsigned name);

    static const std::string& _do_get_desc(_desc_map& storage, unsigned name);
};
#endif // !OGLPLUS_NO_OBJECT_DESC

class ObjectDescRegistry
#if !OGLPLUS_NO_OBJECT_DESC
  : public ObjectDescRegistryBase
#endif
{
private:
#if !OGLPLUS_NO_OBJECT_DESC
    using _base = ObjectDescRegistryBase;
    using _desc_map = ::std::map<unsigned, std::string>;

    static _desc_map& _storage(int id) {
        return ObjectDescRegistryStorage(id);
    }
#endif
public:
    // internal implementation detail. do not use directly
    static void _register_desc(int id, unsigned name, ObjectDesc&& desc)
#if OGLPLUS_NO_OBJECT_DESC
      noexcept {
        OGLPLUS_FAKE_USE(id);
        OGLPLUS_FAKE_USE(name);
        OGLPLUS_FAKE_USE(desc);
    }
#else
    {
        _base::_do_register_desc(_storage(id), name, std::move(desc));
    }
#endif

    // internal implementation detail. do not use directly
    static void _unregister_desc(int id, unsigned name)
#if OGLPLUS_NO_OBJECT_DESC
      noexcept {
        OGLPLUS_FAKE_USE(id);
        OGLPLUS_FAKE_USE(name);
    }
#else
    {
        _base::_do_unregister_desc(_storage(id), name);
    }
#endif

    // internal implementation detail. do not use directly
#if OGLPLUS_NO_OBJECT_DESC
    static const std::string& _get_desc(int, unsigned) noexcept {
        return EmptyStdString();
    }
#else
    static const std::string& _get_desc(int id, unsigned name) {
        return _base::_do_get_desc(_storage(id), name);
    }
#endif
};

} // namespace aux

template <typename ObjTag>
inline const std::string& DescriptionOf(ObjectName<ObjTag> object) {
    return aux::ObjectDescRegistry::_get_desc(ObjTag::value, GetName(object));
}

} // namespace oglplus

#if !OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)
#include <oglplus/object/desc.ipp>
#endif // OGLPLUS_LINK_LIBRARY

#endif // include guard
