#include "BoneAnimation.h"
#include "GameException.h"
#include "Bone.h"
#include "Keyframe.h"
#include "Model.h"
#include "VectorHelper.h"
#include <assimp/scene.h>
#include "glm/ext/quaternion_float.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/gtx/quaternion.hpp"
namespace Library {
    BoneAnimation::BoneAnimation(Model &model, aiNodeAnim &nodeAnim)
            : mModel(&model), mBone(nullptr), mKeyframes() {
        UINT boneIndex = model.BoneIndexMapping().at(nodeAnim.mNodeName.C_Str());
        mBone = model.Bones().at(boneIndex);

        assert(nodeAnim.mNumPositionKeys == nodeAnim.mNumRotationKeys);
        assert(nodeAnim.mNumPositionKeys == nodeAnim.mNumScalingKeys);

        for (UINT i = 0; i < nodeAnim.mNumPositionKeys; i++) {
            aiVectorKey positionKey = nodeAnim.mPositionKeys[i];
            aiQuatKey rotationKey = nodeAnim.mRotationKeys[i];
            aiVectorKey scaleKey = nodeAnim.mScalingKeys[i];

            assert(positionKey.mTime == rotationKey.mTime);
            assert(positionKey.mTime == scaleKey.mTime);

            Keyframe *keyframe = new Keyframe(static_cast<float>(positionKey.mTime),
                                              vec3(positionKey.mValue.x, positionKey.mValue.y, positionKey.mValue.z),
                                              vec4(rotationKey.mValue.x, rotationKey.mValue.y, rotationKey.mValue.z,
                                                   rotationKey.mValue.w),
                                              vec3(scaleKey.mValue.x, scaleKey.mValue.y, scaleKey.mValue.z));
            mKeyframes.push_back(keyframe);
        }
    }

    BoneAnimation::~BoneAnimation() {
        for (Keyframe *keyframe : mKeyframes) {
            delete keyframe;
        }
    }

    Bone &BoneAnimation::GetBone() {
        return *mBone;
    }

    const std::vector<Keyframe *> BoneAnimation::Keyframes() const {
        return mKeyframes;
    }

    UINT BoneAnimation::GetTransform(float time, mat4x4 &transform) const {
        UINT keyframeIndex = FindKeyframeIndex(time);
        Keyframe *keyframe = mKeyframes[keyframeIndex];

        transform = keyframe->Transform();

        return keyframeIndex;
    }

    void BoneAnimation::GetTransformAtKeyframe(UINT keyframeIndex, mat4x4 &transform) const {
        // Clamp the keyframe
        if (keyframeIndex >= mKeyframes.size()) {
            keyframeIndex = mKeyframes.size() - 1;
        }

        Keyframe *keyframe = mKeyframes[keyframeIndex];

        transform = keyframe->Transform();
    }

    void BoneAnimation::GetInteropolatedTransform(float time, mat4x4 &transform) const {
        Keyframe *firstKeyframe = mKeyframes.front();
        Keyframe *lastKeyframe = mKeyframes.back();

        if (time <= firstKeyframe->Time()) {
            // Specified time is before the start time of the animation, so return the first keyframe
            transform = firstKeyframe->Transform();
        } else if (time >= lastKeyframe->Time()) {
            // Specified time is after the end time of the animation, so return the last keyframe
            transform = lastKeyframe->Transform();
        } else {
            // Interpolate the transform between keyframes
            UINT keyframeIndex = FindKeyframeIndex(time);
            Keyframe *keyframeOne = mKeyframes[keyframeIndex];
            Keyframe *keyframeTwo = mKeyframes[keyframeIndex + 1];

            vec4 translationOne = keyframeOne->TranslationVector();
            vec4 rotationQuaternionOne = keyframeOne->RotationQuaternionVector();
            vec4 scaleOne = keyframeOne->ScaleVector();

            vec4 translationTwo = keyframeTwo->TranslationVector();
            vec4 rotationQuaternionTwo = keyframeTwo->RotationQuaternionVector();
            vec4 scaleTwo = keyframeTwo->ScaleVector();

            float lerpValue = ((time - keyframeOne->Time()) / (keyframeTwo->Time() - keyframeOne->Time()));
            vec4 translation = translationOne*(1-lerpValue)+lerpValue*translationTwo;
            quat q1 = quat(rotationQuaternionOne);
            quat q2 = quat(rotationQuaternionTwo);
            quat rotationQuaternion = slerp(q1, q2, lerpValue);
            vec4 scale = scaleOne*(1-lerpValue)+lerpValue*scaleTwo;

            vec4 rotationOrigin = Vector4Helper::Zero;
            transform = translate(identity<mat4x4>(),vec3(translation.x,translation.y,translation.z));
            transform = transform*toMat4(rotationQuaternion);
            transform = glm::scale(transform,vec3(scale));
        }
    }

    UINT BoneAnimation::FindKeyframeIndex(float time) const {
        Keyframe *firstKeyframe = mKeyframes.front();
        if (time <= firstKeyframe->Time()) {
            return 0;
        }

        Keyframe *lastKeyframe = mKeyframes.back();
        if (time >= lastKeyframe->Time()) {
            return mKeyframes.size() - 1;
        }

        UINT keyframeIndex = 1;

        for (; keyframeIndex < mKeyframes.size() - 1 && time >= mKeyframes[keyframeIndex]->Time(); keyframeIndex++);

        return keyframeIndex - 1;
    }
}
