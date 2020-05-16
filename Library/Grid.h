#pragma once

#include "Common.h"
#include "DrawableGameComponent.h"

namespace Library
{
	class BasicMaterial;	
	class Pass;

	class Grid : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Grid, DrawableGameComponent)

	public:
		Grid(Game& game, Camera& camera);
		Grid(Game& game, Camera& camera, UINT size, UINT scale, vec4 color);
		~Grid();

		const vec3& Position() const;
		const vec4 Color() const;
		const UINT Size() const;
		const UINT Scale() const;

		void SetPosition(const vec3& position);
		void SetPosition(float x, float y, float z);
		void SetColor(const vec4& color);
		void SetSize(UINT size);
		void SetScale(UINT scale);

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		Grid();
		Grid(const Grid& rhs);
		Grid& operator=(const Grid& rhs);
		
		void InitializeGrid();

		static const UINT DefaultSize;
		static const UINT DefaultScale;
		static const vec4 DefaultColor;

		BasicMaterial* mMaterial;
		Pass* mPass;
		ID3D11InputLayout* mInputLayout;
		ID3D11Buffer* mVertexBuffer;
	
		vec3 mPosition;
		UINT mSize;
		UINT mScale;
		vec4 mColor;
		mat4x4 mWorldMatrix;
	};
}