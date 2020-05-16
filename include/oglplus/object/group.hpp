/**
 *  @file oglplus/object/group.hpp
 *  @brief A group of object references
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_OBJECT_GROUP_1405011014_HPP
#define OGLPLUS_OBJECT_GROUP_1405011014_HPP

#include <oglplus/config/compiler.hpp>
#include <oglplus/object/seq_tpl.hpp>
#include <array>
#include <cassert>
#include <vector>

namespace oglplus {

template <typename ObjName>
class Group;

/// A dynamic group (convertible to a @c Sequence) of object names
template <typename ObjTag>
class Group<ObjectName<ObjTag>> {
private:
    using NameT = typename ObjTag::NameType;
    std::vector<NameT> _names;

public:
    /// Constructs an empty Group
    Group() {}

    Group(ObjectName<ObjTag> a, ObjectName<ObjTag> b) {
        _names.push_back(GetName(a));
        _names.push_back(GetName(b));
    }

    Group(const Group& that)
      : _names(that._names) {}

    Group(Group&& temp)
      : _names(std::move(temp._names)) {}

    /// Constructs the Group from an initializer list
    Group(std::initializer_list<ObjectName<ObjTag>> names) {
        _names.reserve(names.size());

        auto i = names.begin();
        while(i != names.end()) {
            _names.push_back(GetName(*i));
            ++i;
        }
    }

    /// Constructs the Group from a std range supporting begin and end
    template <typename StdRange>
    Group(const StdRange& range) {
        using std::begin;
        using std::distance;
        using std::end;

        auto b = begin(range);
        auto e = end(range);

        _names.reserve(distance(b, e));

        auto i = b;
        while(i != e) {
            _names.push_back(GetName(*i));
            ++i;
        }
    }

    /// Add a new name to this group
    Group& Add(ObjectName<ObjTag> name) {
        _names.push_back(GetName(name));
        return *this;
    }

    Sequence<ObjectName<ObjTag>> seq() const {
        return Sequence<ObjectName<ObjTag>>(_names.data(), _names.size());
    }

    /// Returns a sequence referencing the names in this group
    /** Note that the returned sequence must not be used after
     *  this group has been destroyed.
     */
    operator Sequence<ObjectName<ObjTag>>() const {
        return seq();
    }
};

template <typename ObjTag>
inline Group<ObjectName<ObjTag>> operator,(
  ObjectName<ObjTag> a, ObjectName<ObjTag> b) {
    return Group<ObjectName<ObjTag>>(a, b);
}

template <typename ObjTag>
inline Group<ObjectName<ObjTag>>&& operator,(
  Group<ObjectName<ObjTag>>&& g, ObjectName<ObjTag> n) {
    g.Add(n);
    return std::move(g);
}

template <typename ObjName, std::size_t N>
class StaticGroup;

template <typename ObjTag, std::size_t N>
class StaticGroup<ObjectName<ObjTag>, N> {
private:
    using NameT = typename ObjTag::NameType;

    std::array<NameT, N> _names;

    void _init(std::size_t) {}

    template <typename... Tags>
    void _init(
      std::size_t i, ObjectName<ObjTag> name, ObjectName<Tags>... names) {
        _names[i] = GetName(name);
        _init(i + 1, names...);
    }

public:
    StaticGroup(const ObjectName<ObjTag> (&names)[N]) {
        for(std::size_t i = 0; i != N; ++i) {
            _names[i] = GetName(names[i]);
        }
    }

    template <typename... Tag>
    StaticGroup(ObjectName<Tag>... names) {
        _init(0, names...);
    }

    Sequence<ObjectName<ObjTag>> seq() const {
        return Sequence<ObjectName<ObjTag>>(_names.data(), _names.size());
    }

    /// Returns a sequence referencing the names in this group
    /** Note that the returned sequence must not be used after
     *  this group has been destroyed.
     */
    operator Sequence<ObjectName<ObjTag>>() const {
        return seq();
    }
};

template <typename ObjTag, typename... ObjTags>
inline StaticGroup<ObjectName<ObjTag>, 1 + sizeof...(ObjTags)> MakeGroup(
  ObjectName<ObjTag> name, ObjectName<ObjTags>... names) {
    return StaticGroup<ObjectName<ObjTag>, 1 + sizeof...(ObjTags)>(
      name, names...);
}

} // namespace oglplus

#endif // include guard
