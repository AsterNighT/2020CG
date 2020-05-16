/**
 *  @file oglplus/error/basic.hpp
 *  @brief Declaration of basic OGLplus' exceptions
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_ERROR_BASIC_1107121317_HPP
#define OGLPLUS_ERROR_BASIC_1107121317_HPP

#include <oglplus/config/compiler.hpp>
#include <oglplus/config/error.hpp>
#include <oglplus/error/code.hpp>
#include <oglplus/size_type.hpp>
#include <oglplus/string/def.hpp>
#include <oglplus/string/empty.hpp>
#include <oglplus/string/ref.hpp>
#include <cassert>
#include <stdexcept>

namespace oglplus {

/** @defgroup error_handling Error handling
 *
 *  The exception classes listed below provide information about errors
 *  that occur during the excecution of the OpenGL function calls in the
 *  @OGLplus wrappers.
 */

/// Exception class for general OpenGL errors
/** Instances of this exception class are thrown whenever an error is detected
 *  during the execution of OpenGL API calls in the @OGLplus code. There are
 * several other classes derived for more specific error types, like GL shading
 * language compilation and linking errors, limit errors , etc. This class is
 * derived from the standard runtime_error exception and thus the basic error
 * message can be obtained by calling its @c what() member function.
 *
 *  @ingroup error_handling
 */
class Error : public std::runtime_error {
private:
    GLenum _code;
#if !OGLPLUS_ERROR_NO_FILE
    const char* _file;
#endif
#if !OGLPLUS_ERROR_NO_FUNC
    const char* _func;
#endif
#if !OGLPLUS_ERROR_NO_LINE
    unsigned _line;
#endif

#if !OGLPLUS_ERROR_NO_GL_LIB
    const char* _gllib_name;
#endif

#if !OGLPLUS_ERROR_NO_GL_FUNC
    const char* _glfunc_name;
#endif

#if !OGLPLUS_ERROR_NO_GL_SYMBOL
    const char* _enumpar_name;
    GLenum _enumpar;
    GLint _index;
#endif

public:
    static const char* Message(GLenum);

    Error(const char* message);

    Error(const Error&) = default;
    Error& operator=(const Error&) = default;

    ~Error() noexcept {
    }

    Error& NoInfo() {
        return *this;
    }

    Error& Code(GLenum code) {
        _code = code;
        return *this;
    }

    /// Returns the GL error code related to the error
    ErrorCode Code() const {
        return ErrorCode(_code);
    }

    Error& SourceFile(const char* file) {
#if !OGLPLUS_ERROR_NO_FILE
        _file = file;
#endif
        OGLPLUS_FAKE_USE(file);
        return *this;
    }

    /// Returns the name of the source file where the error occured
    /**
     *  The result of this function is also influenced by the
     *  #OGLPLUS_ERROR_NO_FILE preprocessor configuration option.
     *  If set to zero this function behaves as described above, otherwise it
     *  returns nullptr.
     */
    const char* SourceFile() const;

    Error& SourceFunc(const char* func) {
#if !OGLPLUS_ERROR_NO_FUNC
        _func = func;
#endif
        OGLPLUS_FAKE_USE(func);
        return *this;
    }

    /// Returns the name of the function where the error occured
    /**
     *  The result of this function is also influenced by the
     *  #OGLPLUS_ERROR_NO_FUNC preprocessor configuration option.
     *  If set to zero this function behaves as described above, otherwise it
     *  returns nullptr.
     */
    const char* SourceFunc() const;

    Error& SourceLine(unsigned line) {
#if !OGLPLUS_ERROR_NO_LINE
        _line = line;
#endif
        OGLPLUS_FAKE_USE(line);
        return *this;
    }

    /// Returns the line of the source file where the error occured
    /**
     *  The result of this function is also influenced by the
     *  #OGLPLUS_ERROR_NO_LINE preprocessor configuration option.
     *  If set to zero this function behaves as described above, otherwise it
     *  returns zero.
     */
    unsigned SourceLine() const;

