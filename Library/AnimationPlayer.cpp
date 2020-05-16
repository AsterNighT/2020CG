#include "AnimationPlayer.h"
#include "GameTime.h"
#include "Model.h"
#include "Bone.h"
#include "AnimationClip.h"
#include "BoneAnimation.h"
#include "Keyframe.h"
#include "MatrixHelper.h"
#include "glm/gtc/matrix_inverse.hpp"

namespace Library {
    RTTI_DEFINITIONS(AnimationPlayer)

    AnimationPlayer::AnimationPlayer(Game &game, Model &model, bool interpolationEnabled)
            : GameComponent(game),
              mModel(&model), mCurrentClip(nullptr), mCurrentTime(0.0f), mCurrentKeyframe(0U), mToRootTransforms(),
              mFinalTransforms(),
              mInverseRootTransform(MatrixHelper::Identity), mInterpolationEnabled(interpolationEnabled),
              mIsPlayingClip(false), mIsClipLooped(true) {
        mFinalTransforms.resize(model.Bones().size());
    }

    const Model &AnimationPlayer::GetModel() const {
        return *mModel;
    }

    const AnimationClip *AnimationPlayer::CurrentClip() const {
        return mCurrentClip;
    }

    float AnimationPlayer::CurrentTime() const {
        return mCurrentTime;
    }

    UINT AnimationPlayer::CurrentKeyframe() const {
        return mCurrentKeyframe;
    }

    const std::vector<mat4x4> &AnimationPlayer::BoneTransforms() const {
        return mFinalTransforms;
    }

    bool AnimationPlayer::InterpolationEnabled() const {
        return mInterpolationEnabled;
    }

    bool AnimationPlayer::IsPlayingClip() const {
        return mIsPlayingClip;
    }

    bool AnimationPlayer::IsClipLooped() const {
        return mIsClipLooped;
    }

    void AnimationPlayer::SetInterpolationEnabled(bool interpolationEnabled) {
        mInterpolationEnabled = interpolationEnabled;
    }

    void AnimationPlayer::StartClip(AnimationClip &clip) {
        mCurrentClip = &clip;
        mCurrentTime = 0.0f;
        mCurrentKeyframe = 0;
        mIsPlayingClip = true;

        mat4x4 inverseRootTransform = inverseTranspose(mModel->RootNode()->TransformMatrix());
        mInverseRootTransform = inverseRootTransform;
        GetBindPose(*(mModel->RootNode()));
    }

    void AnimationPlayer::PauseClip() {
        mIsPlayingClip = false;
    }

    void AnimationPlayer::ResumeClip() {
        if (mCurrentClip != nullptr) {
            mIsPlayingClip = true;
        }
    }

    void AnimationPlayer::Update(const GameTime &gameTime) {
        if (mIsPlayingClip) {
            assert(mCurrentClip != nullptr);

            mCurrentTime += static_cast<float>(gameTime.ElapsedGameTime()) * mCurrentClip->TicksPerSecond();
            if (mCurrentTime >= mCurrentClip->Duration()) {
                if (mIsClipLooped) {
                    mCurrentTime = 0.0f;
                } else {
                    mIsPlayingClip = false;
                    return;
                }
            }

            if (mInterpolationEnabled) {
                GetInterpolatedPose(mCurrentTime, *(mModel->RootNode()));
            } else {
                GetPose(mCurrentTime, *(mModel->RootNode()));
            }
        }
    }

    void AnimationPlayer::SetCurrentKeyFrame(UINT keyframe) {
        mCurrentKeyframe = keyframe;
        GetPoseAtKeyframe(mCurrentKeyframe, *(mModel->RootNode()));
    }

