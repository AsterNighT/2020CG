#pragma once

#include "Common.h"

namespace Library
{
	class SceneNode : public RTTI
	{
		RTTI_DECLARATIONS(SceneNode, RTTI)

	public:	
		const std::string& Name() const;
		SceneNode* Parent();
		std::vector<SceneNode*>& Children();
		const mat4x4& Transform() const;
		mat4x4 TransformMatrix() const;

		void SetParent(SceneNode* parent);

		void SetTransform(mat4x4& transform);
		void SetTransform(Cmat4x4 transform);

		SceneNode(const std::string& name);
		SceneNode(const std::string& name, const mat4x4& transform);

	protected:
		std::string mName;
		SceneNode* mParent;
		std::vector<SceneNode*> mChildren;
		mat4x4 mTransform;

	private:
		SceneNode();
		SceneNode(const SceneNode& rhs);
		SceneNode& operator=(const SceneNode& rhs);
	};
}
