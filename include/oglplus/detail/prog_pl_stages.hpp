/**
 *  .file oglplus/detail/prog_pl_stages.hpp
 *  .brief Helper class used by ProgramPipeline::UseStages
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_AUX_PROG_PL_STAGES_1107121519_HPP
#define OGLPLUS_AUX_PROG_PL_STAGES_1107121519_HPP

#include <oglplus/error/object.hpp>
#include <oglplus/glfunc.hpp>

namespace oglplus {

#if GL_VERSION_4_1 || GL_ARB_separate_shader_objects

class ProgPLUseStages {
private:
    GLuint _pipeline, _program;
    GLbitfield _bits;

    GLbitfield _forward() noexcept {
        GLbitfield res = _bits;
        _bits = 0;
        return res;
    }

    ProgPLUseStages();
    ProgPLUseStages(const ProgPLUseStages&);

    inline ProgPLUseStages _make(GLbitfield bit) noexcept {
        return ProgPLUseStages(_pipeline, _program, _forward() | bit);
    }

public:
    ProgPLUseStages(GLuint pipeline, GLuint program, GLbitfield bits) noexcept
      : _pipeline(pipeline)
      , _program(program)
      , _bits(bits) {
    }

    inline ProgPLUseStages Vertex() noexcept {
        return _make(GL_VERTEX_SHADER_BIT);
    }

    inline ProgPLUseStages TessControl() noexcept {
        return _make(GL_TESS_CONTROL_SHADER_BIT);
    }

    inline ProgPLUseStages TessEvaluation() noexcept {
        return _make(GL_TESS_EVALUATION_SHADER_BIT);
    }

    inline ProgPLUseStages Geometry() noexcept {
        return _make(GL_GEOMETRY_SHADER_BIT);
    }

    inline ProgPLUseStages Fragment() noexcept {
        return _make(GL_FRAGMENT_SHADER_BIT);
    }

    inline ProgPLUseStages All() noexcept {
        return _make(GL_ALL_SHADER_BITS);
    }

    inline ProgPLUseStages(ProgPLUseStages&& temp) noexcept
      : _pipeline(temp._pipeline)
      , _program(temp._program)
      , _bits(temp._forward()) {
    }

    void DoIt() {
        if(_bits) {
            assert(_pipeline);
            assert(_program);
            OGLPLUS_GLFUNC(UseProgramStages)(_pipeline, _bits, _program);
            OGLPLUS_VERIFY(
              UseProgramStages,
              ObjectError,
              Object(ProgramPipelineName(_pipeline)));
            _bits = 0;
        }
    }

    inline ~ProgPLUseStages() {
        try {
            DoIt();
        } catch(...) {
        }
    }
};

#endif // program pipeline

} // namespace oglplus

#endif // include guard
