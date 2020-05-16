#include "ProxyModel.h"
#include "Game.h"
#include "GameException.h"
#include "BasicMaterial.h"
#include "Camera.h"
#include "MatrixHelper.h"
#include "VectorHelper.h"
#include "Model.h"
#include "Mesh.h"
#include "Utility.h"
#include "RasterizerStates.h"

namespace Library
{
	RTTI_DEFINITIONS(ProxyModel)

	ProxyModel::ProxyModel(Game& game, Camera& camera, const std::string& modelFileName, float scale)
		: DrawableGameComponent(game, camera),
		  mModelFileName(modelFileName), mEffect(nullptr), mMaterial(nullptr),
		  mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0),
		  mWorldMatrix(MatrixHelper::Identity), mScaleMatrix(MatrixHelper::Identity), mDisplayWireframe(true),
		  mPosition(Vector3Helper::Zero), mDirection(Vector3Helper::Forward), mUp(Vector3Helper::Up), mRight(Vector3Helper::Right)
	{
		XMStoreFloat4x4(&mScaleMatrix, mat4x4Scaling(scale, scale, scale));
	}

	ProxyModel::~ProxyModel()
	{
		DeleteObject(mMaterial);
		DeleteObject(mEffect);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}

	const vec3& ProxyModel::Position() const
    {
        return mPosition;
    }

    const vec3& ProxyModel::Direction() const
    {
        return mDirection;
    }
    
    const vec3& ProxyModel::Up() const
    {
        return mUp;
    }

    const vec3& ProxyModel::Right() const
    {
        return mRight;
    }

    vec4 ProxyModel::PositionVector() const
    {
        return mPosition;
    }

    vec4 ProxyModel::DirectionVector() const
    {
        return mDirection;
    }

    vec4 ProxyModel::UpVector() const
    {
        return mUp;
    }
    
    vec4 ProxyModel::RightVector() const
    {
        return mRight;
    }

	bool& ProxyModel::DisplayWireframe()
	{
		return mDisplayWireframe;
	}

	void ProxyModel::SetPosition(FLOAT x, FLOAT y, FLOAT z)
    {
        vec4 position = vec4Set(x, y, z, 1.0f);
        SetPosition(position);
    }

    void ProxyModel::SetPosition(Fvec4 position)
    {
        XMStoreFloat3(&mPosition, position);
    }

    void ProxyModel::SetPosition(const vec3& position)
    {
        mPosition = position;
    }

	void ProxyModel::ApplyRotation(Cmat4x4 transform)
    {
        vec4 direction = mDirection;
        vec4 up = mUp;
        
        direction = vec43TransformNormal(direction, transform);
        direction = vec43Normalize(direction);

        up = vec43TransformNormal(up, transform);
        up = vec43Normalize(up);

        vec4 right = vec43Cross(direction, up);
        up = vec43Cross(right, direction);

        XMStoreFloat3(&mDirection, direction);
        XMStoreFloat3(&mUp, up);
        XMStoreFloat3(&mRight, right);
    }

    void ProxyModel::ApplyRotation(const mat4x4& transform)
    {
        mat4x4 transformMatrix = transform;
        ApplyRotation(transformMatrix);
    }

	void ProxyModel::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		std::unique_ptr<Model> model(new Model(*mGame, mModelFileName, true));

		mEffect = new Effect(*mGame);
		mEffect->LoadCompiledEffect(L"Content\\Effects\\BasicEffect.cso");

		mMaterial = new BasicMaterial();
		mMaterial->Initialize(*mEffect);

		Mesh* mesh = model->Meshes().at(0);
		mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &mVertexBuffer);
		mesh->CreateIndexBuffer(&mIndexBuffer);
		mIndexCount = mesh->Indices().size();
	}

	void ProxyModel::Update(const GameTime& gameTime)
	{
		mat4x4 worldMatrix = mat4x4Identity();
		MatrixHelper::SetForward(worldMatrix, mDirection);
		MatrixHelper::SetUp(worldMatrix, mUp);
		MatrixHelper::SetRight(worldMatrix, mRight);
		MatrixHelper::SetTranslation(worldMatrix, mPosition);

		XMStoreFloat4x4(&mWorldMatrix, mScaleMatrix * worldMatrix);
	}

	void ProxyModel::Draw(const GameTime& gameTime)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();			
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pass* pass = mMaterial->CurrentTechnique()->Passes().at(0);		
		ID3D11InputLayout* inputLayout = mMaterial->InputLayouts().at(pass);
		direct3DDeviceContext->IASetInputLayout(inputLayout);

		UINT stride = mMaterial->VertexSize();
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		mat4x4 wvp = mWorldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
		mMaterial->WorldViewProjection() << wvp;
		
		pass->Apply(0, direct3DDeviceContext);

		if (mDisplayWireframe)
		{
			mGame->Direct3DDeviceContext()->RSSetState(RasterizerStates::Wireframe);
			direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
			mGame->Direct3DDeviceContext()->RSSetState(nullptr);
		}
		else
		{
			direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
		}
	}
}