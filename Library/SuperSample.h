#pragma once

#include "Common.h"
#include "DrawableGameComponent.h"

namespace Library
{
	class Effect;
	class SuperSampleMaterial;
	class FullScreenRenderTarget;
	class FullScreenQuad;

	class SuperSample : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(SuperSample, DrawableGameComponent)

	public:
		SuperSample(Game& game, Camera& camera);
		~SuperSample();

		ID3D11ShaderResourceView* SceneTexture();
		void SetSceneTexture(ID3D11ShaderResourceView& sceneTexture);

		ID3D11ShaderResourceView* OutputTexture();

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		SuperSample();
		SuperSample(const SuperSample& rhs);
		SuperSample& operator=(const SuperSample& rhs);

		void UpdateGaussianMaterialNoBlur();

		static const float DefaultBlurAmount;

		Effect* mEffect;
		SuperSampleMaterial* mMaterial;
		ID3D11ShaderResourceView* mSceneTexture;
		ID3D11ShaderResourceView* mOutputTexture;
		FullScreenQuad* mFullScreenQuad;
	};
}