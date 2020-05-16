/**
 *  .file oglplus/detail/boolean.hpp
 *  .brief Implementation of wrapper for booleans
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_DETAIL_BOOLEAN_1501311633_HPP
#define OGLPLUS_DETAIL_BOOLEAN_1501311633_HPP

#include <oglplus/config/compiler.hpp>
#include <new>
#include <stdexcept>

namespace oglplus {

template <typename B, typename T, T TrueVal, T FalseVal>
struct BoolImpl;

template <typename B, typename T, T TrueVal, T FalseVal>
struct WeakBoolImpl {
private:
    T _v;

    explicit WeakBoolImpl(T v) noexcept
      : _v(v) {
    }

    friend struct BoolImpl<B, T, TrueVal, FalseVal>;

public:
    operator bool() const noexcept {
        return _v != FalseVal;
    }

    bool operator!() const noexcept {
        return _v != TrueVal;
    }
};

template <typename B, typename T, T TrueVal, T FalseVal>
struct BoolImpl {
private:
    static_assert(TrueVal != FalseVal, "");
    static_assert(TrueVal >= 0, "");
    static_assert(FalseVal >= 0, "");

    T _v;

public:
    // implementation detail
    inline B _get() const noexcept {
        return B(_v);
    }

    // implementation detail
    inline T* _ptr() noexcept {
        return &_v;
    }

    BoolImpl() noexcept
      : _v(T(FalseVal)) {
    }

    BoolImpl(bool bv) noexcept
      : _v(bv ? TrueVal : FalseVal) {
    }

    BoolImpl(T v, std::nothrow_t) noexcept
      : _v(v) {
    }

    explicit BoolImpl(T v)
      : _v(v) {
        if((_v != TrueVal) && (_v != FalseVal)) {
            throw std::domain_error("Invalid value for Boolean");
        }
    }

    static BoolImpl Indeterminate() noexcept {
        return BoolImpl(TrueVal + FalseVal + 1);
    }

    operator bool() const noexcept {
        return _v == TrueVal;
    }

    bool operator!() const noexcept {
        return _v == FalseVal;
    }

    WeakBoolImpl<B, T, TrueVal, FalseVal> operator~() const noexcept {
        return WeakBoolImpl<B, T, TrueVal, FalseVal>(_v);
    }
};

} // namespace oglplus

#endif // include guard
