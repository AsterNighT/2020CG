#pragma once

#include "DrawableGameComponent.h"
#include "Frustum.h"

namespace Library
{
    class BasicMaterial;
	class Pass;
	
    class RenderableFrustum : public DrawableGameComponent
    {
        RTTI_DECLARATIONS(RenderableFrustum, DrawableGameComponent)

    public:
        RenderableFrustum(Game& game, Camera& camera);
		RenderableFrustum(Game& game, Camera& camera, const vec4& color);
        ~RenderableFrustum();

		const vec3& Position() const;
        const vec3& Direction() const;
        const vec3& Up() const;
        const vec3& Right() const;

		vec4 PositionVector() const;
        vec4 DirectionVector() const;
        vec4 UpVector() const;
        vec4 RightVector() const;

		void SetPosition(FLOAT x, FLOAT y, FLOAT z);
        void SetPosition(Fvec4 position);
        void SetPosition(const vec3& position);

		void ApplyRotation(Cmat4x4 transform);
        void ApplyRotation(const mat4x4& transform);

		void InitializeGeometry(const Frustum& frustum);

        virtual void Initialize() override;
        virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

    private:
        RenderableFrustum();
        RenderableFrustum(const RenderableFrustum& rhs);
        RenderableFrustum& operator=(const RenderableFrustum& rhs);

		void InitializeVertexBuffer(const Frustum& frustum);
		void InitializeIndexBuffer();

		static const vec4 DefaultColor;
		static const UINT FrustumVertexCount;		
		static const UINT FrustumPrimitiveCount;
		static const UINT FrustumIndicesPerPrimitive;
		static const UINT FrustumIndexCount;
		static const USHORT FrustumIndices[];

		ID3D11Buffer* mVertexBuffer;
        ID3D11Buffer* mIndexBuffer;
        BasicMaterial* mMaterial;
        Pass* mPass;
		ID3D11InputLayout* mInputLayout;
		
		vec4 mColor;
		vec3 mPosition;
		vec3 mDirection;
        vec3 mUp;
        vec3 mRight;

		mat4x4 mWorldMatrix;
    };
}