    Error& GLLib(const char* lib_name) {
#if !OGLPLUS_ERROR_NO_GL_LIB
        _gllib_name = lib_name;
#endif
        OGLPLUS_FAKE_USE(lib_name);
        return *this;
    }

    const char* GLLib() const;

    Error& GLFunc(const char* func_name) {
#if !OGLPLUS_ERROR_NO_GL_FUNC
        _glfunc_name = func_name;
#endif
        OGLPLUS_FAKE_USE(func_name);
        return *this;
    }

    /// Returns the name of the GL function related to the error
    /** This function returns the name of the failed OpenGL function
     *  (without the @c gl prefix) which is related to the error.
     *
     *  The result of this function is also influenced by the
     *  #OGLPLUS_ERROR_NO_GL_FUNC preprocessor configuration option.
     *  If set to zero this function behaves as described above, otherwise it
     *  returns nullptr.
     */
    const char* GLFunc() const;

    template <typename Enum_>
    Error& EnumParam(Enum_ param) {
#if !OGLPLUS_ERROR_NO_GL_SYMBOL
        _enumpar = GLenum(param);
        _enumpar_name = EnumValueName(param).c_str();
#endif
        OGLPLUS_FAKE_USE(param);
        return *this;
    }

    Error& EnumParam(GLenum param, const char* param_name) {
#if !OGLPLUS_ERROR_NO_GL_SYMBOL
        _enumpar = param;
        _enumpar_name = param_name;
#endif
        OGLPLUS_FAKE_USE(param);
        OGLPLUS_FAKE_USE(param_name);
        return *this;
    }

    /// Returns the value of the enumeration parameter related to the error
    /** This function returns the value of the main enumeration
     *  parameter passed to the failed OpenGL function
     *
     *  The result of this function is also influenced by the
     *  #OGLPLUS_ERROR_NO_GL_SYMBOL preprocessor configuration option.
     *  If set to zero this function behaves as described above, otherwise it
     *  returns zero.
     */
    GLenum EnumParam() const;

    /// Returns the name of the enumeration parameter related to the error
    /** This function returns the name of the main enumeration
     *  parameter passed to the failed OpenGL function
     *
     *  The result of this function is also influenced by the
     *  #OGLPLUS_ERROR_NO_GL_SYMBOL preprocessor configuration option.
     *  If set to zero this function behaves as described above, otherwise it
     *  returns nullptr.
     */
    const char* EnumParamName() const;

    Error& Index(SizeType index) {
#if !OGLPLUS_ERROR_NO_GL_SYMBOL
        _index = GLint(index);
#endif
        OGLPLUS_FAKE_USE(index);
        return *this;
    }

    /// Returns the index parameter related to the error
    /** This function returns the value of the index
     *  parameter passed to the failed OpenGL function if applicable.
     *  If no index value is available then this function return a negative
     *  integer.
     *
     *  The result of this function is also influenced by the
     *  #OGLPLUS_ERROR_NO_GL_SYMBOL preprocessor configuration option.
     *  If set to zero this function behaves as described above, otherwise it
     *  returns a negative integer.
     */
    GLint Index() const;

    /// Returns the value parameter related to the error
    virtual GLfloat Value() const {
        return GLfloat(0);
    }

    /// Returns the limit value related to the error
    virtual GLfloat Limit() const {
        return GLfloat(0);
    }

    /// Returns the bind target
    virtual GLenum BindTarget() const {
        return GLenum(0);
    }

    /// Returns the bind target name
    virtual const char* TargetName() const {
        return nullptr;
    }

    /// Returns the object type
    /** If the error is related to a GL object, then an object
     *  type enumeration value is returned. Otherwise the result is zero.
     */
    virtual GLenum ObjectType() const {
        return GLenum(0);
    }

