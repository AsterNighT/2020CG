/**
 *  @file oglplus/sync.hpp
 *  @brief Sync object wrappers
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2019 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_SYNC_1107121519_HPP
#define OGLPLUS_SYNC_1107121519_HPP

#include <oglplus/enums/sync_condition.hpp>
#include <oglplus/enums/sync_status.hpp>
#include <oglplus/enums/sync_type.hpp>
#include <oglplus/enums/sync_wait_result.hpp>
#include <oglplus/error/basic.hpp>
#include <oglplus/glfunc.hpp>

namespace oglplus {

// NOTE: Xlib.h defines this symbol
// using the preprocessor. To avoid any sort of
// problems here it is necessary to observe correct order
// of header includes or you gotta keep 'em separated
// (in different translation units)
#ifdef Status
#undef Status
#endif

#if OGLPLUS_DOCUMENTATION_ONLY || GL_VERSION_3_2 || GL_ARB_sync

/// Encapsulates sync object and fence functionality
/**
 *
 *  @glvoereq{3,2,ARB,sync}
 */
class Sync {
private:
    GLsync _sync;

public:
    /// Types related to Sync
    struct Property {
        /// The synchronization condition
        using Condition = SyncCondition;

        /// Synchronization object type
        using Type = SyncType;

        /// Synchronization object status
        using Status = SyncStatus;

        /// Synchronization wait result
        using WaitResult = SyncWaitResult;
    };

    /// Creates a new sync object for the specified @p condition
    /**
     *
     * 	Sync objects are non-copyable
     *
     *  @glsymbols
     *  @glfunref{FenceSync}
     */
    Sync(SyncCondition condition = SyncCondition::GPUCommandsComplete)
      : _sync(OGLPLUS_GLFUNC(FenceSync)(GLenum(condition), 0)) {
        OGLPLUS_CHECK_SIMPLE(FenceSync);
    }

    Sync(const Sync&) = delete;

    /// Sync objects are moveable
    Sync(Sync&& temp)
      : _sync(temp._sync) {
        temp._sync = 0;
    }

    ~Sync() {
        if(_sync != 0)
            OGLPLUS_GLFUNC(DeleteSync)(_sync);
    }

    /// Returns true if this Sync object is in signaled status
    /**
     *  @glsymbols
     *  @glfunref{GetSync}
     *  @gldefref{SYNC_STATUS}
     *  @gldefref{SIGNALED}
     */
    bool Signaled() const {
        GLint result = 0;
        OGLPLUS_GLFUNC(GetSynciv)(_sync, GL_SYNC_STATUS, 1, nullptr, &result);
        return result == GL_SIGNALED;
    }

    /// Returns the type of this Sync object
    /**
     *  @glsymbols
     *  @glfunref{GetSync}
     *  @gldefref{OBJECT_TYPE}
     */
    SyncType Type() const {
        GLint result = 0;
        OGLPLUS_GLFUNC(GetSynciv)(_sync, GL_OBJECT_TYPE, 1, nullptr, &result);
        return SyncType(result);
    }

    /// Returns the condition of this Sync object
    /**
     *  @glsymbols
     *  @glfunref{GetSync}
     *  @gldefref{SYNC_CONDITION}
     */
    SyncCondition Condition() const {
        GLint result = 0;
        OGLPLUS_GLFUNC(GetSynciv)
        (_sync, GL_SYNC_CONDITION, 1, nullptr, &result);
        return SyncCondition(result);
    }

    /// Returns the status of this Sync object
    /**
     *  @glsymbols
     *  @glfunref{GetSync}
     *  @gldefref{SYNC_STATUS}
     */
    SyncStatus Status() const {
        GLint result = 0;
        OGLPLUS_GLFUNC(GetSynciv)(_sync, GL_SYNC_STATUS, 1, nullptr, &result);
        return SyncStatus(result);
    }

    /// Wait for the condition to be satisfied
    /**
     *  @glsymbols
     *  @glfunref{ClientWaitSync}
     */
    SyncWaitResult ClientWait(GLuint64 timeout) const {
        GLenum result = OGLPLUS_GLFUNC(ClientWaitSync)(_sync, 0, timeout);
        OGLPLUS_VERIFY_SIMPLE(ClientWaitSync);
        return SyncWaitResult(result);
    }

    /// Wait for the condition to be satisfied
    /**
     *  @glsymbols
     *  @glfunref{WaitSync}
     */
    void Wait(GLuint64 timeout = GL_TIMEOUT_IGNORED) const {
        OGLPLUS_GLFUNC(WaitSync)(_sync, 0, timeout);
        OGLPLUS_VERIFY_SIMPLE(WaitSync);
    }
};

#endif // sync

} // namespace oglplus

#endif // include guard