    void AnimationPlayer::GetBindPoseBottomUp(SceneNode &sceneNode) {
        mat4x4 toRootTransform = sceneNode.TransformMatrix();

        SceneNode *parentNode = sceneNode.Parent();
        while (parentNode != nullptr) {
            toRootTransform = toRootTransform * parentNode->TransformMatrix();
            parentNode = parentNode->Parent();
        }

        Bone *bone = sceneNode.As<Bone>();
        if (bone != nullptr) {
            mFinalTransforms[bone->Index()] = bone->OffsetTransformMatrix() * toRootTransform * mInverseRootTransform;
        }

        for (SceneNode *childNode : sceneNode.Children()) {
            GetBindPose(*childNode);
        }
    }

    void AnimationPlayer::GetBindPose(SceneNode &sceneNode) {
        mat4x4 toParentTransform = sceneNode.TransformMatrix();
        mat4x4 toRootTransform = (sceneNode.Parent() != nullptr ? toParentTransform *
                                                                  mToRootTransforms.at(sceneNode.Parent())
                                                                : toParentTransform);
        mToRootTransforms[&sceneNode] = toRootTransform;

        Bone *bone = sceneNode.As<Bone>();
        if (bone != nullptr) {
            mFinalTransforms[bone->Index()], bone->OffsetTransformMatrix() * toRootTransform * mInverseRootTransform;
        }

        for (SceneNode *childNode : sceneNode.Children()) {
            GetBindPose(*childNode);
        }
    }

    void AnimationPlayer::GetPose(float time, SceneNode &sceneNode) {
        mat4x4 toParentTransform;
        Bone *bone = sceneNode.As<Bone>();
        if (bone != nullptr) {
            mCurrentKeyframe = mCurrentClip->GetTransform(time, *bone, toParentTransform);
        } else {
            toParentTransform = sceneNode.Transform();
        }

        mat4x4 toRootTransform = (sceneNode.Parent() != nullptr ? toParentTransform *
                                                                  mToRootTransforms.at(sceneNode.Parent())
                                                                : toParentTransform);
        mToRootTransforms[&sceneNode] = toRootTransform;

        if (bone != nullptr) {
            mFinalTransforms[bone->Index()] = bone->OffsetTransformMatrix() * toRootTransform * mInverseRootTransform;
        }

        for (SceneNode *childNode : sceneNode.Children()) {
            GetPose(time, *childNode);
        }
    }

    void AnimationPlayer::GetPoseAtKeyframe(UINT keyframe, SceneNode &sceneNode) {
        mat4x4 toParentTransform;
        Bone *bone = sceneNode.As<Bone>();
        if (bone != nullptr) {
            mCurrentClip->GetTransformAtKeyframe(keyframe, *bone, toParentTransform);
        } else {
            toParentTransform = sceneNode.Transform();
        }

        mat4x4 toRootTransform = (sceneNode.Parent() != nullptr ? toParentTransform *
                                                                  mToRootTransforms.at(sceneNode.Parent())
                                                                : toParentTransform);
        mToRootTransforms[&sceneNode] = toRootTransform;

        if (bone != nullptr) {
            mFinalTransforms[bone->Index()] =
                    bone->OffsetTransformMatrix() * toRootTransform * mInverseRootTransform;
        }

        for (SceneNode *childNode : sceneNode.Children()) {
            GetPoseAtKeyframe(keyframe, *childNode);
        }
    }

    void AnimationPlayer::GetInterpolatedPose(float time, SceneNode &sceneNode) {
        mat4x4 toParentTransform;
        Bone *bone = sceneNode.As<Bone>();
        if (bone != nullptr) {
            mCurrentClip->GetInteropolatedTransform(time, *bone, toParentTransform);
        } else {
            toParentTransform = sceneNode.Transform();
        }

        mat4x4 toRootTransform = (sceneNode.Parent() != nullptr ? toParentTransform *
                                                                  mToRootTransforms.at(sceneNode.Parent())
                                                                : toParentTransform);
        mToRootTransforms[&sceneNode] = toRootTransform;

        if (bone != nullptr) {
            mFinalTransforms[bone->Index()] = bone->OffsetTransformMatrix() * toRootTransform * mInverseRootTransform;
        }

        for (SceneNode *childNode : sceneNode.Children()) {
            GetInterpolatedPose(time, *childNode);
        }
    }
}