    /// Returns the object type name
    /** If the error is related to a GL object, then a C string
     *  storing object type name is returned. Otherwise the result
     *  is nullptr.
     */
    virtual const char* ObjectTypeName() const {
        return nullptr;
    }

    /// Returns the object instance GL name
    /** If the error is related to a GL object, then the numeric
     *  GL name of the object is returned. Otherwise the result
     *  is a negative integer.
     */
    virtual GLint ObjectName() const {
        return -1;
    }

    /// Returns the object instance description
    /** If the error is related to a GL object, then a std::string
     *  storing object description is returned. Otherwise the result
     *  is an empty std::string.
     */
    virtual const std::string& ObjectDesc() const {
        return EmptyStdString();
    }

    /// Returns the subject type
    /** If the error is related to a pair of GL objects, then
     *  an object type enumeration value is returned. Otherwise
     *  the result is zero.
     */
    virtual GLenum SubjectType() const {
        return GLenum(0);
    }

    /// Returns the subject class name
    /** If the error is related a pair of GL objects, then a C string
     *  storing secondary object type name is returned. Otherwise the result
     *  is nullptr.
     */
    virtual const char* SubjectTypeName() const {
        return nullptr;
    }

    /// Returns the subject GL name
    /** If the error is related to a pair of GL objects, then
     *  the numeric GL name of the secondary object is returned.
     *  Otherwise the result is a negative integer.
     */
    virtual GLint SubjectName() const {
        return -1;
    }

    /// Returns the subject textual description
    /** If the error is related to a pair of GL objects, then a std::string
     *  storing the secondary object description is returned. Otherwise
     *  the result is an empty std::string.
     */
    virtual const std::string& SubjectDesc() const {
        return EmptyStdString();
    }

    /// Returns the identifier of a GPU program variable
    /** If the error is related to a GPU program variable (vertex attrib,
     *  uniform, subroutine, etc.) then this function returns a C string
     *  storing the identifier of the variable. Otherwise the result
     *  is nullptr.
     */
    virtual const char* Identifier() const {
        return nullptr;
    }

    /// Returns a log string associated with the error
    /** If the error was caused by a process (like shader compilation,
     *  program linking or validation, etc.) which creates a textual
     *  log and it is available then it is returned by this function.
     *  Otherwise the result is an empty String.
     */
    virtual const String& Log() const {
        return EmptyString();
    }
};

/// Generic error handling function
template <typename ErrorType>
inline void HandleError(ErrorType& error) {
    throw error;
}

#define OGLPLUS_ERROR_CONTEXT(GLFUNC, CLASS) \
    static const char* _errinf_glfn() {      \
        return #GLFUNC;                      \
    }                                        \
    static const char* _errinf_cls() {       \
        return #CLASS;                       \
    }

#define OGLPLUS_ERROR_REUSE_CONTEXT(SOURCE) \
    using SOURCE::_errinf_glfn;             \
    using SOURCE::_errinf_cls;

// Macro for generic error handling
#define OGLPLUS_HANDLE_ERROR(ERROR_CODE, MESSAGE, ERROR, ERROR_INFO) \
    {                                                                \
        ERROR error(MESSAGE);                                        \
        OGLPLUS_FAKE_USE(error.ERROR_INFO.SourceFile(__FILE__)       \
                           .SourceFunc(__FUNCTION__)                 \
                           .SourceLine(__LINE__)                     \
                           .Code(error_code));                       \
        HandleError(error);                                          \
    }

#define OGLPLUS_RETURN_HANDLER(ERROR_CODE, MESSAGE, ERROR, ERROR_INFO)    \
    {                                                                     \
        return DeferredHandler([=]() -> void {                            \
            OGLPLUS_HANDLE_ERROR(ERROR_CODE, MESSAGE, ERROR, ERROR_INFO); \
        });                                                               \
    }

