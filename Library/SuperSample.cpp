#include "SuperSample.h"
#include "Game.h"
#include "GameException.h"
#include "SuperSampleMaterial.h"
#include "FullScreenRenderTarget.h"
#include "FullScreenQuad.h"
#include "Camera.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "Utility.h"
#include "ColorHelper.h"

namespace Library
{
	RTTI_DEFINITIONS(SuperSample)

		const float SuperSample::DefaultBlurAmount = 1.0f;

	SuperSample::SuperSample(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera),
		mEffect(nullptr), mMaterial(nullptr), mSceneTexture(nullptr), mOutputTexture(nullptr), mFullScreenQuad(nullptr)
	{
	}

	SuperSample::~SuperSample()
	{
		DeleteObject(mFullScreenQuad);
		DeleteObject(mMaterial);
		DeleteObject(mEffect);
	}

	ID3D11ShaderResourceView* SuperSample::SceneTexture()
	{
		return mSceneTexture;
	}

	void SuperSample::SetSceneTexture(ID3D11ShaderResourceView& sceneTexture)
	{
		mSceneTexture = &sceneTexture;
	}

	ID3D11ShaderResourceView* SuperSample::OutputTexture()
	{
		return mOutputTexture;
	}

	void SuperSample::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		mEffect = new Effect(*mGame);
		mEffect->LoadCompiledEffect(L"Content\\Effects\\SuperSample.cso");

		mMaterial = new SuperSampleMaterial();
		mMaterial->Initialize(*mEffect);

		mFullScreenQuad = new FullScreenQuad(*mGame, *mMaterial);
		mFullScreenQuad->Initialize();
	}

	void SuperSample::Draw(const GameTime& gameTime)
	{
		mOutputTexture = nullptr;

		mFullScreenQuad->SetActiveTechnique("SuperSample", "p0");
		mFullScreenQuad->SetCustomUpdateMaterial(std::bind(&SuperSample::UpdateGaussianMaterialNoBlur, this));
		mFullScreenQuad->Draw(gameTime);
	}

	void SuperSample::UpdateGaussianMaterialNoBlur()
	{
		mMaterial->ColorTexture() << mSceneTexture;
		//mMaterial->SampleWeights() << 1;
		//mMaterial->SampleOffsets() << 1;
	}
}