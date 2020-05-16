#include "SceneNode.h"
#include "MatrixHelper.h"

namespace Library
{
	RTTI_DEFINITIONS(SceneNode)

	SceneNode::SceneNode(const std::string& name)
		: mName(name), mParent(nullptr), mChildren(), mTransform(MatrixHelper::Identity)
	{
	}

	SceneNode::SceneNode(const std::string& name, const mat4x4& transform)
		: mName(name), mParent(nullptr), mChildren(), mTransform(transform)
    {
    }

	const std::string& SceneNode::Name() const
	{
		return mName;
	}

	SceneNode* SceneNode::Parent()
	{
		return mParent;
	}

	std::vector<SceneNode*>& SceneNode::Children()
	{
		return mChildren;
	}

	const mat4x4& SceneNode::Transform() const
	{
		return mTransform;
	}

	mat4x4 SceneNode::TransformMatrix() const
	{
		return mTransform;
	}

	void SceneNode::SetParent(SceneNode* parent)
	{
		mParent = parent;
	}

	void SceneNode::SetTransform(mat4x4& transform)
	{
		mTransform = transform;
	}
}