// Macro for optional generic error handling
#define OGLPLUS_HANDLE_ERROR_WITH_HANDLER_IF(                       \
  CONDITION, ERROR_CODE, MESSAGE, ERROR, ERROR_INFO, HANDLER_MACRO) \
    {                                                               \
        GLenum error_code = ERROR_CODE;                             \
        if(CONDITION)                                               \
            HANDLER_MACRO(error_code, MESSAGE, ERROR, ERROR_INFO)   \
    }

#define OGLPLUS_HANDLE_ERROR_IF(                     \
  CONDITION, ERROR_CODE, MESSAGE, ERROR, ERROR_INFO) \
    OGLPLUS_HANDLE_ERROR_WITH_HANDLER_IF(            \
      CONDITION, ERROR_CODE, MESSAGE, ERROR, ERROR_INFO, OGLPLUS_HANDLE_ERROR)

#define OGLPLUS_RETURN_HANDLER_IF(                   \
  CONDITION, ERROR_CODE, MESSAGE, ERROR, ERROR_INFO) \
    OGLPLUS_HANDLE_ERROR_WITH_HANDLER_IF(            \
      CONDITION,                                     \
      ERROR_CODE,                                    \
      MESSAGE,                                       \
      ERROR,                                         \
      ERROR_INFO,                                    \
      OGLPLUS_RETURN_HANDLER)

#define OGLPLUS_GLFUNC_CHECK_WITH_HANDLER(     \
  FUNC_NAME, ERROR, ERROR_INFO, HANDLER_MACRO) \
    OGLPLUS_HANDLE_ERROR_WITH_HANDLER_IF(      \
      error_code != GL_NO_ERROR,               \
      glGetError(),                            \
      ERROR::Message(error_code),              \
      ERROR,                                   \
      ERROR_INFO.GLFunc(FUNC_NAME),            \
      HANDLER_MACRO)

#define OGLPLUS_GLFUNC_CHECK(FUNC_NAME, ERROR, ERROR_INFO) \
    OGLPLUS_GLFUNC_CHECK_WITH_HANDLER(                     \
      FUNC_NAME, ERROR, ERROR_INFO, OGLPLUS_HANDLE_ERROR)

#define OGLPLUS_RETURN_GLFUNC_CHECK_HANDLER(FUNC_NAME, ERROR, ERROR_INFO) \
    OGLPLUS_GLFUNC_CHECK_WITH_HANDLER(                                    \
      FUNC_NAME, ERROR, ERROR_INFO, OGLPLUS_RETURN_HANDLER)

#define OGLPLUS_CHECK(GLFUNC, ERROR, ERROR_INFO) \
    OGLPLUS_GLFUNC_CHECK(#GLFUNC, ERROR, ERROR_INFO)

#define OGLPLUS_CHECK_CTXT(ERROR, ERROR_INFO) \
    OGLPLUS_GLFUNC_CHECK(_errinf_glfn(), ERROR, ERROR_INFO)

#define OGLPLUS_CHECK_SIMPLE(GLFUNC) OGLPLUS_CHECK(GLFUNC, Error, NoInfo())

#if !OGLPLUS_LOW_PROFILE
#define OGLPLUS_VERIFY(GLFUNC, ERROR, ERROR_INFO) \
    OGLPLUS_CHECK(GLFUNC, ERROR, ERROR_INFO)
#else
#define OGLPLUS_VERIFY(GLFUNC, ERROR, ERROR_INFO)
#endif

#define OGLPLUS_VERIFY_SIMPLE(GLFUNC) OGLPLUS_CHECK(GLFUNC, Error, NoInfo())

#define OGLPLUS_IGNORE(PARAM) ::glGetError();

#define OGLPLUS_DEFERRED_CHECK(GLFUNC, ERROR, ERROR_INFO) \
    OGLPLUS_GLFUNC_CHECK(#GLFUNC, ERROR, ERROR_INFO)

} // namespace oglplus

#if !OGLPLUS_LINK_LIBRARY || defined(OGLPLUS_IMPLEMENTING_LIBRARY)
#include <oglplus/error/basic.ipp>
#endif

#endif // include guard
