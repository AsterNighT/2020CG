/**
 *  @file oglplus/text/any.hpp
 *  @brief A type erasure for various implementation of text rendering
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_TEXT_ANY_HPP
#define OGLPLUS_TEXT_ANY_HPP

#include <oglplus/text/common.hpp>
#include <oglplus/text/unicode.hpp>

#include <cassert>
#include <memory>

namespace oglplus {
namespace text {

class AnyRenderer;
class AnyRendering;

class AnyFont {
private:
    struct _intf {
        virtual ~_intf() {}

        virtual _intf* _clone() const = 0;
        virtual const void* _addr() = 0;

        virtual Rectangle GlyphLogicalMetrics(CodePoint code_point) = 0;

        virtual GLfloat QueryXOffsets(
          const CodePoint* cps,
          GLsizei size,
          std::vector<GLfloat>& x_offsets) = 0;
    };

    template <class Font>
    class _impl : public _intf {
    private:
        Font _font;

    public:
        _impl(const Font& font)
          : _font(font) {}

        _intf* _clone() const override {
            return new _impl(_font);
        }

        const void* _addr() override {
            return static_cast<void*>(&_font);
        }

        Rectangle GlyphLogicalMetrics(CodePoint code_point) override {
            return _font.GlyphLogicalMetrics(code_point);
        }

        GLfloat QueryXOffsets(
          const CodePoint* cps,
          GLsizei size,
          std::vector<GLfloat>& x_offsets) override {
            return _font.QueryXOffsets(cps, size, x_offsets);
        }
    };

    _intf* _pimpl;

    _intf* _clone() const {
        assert(_pimpl);
        return _pimpl->_clone();
    }

    template <class Font>
    AnyFont(const Font& font)
      : _pimpl(new _impl<Font>(font)) {}

    friend class AnyRendering;

public:
    AnyFont(const AnyFont& that)
      : _pimpl(that._clone()) {}

    AnyFont(AnyFont&& tmp)
      : _pimpl(tmp._pimpl) {
        tmp._pimpl = nullptr;
    }

    ~AnyFont() {
        if(_pimpl)
            delete _pimpl;
    }

    AnyFont& operator=(const AnyFont& that) {
        if(this != &that) {
            _intf* tmp = that._clone();
            if(_pimpl)
                delete _pimpl;
            _pimpl = tmp;
        }
        return *this;
    }

    AnyFont& operator=(AnyFont&& tmp) {
        if(this != &tmp) {
            if(_pimpl)
                delete _pimpl;
            _pimpl = tmp._pimpl;
            tmp._pimpl = nullptr;
        }
        return *this;
    }

    template <class Font>
    const Font& As() const {
        assert(_pimpl);
        const void* ptr = _pimpl->_addr();
        assert(ptr);
        return *static_cast<const Font*>(ptr);
    }

    Rectangle GlyphLogicalMetrics(CodePoint code_point) {
        assert(_pimpl);
        return _pimpl->GlyphLogicalMetrics(code_point);
    }

    GLfloat QueryXOffsets(
      const CodePoint* cps, SizeType size, std::vector<GLfloat>& x_offsets) {
        assert(_pimpl);
        return _pimpl->QueryXOffsets(cps, size, x_offsets);
    }
};

class AnyLayout {
private:
    struct _intf {
        virtual ~_intf() {}

        virtual const void* _addr() = 0;

        virtual GLsizei Capacity() const = 0;
        virtual GLfloat Width() const = 0;

        virtual void Set(
          const CodePoint* code_points, const GLsizei length) = 0;
        virtual void Set(StrCRef string) = 0;
    };

    template <class Layout>
    class _impl : public _intf {
    private:
        Layout _layout;

    public:
        _impl(Layout&& layout)
          : _layout(std::move(layout)) {}

        const void* _addr() override {
            return static_cast<void*>(&_layout);
        }

        GLsizei Capacity() const override {
            return _layout.Capacity();
        }

        GLfloat Width() const override {
            return _layout.Width();
        }

        void Set(const CodePoint* code_points, const GLsizei length) override {
            _layout.Set(code_points, length);
        }

        void Set(StrCRef str) override {
            _layout.Set(str);
        }
    };

    _intf* _pimpl;

    AnyLayout(_intf* pimpl)
      : _pimpl(pimpl) {}

    template <class Layout>
    AnyLayout(Layout&& layout)
      : _pimpl(new _impl<Layout>(std::move(layout))) {}

    friend class AnyRendering;

public:
    AnyLayout(AnyLayout&& tmp)
      : _pimpl(tmp._pimpl) {
        tmp._pimpl = nullptr;
    }

    ~AnyLayout() {
        if(_pimpl)
            delete _pimpl;
    }

    AnyLayout& operator=(AnyLayout&& tmp) {
        if(this != &tmp) {
            if(_pimpl)
                delete _pimpl;
            _pimpl = tmp._pimpl;
            tmp._pimpl = nullptr;
        }
        return *this;
    }

    template <class Layout>
    const Layout& As() const {
        assert(_pimpl);
        const void* ptr = _pimpl->_addr();
        assert(ptr);
        return *static_cast<const Layout*>(ptr);
    }

    SizeType Capacity() const {
        assert(_pimpl);
        return MakeSizeType(_pimpl->Capacity(), std::nothrow);
    }

    GLfloat Width() const {
        assert(_pimpl);
        return _pimpl->Width();
    }

    void Set(const CodePoint* code_points, const GLsizei length) {
        assert(_pimpl);
        _pimpl->Set(code_points, length);
    }

    void Set(StrCRef str) {
        assert(_pimpl);
        _pimpl->Set(str);
    }
};

class AnyRenderer {
private:
    struct _intf {
        virtual ~_intf() {}

        virtual void SetProjection(const Mat4f& projection_matrix) = 0;
        virtual void SetCamera(const Mat4f& camera_matrix) = 0;
        virtual void SetLayoutTransform(const Mat4f& layout_matrix) = 0;
        virtual void SetAlignment(Alignment alignment) = 0;
        virtual void SetDirection(Direction direction) = 0;
        virtual void Use() = 0;
        virtual void Render(const AnyLayout& layout) = 0;
    };

    template <class Renderer, class Layout>
    class _impl : public _intf {
    private:
        Renderer _renderer;

    public:
        _impl(Renderer&& renderer)
          : _renderer(std::move(renderer)) {}

        void SetProjection(const Mat4f& projection_matrix) override {
            _renderer.SetProjection(projection_matrix);
        }

        void SetCamera(const Mat4f& camera_matrix) override {
            _renderer.SetCamera(camera_matrix);
        }

        void SetLayoutTransform(const Mat4f& layout_matrix) override {
            _renderer.SetLayoutTransform(layout_matrix);
        }

        void SetAlignment(Alignment alignment) override {
            _renderer.SetAlignment(alignment);
        }

        void SetDirection(Direction direction) override {
            _renderer.SetDirection(direction);
        }

        void Use() override {
            _renderer.Use();
        }

        void Render(const AnyLayout& layout) override {
            _renderer.Render(layout.As<Layout>());
        }
    };

    _intf* _pimpl;

    template <class Renderer, class Layout>
    AnyRenderer(Renderer&& renderer, Layout*)
      : _pimpl(new _impl<Renderer, Layout>(std::move(renderer))) {}

    friend class AnyRendering;

public:
    AnyRenderer(AnyRenderer&& tmp)
      : _pimpl(tmp._pimpl) {
        tmp._pimpl = nullptr;
    }

    ~AnyRenderer() {
        if(_pimpl)
            delete _pimpl;
    }

    AnyRenderer& operator=(AnyRenderer&& tmp) {
        if(this != &tmp) {
            if(_pimpl)
                delete _pimpl;
            _pimpl = tmp._pimpl;
            tmp._pimpl = nullptr;
        }
        return *this;
    }

    void SetProjection(const Mat4f& projection_matrix) {
        assert(_pimpl);
        _pimpl->SetProjection(projection_matrix);
    }

    void SetCamera(const Mat4f& camera_matrix) {
        assert(_pimpl);
        _pimpl->SetCamera(camera_matrix);
    }

    void SetLayoutTransform(const Mat4f& layout_matrix) {
        assert(_pimpl);
        _pimpl->SetLayoutTransform(layout_matrix);
    }

    void SetAlignment(Alignment alignment) {
        assert(_pimpl);
        _pimpl->SetAlignment(alignment);
    }

    void SetDirection(Direction direction) {
        assert(_pimpl);
        _pimpl->SetDirection(direction);
    }

    void Use() {
        assert(_pimpl);
        _pimpl->Use();
    }

    void Render(const AnyLayout& layout) {
        assert(_pimpl);
        _pimpl->Render(layout);
    }
};

class AnyRendering {
private:
    struct _intf {
        virtual ~_intf() {}

        virtual AnyFont LoadFont(const char* font_name) = 0;

        virtual AnyLayout MakeLayout(const AnyFont& font, GLsizei capacity) = 0;

        virtual AnyLayout MakeLayout(const AnyFont& font, StrCRef str) = 0;

        virtual AnyRenderer GetRenderer(const FragmentShader& shader) = 0;
    };

    template <class RenderingUtility>
    class _impl : public _intf {
    private:
        RenderingUtility _ru;
        using Renderer = typename RenderingUtility::Renderer;
        using Layout = typename RenderingUtility::Layout;
        using Font = typename RenderingUtility::Font;

    public:
        _impl(RenderingUtility&& ru)
          : _ru(std::move(ru)) {}

        AnyFont LoadFont(const char* font_name) override {
            return AnyFont(_ru.LoadFont(font_name));
        }

        AnyLayout MakeLayout(const AnyFont& font, GLsizei capacity) override {
            return AnyLayout(_ru.MakeLayout(font.As<Font>(), capacity));
        }

        AnyLayout MakeLayout(const AnyFont& font, StrCRef str) override {
            return AnyLayout(_ru.MakeLayout(font.As<Font>(), str));
        }

        AnyRenderer GetRenderer(const FragmentShader& shader) override {
            return AnyRenderer(_ru.GetRenderer(shader), (Layout*)nullptr);
        }
    };

    _intf* _pimpl;

public:
    template <class RenderingUtility>
    AnyRendering(RenderingUtility&& renderer)
      : _pimpl(new _impl<RenderingUtility>(std::move(renderer))) {}

    AnyRendering(AnyRendering&& tmp)
      : _pimpl(tmp._pimpl) {
        tmp._pimpl = nullptr;
    }

    ~AnyRendering() {
        if(_pimpl) {
            delete _pimpl;
        }
    }

    AnyRendering& operator=(AnyRendering&& tmp) {
        if(_pimpl)
            delete _pimpl;
        _pimpl = tmp._pimpl;
        tmp._pimpl = nullptr;
        return *this;
    }

    using Font = AnyFont;

    Font LoadFont(const char* font_name) {
        assert(_pimpl);
        return _pimpl->LoadFont(font_name);
    }

    using Layout = AnyLayout;

    Layout MakeLayout(const Font& font, GLsizei capacity) {
        assert(_pimpl);
        return _pimpl->MakeLayout(font, capacity);
    }

    Layout MakeLayout(const Font& font, StrCRef str) {
        assert(_pimpl);
        return _pimpl->MakeLayout(font, str);
    }

    using Renderer = AnyRenderer;

    Renderer GetRenderer(const FragmentShader& fragment_shader) {
        assert(_pimpl);
        return _pimpl->GetRenderer(fragment_shader);
    }
};

using AnyTextRendering = AnyRendering;

} // namespace text
} // namespace oglplus

#endif // include guard
