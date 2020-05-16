/**
 *  @file oglplus/error/program.hpp
 *  @brief Program errors
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_ERROR_PROGRAM_1107121519_HPP
#define OGLPLUS_ERROR_PROGRAM_1107121519_HPP

#include <oglplus/error/object.hpp>
#include <oglplus/string/def.hpp>

#include <cassert>

namespace oglplus {

/// Base class for program compilation or linking errors
/**
 *  @ingroup error_handling
 */
class ProgramBuildError : public ObjectError {
private:
    String _log;

public:
    ProgramBuildError(const char* message)
      : ObjectError(message) {
    }

    ProgramBuildError(const ProgramBuildError&) = default;
    ProgramBuildError(ProgramBuildError&&) = default;

    ~ProgramBuildError() noexcept {
    }

    ProgramBuildError& Log(String&& log) {
        _log = std::move(log);
        return *this;
    }

    /// Returns the compiler error output
    const String& Log() const {
        return _log;
    }
};

/// Exception class for OpenGL shading language compilation error
/**
 *  @ingroup error_handling
 */
class CompileError : public ProgramBuildError {
public:
    static const char* Message();

    CompileError(const char* message)
      : ProgramBuildError(message) {
    }
};

/// Exception class for OpenGL shading language program link error
/**
 *  @ingroup error_handling
 */
class LinkError : public ProgramBuildError {
public:
    static const char* Message();

    LinkError(const char* message)
      : ProgramBuildError(message) {
    }
};

/// Exception class for OpenGL shading language program validation error
/**
 *  @ingroup error_handling
 */
class ValidationError : public ProgramBuildError {
public:
    static const char* Message();

    ValidationError(const char* message)
      : ProgramBuildError(message) {
    }
};

} // namespace oglplus

#if !OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)
#include <oglplus/error/program.ipp>
#endif

#endif // include guard
