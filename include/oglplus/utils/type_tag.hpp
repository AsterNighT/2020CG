/**
 *  @file oglplus/utils/type_tag.hpp
 *  @brief TypeTag utility template
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2015-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_UTILS_TYPE_TAG_1107121519_HPP
#define OGLPLUS_UTILS_TYPE_TAG_1107121519_HPP

#include <oglplus/config/compiler.hpp>

namespace oglplus {

/// A tag template used mainly for data-type-based function overload dispatching
template <typename T>
struct TypeTag {
    using Type = T;

    T* operator&() const noexcept {
        return nullptr;
    }
};

} // namespace oglplus

#endif // include guard
