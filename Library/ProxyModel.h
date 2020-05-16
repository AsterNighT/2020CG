#pragma once

#include "Common.h"
#include "DrawableGameComponent.h"

namespace Library
{
	class Effect;
	class BasicMaterial;

	class ProxyModel : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ProxyModel, DrawableGameComponent)

	public:
		ProxyModel(Game& game, Camera& camera, const std::string& modelFileName, float scale = 1.0f);
		~ProxyModel();

		const vec3& Position() const;
        const vec3& Direction() const;
        const vec3& Up() const;
        const vec3& Right() const;

		vec4 PositionVector() const;
        vec4 DirectionVector() const;
        vec4 UpVector() const;
        vec4 RightVector() const;

		bool& DisplayWireframe();

		void SetPosition(FLOAT x, FLOAT y, FLOAT z);
        void SetPosition(Fvec4 position);
        void SetPosition(const vec3& position);

		void ApplyRotation(Cmat4x4 transform);
        void ApplyRotation(const mat4x4& transform);

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;		
		virtual void Draw(const GameTime& gameTime) override;

	private:
		ProxyModel();
		ProxyModel(const ProxyModel& rhs);
		ProxyModel& operator=(const ProxyModel& rhs);

		std::string mModelFileName;
		Effect* mEffect;
		BasicMaterial* mMaterial;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;
        
		mat4x4 mWorldMatrix;
		mat4x4 mScaleMatrix;

		bool mDisplayWireframe;
		vec3 mPosition;
		vec3 mDirection;
        vec3 mUp;
        vec3 mRight;
	};
}