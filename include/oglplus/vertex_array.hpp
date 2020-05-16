/**
 *  @file oglplus/vertex_array.hpp
 *  @brief VertexArray wrappers
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_VERTEX_ARRAY_1107121519_HPP
#define OGLPLUS_VERTEX_ARRAY_1107121519_HPP

#include <oglplus/boolean.hpp>
#include <oglplus/error/object.hpp>
#include <oglplus/glfunc.hpp>
#include <oglplus/object/wrapper.hpp>
#include <cassert>

namespace oglplus {

/// Class wrapping vertex array construction/destruction functions
/** @note Do not use this class directly, use VertexArray instead.
 *
 *  @glsymbols
 *  @glfunref{GenVertexArrays}
 *  @glfunref{DeleteVertexArrays}
 *  @glfunref{IsVertexArray}
 */
template <>
class ObjGenDelOps<tag::VertexArray> {
protected:
    static void Gen(tag::Generate, GLsizei count, GLuint* names) {
        assert(names != nullptr);
        OGLPLUS_GLFUNC(GenVertexArrays)(count, names);
        OGLPLUS_CHECK_SIMPLE(GenVertexArrays);
    }
#if GL_VERSION_4_5 || GL_ARB_direct_state_access
    static void Gen(tag::Create, GLsizei count, GLuint* names) {
        assert(names != nullptr);
        OGLPLUS_GLFUNC(CreateVertexArrays)(count, names);
        OGLPLUS_CHECK_SIMPLE(CreateVertexArrays);
    }
#endif

    static void Delete(GLsizei count, GLuint* names) {
        assert(names != nullptr);
        OGLPLUS_GLFUNC(DeleteVertexArrays)(count, names);
        OGLPLUS_VERIFY_SIMPLE(DeleteVertexArrays);
    }

    static Boolean IsA(GLuint name) {
        Boolean result(OGLPLUS_GLFUNC(IsVertexArray)(name), std::nothrow);
        OGLPLUS_VERIFY_SIMPLE(IsVertexArray);
        return result;
    }
};

/// Vertex array binding operations
template <>
class ObjBindingOps<tag::VertexArray> {
protected:
    static GLuint _binding() {
        GLint name = 0;
        OGLPLUS_GLFUNC(GetIntegerv)(GL_VERTEX_ARRAY_BINDING, &name);
        OGLPLUS_VERIFY(
          GetIntegerv, ObjectError, EnumParam(GLenum(GL_VERTEX_ARRAY_BINDING)));
        assert(!(name < 0));
        return GLuint(name);
    }

public:
    /// Returns the currently bound VertexArray
    /**
     *  @glsymbols
     *  @glfunref{GetIntegerv}
     */
    static VertexArrayName Binding() {
        return VertexArrayName(_binding());
    }

    /// Binds the specified @p vertex_array object
    /**
     *  @glsymbols
     *  @glfunref{BindVertexArray}
     */
    static void Bind(VertexArrayName vertex_array) {
        OGLPLUS_GLFUNC(BindVertexArray)(GetGLName(vertex_array));
        OGLPLUS_VERIFY(BindVertexArray, ObjectError, Object(vertex_array));
    }
};

/// Common vertex array operations
/** @note Do not use this class directly, use VertexArray
 *  or NoVertexArray instead.
 */
template <>
class ObjCommonOps<tag::VertexArray>
  : public VertexArrayName
  , public ObjBindingOps<tag::VertexArray> {
protected:
    ObjCommonOps(VertexArrayName name) noexcept
      : VertexArrayName(name) {}

public:
    ObjCommonOps(ObjCommonOps&&) = default;
    ObjCommonOps(const ObjCommonOps&) = default;
    ObjCommonOps& operator=(ObjCommonOps&&) = default;
    ObjCommonOps& operator=(const ObjCommonOps&) = default;

    using ObjBindingOps<tag::VertexArray>::Bind;

    /// Binds this vertex array object
    /**
     *  @glsymbols
     *  @glfunref{BindVertexArray}
     */
    void Bind() const {
        Bind(*this);
    }
};

/// VertexArray operations with implicit selector
using VertexArrayOps = ObjectOps<tag::ImplicitSel, tag::VertexArray>;

/// An @ref oglplus_object encapsulating vertex array zero functionality
/**
 *  @ingroup oglplus_objects
 */
using NoVertexArray =
  ObjectZero<ObjZeroOps<tag::ImplicitSel, tag::VertexArray>>;

/// An @ref oglplus_object encapsulating vertex array object functionality
/**
 *  @ingroup oglplus_objects
 */
using VertexArray = Object<VertexArrayOps>;

} // namespace oglplus

#endif // include guard
