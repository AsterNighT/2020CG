/**
 *  @file oglplus/native/context_glx.hpp
 *  @brief Wrapper for native current GLX/OpenGL context
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_NATIVE_CONTEXT_1404232057_HPP
#define OGLPLUS_NATIVE_CONTEXT_1404232057_HPP
// NOTE: the include guard is intentionally without the _GLX suffix so that
// different native context implementation cannot be included simultaneously.

#include <oglplus/native/surface_glx.hpp>

namespace oglplus {
namespace native {

/// Wrapper for a valid GLX context handle
class ContextGLX {
private:
    ::Display* _display;
    ::GLXContext _context;

    friend ::Display* GetGLXDisplay(const ContextGLX&) noexcept;

    friend ::GLXContext GetGLXContext(const ContextGLX&) noexcept;

protected:
    struct Current_ {};

    ContextGLX(Current_)
      : _display(::glXGetCurrentDisplay())
      , _context(::glXGetCurrentContext()) {
        if(!_display)
            HandleNoGLXDisplay();
        if(!_context)
            HandleNoGLXContext();
    }

public:
    friend bool operator==(const ContextGLX& a, const ContextGLX& b) noexcept {
        return (a._display == b._display) && (a._context == b._context);
    }

    friend bool operator!=(const ContextGLX& a, const ContextGLX& b) noexcept {
        return (a._display != b._display) || (a._context != b._context);
    }

    friend bool operator<(const ContextGLX& a, const ContextGLX& b) noexcept {
        return (a._display < b._display) ||
               ((a._display == b._display) && (a._context < b._context));
    }

    /// Returns a wrapper for the currently bound GLX context
    /** This function gets and wraps the current GLX context (+display).
     *  If no context is current it throws a @c runtime_error.
     *
     *  @throws std::runtime_error
     */
    static ContextGLX Current() {
        return ContextGLX(Current_());
    }

    /// Makes the context current on this thread
    void MakeCurrent(const SurfaceGLX& surface) {
        ::glXMakeCurrent(_display, surface._drawable, _context);
    }

    /// Releases the current context without binding a new one
    void Release() {
        ::glXMakeCurrent(_display, ::GLXDrawable(0), ::GLXContext(0));
    }
};

inline ::Display* GetGLXDisplay(const ContextGLX& cglx) noexcept {
    return cglx._display;
}

inline ::GLXContext GetGLXContext(const ContextGLX& cglx) noexcept {
    return cglx._context;
}

using Context = ContextGLX;

class CurrentContextGLX : public ContextGLX {
public:
    CurrentContextGLX()
      : ContextGLX(ContextGLX::Current_()) {}
};

using CurrentContext = CurrentContextGLX;

} // namespace native
} // namespace oglplus

#endif // include guard
