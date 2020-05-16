#include "RenderableFrustum.h"
#include "Game.h"
#include "GameException.h"
#include "BasicMaterial.h"
#include "VertexDeclarations.h"
#include "ColorHelper.h"
#include "MatrixHelper.h"
#include "VectorHelper.h"
#include "Camera.h"
#include "VertexDeclarations.h"

namespace Library
{
    RTTI_DEFINITIONS(RenderableFrustum)

	const vec4 RenderableFrustum::DefaultColor = ColorHelper::Green;

	const UINT RenderableFrustum::FrustumVertexCount = 8;
	const UINT RenderableFrustum::FrustumPrimitiveCount = 12;
	const UINT RenderableFrustum::FrustumIndicesPerPrimitive = 2;
	const UINT RenderableFrustum::FrustumIndexCount = FrustumPrimitiveCount * FrustumIndicesPerPrimitive;
	const USHORT RenderableFrustum::FrustumIndices[] = {
														// Near plane lines
														0, 1,
														1, 2,
														2, 3,
														3, 0,

														// Sides
														0, 4,
														1, 5,
														2, 6,
														3, 7,

														// Far plane lines
														4, 5,
														5, 6,
														6, 7,
														7, 4
													};

    RenderableFrustum::RenderableFrustum(Game& game, Camera& camera)
        : DrawableGameComponent(game, camera),
          mVertexBuffer(nullptr), mIndexBuffer(nullptr), mMaterial(nullptr), mPass(nullptr), mInputLayout(nullptr),          
		  mColor(DefaultColor), mPosition(Vector3Helper::Zero), mDirection(Vector3Helper::Forward), mUp(Vector3Helper::Up), mRight(Vector3Helper::Right),
		  mWorldMatrix(MatrixHelper::Identity)
    {
    }

	RenderableFrustum::RenderableFrustum(Game& game, Camera& camera, const vec4& color)
        : DrawableGameComponent(game, camera),
          mVertexBuffer(nullptr), mIndexBuffer(nullptr), mMaterial(nullptr), mPass(nullptr), mInputLayout(nullptr),          
		  mColor(color), mPosition(Vector3Helper::Zero), mDirection(Vector3Helper::Forward), mUp(Vector3Helper::Up), mRight(Vector3Helper::Right),
		  mWorldMatrix(MatrixHelper::Identity)
    {
    }

    RenderableFrustum::~RenderableFrustum()
    {		
        ReleaseObject(mIndexBuffer);
        ReleaseObject(mVertexBuffer);

		if (mMaterial != nullptr)
		{
			delete mMaterial->GetEffect();
			delete mMaterial;
			mMaterial = nullptr;
		}
    }

	const vec3& RenderableFrustum::Position() const
    {
        return mPosition;
    }

    const vec3& RenderableFrustum::Direction() const
    {
        return mDirection;
    }
    
    const vec3& RenderableFrustum::Up() const
    {
        return mUp;
    }

    const vec3& RenderableFrustum::Right() const
    {
        return mRight;
    }

    vec4 RenderableFrustum::PositionVector() const
    {
        return mPosition;
    }

    vec4 RenderableFrustum::DirectionVector() const
    {
        return mDirection;
    }

    vec4 RenderableFrustum::UpVector() const
    {
        return mUp;
    }
    
    vec4 RenderableFrustum::RightVector() const
    {
        return mRight;
    }

	void RenderableFrustum::SetPosition(FLOAT x, FLOAT y, FLOAT z)
    {
        vec4 position = vec4Set(x, y, z, 1.0f);
        SetPosition(position);
    }

    void RenderableFrustum::SetPosition(Fvec4 position)
    {
        XMStoreFloat3(&mPosition, position);
    }

    void RenderableFrustum::SetPosition(const vec3& position)
    {
        mPosition = position;
    }

	void RenderableFrustum::ApplyRotation(Cmat4x4 transform)
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

    void RenderableFrustum::ApplyRotation(const mat4x4& transform)
    {
        mat4x4 transformMatrix = transform;
        ApplyRotation(transformMatrix);
    }

	void RenderableFrustum::InitializeGeometry(const Frustum& frustum)
	{
		InitializeVertexBuffer(frustum);
	}

    void RenderableFrustum::Initialize()
	{
		Effect* effect = new Effect(*mGame);
		effect->LoadCompiledEffect(L"Content\\Effects\\BasicEffect.cso");
		
		mMaterial = new BasicMaterial();
		mMaterial->Initialize(*effect);

		mPass = mMaterial->CurrentTechnique()->Passes().at(0);
		mInputLayout = mMaterial->InputLayouts().at(mPass);

		InitializeIndexBuffer();
	}

	void RenderableFrustum::Update(const GameTime& gameTime)
	{
		mat4x4 worldMatrix = mat4x4Identity();
		MatrixHelper::SetForward(worldMatrix, mDirection);
		MatrixHelper::SetUp(worldMatrix, mUp);
		MatrixHelper::SetRight(worldMatrix, mRight);
		MatrixHelper::SetTranslation(worldMatrix, mPosition);

		XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
	}

	void RenderableFrustum::Draw(const GameTime& gameTime)
	{
		assert(mPass != nullptr);
        assert(mInputLayout != nullptr);

        ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();        
        direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        direct3DDeviceContext->IASetInputLayout(mInputLayout);

        UINT stride = sizeof(VertexPositionColor);
        UINT offset = 0;
        direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);		
        direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

		mat4x4 world = mWorldMatrix;
		mat4x4 wvp = world * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
		mMaterial->WorldViewProjection() << wvp;

        mPass->Apply(0, direct3DDeviceContext);

        direct3DDeviceContext->DrawIndexed(FrustumIndexCount, 0, 0);
	}

	void RenderableFrustum::InitializeVertexBuffer(const Frustum& frustum)
	{
		ReleaseObject(mVertexBuffer);

		VertexPositionColor vertices[FrustumVertexCount];
		const vec3* corners = frustum.Corners();
		for (UINT i = 0; i < FrustumVertexCount; i++)
		{
			vertices[i].Position = vec4(corners[i].x, corners[i].y, corners[i].z, 1.0f);
			vertices[i].Color = mColor;
		}

		D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.ByteWidth = sizeof(VertexPositionColor) * FrustumVertexCount;
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;	
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexSubResourceData;
        ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
        vertexSubResourceData.pSysMem = vertices;
        if (FAILED(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &mVertexBuffer)))
        {
            throw GameException("ID3D11Device::CreateBuffer() failed.");
        }
	}

	void RenderableFrustum::InitializeIndexBuffer()
	{
		D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
        indexBufferDesc.ByteWidth = sizeof(USHORT) * FrustumIndexCount;
        indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;		
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

        D3D11_SUBRESOURCE_DATA indexSubResourceData;
        ZeroMemory(&indexSubResourceData, sizeof(indexSubResourceData));
        indexSubResourceData.pSysMem = FrustumIndices;
        if (FAILED(mGame->Direct3DDevice()->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &mIndexBuffer)))
        {
            throw GameException("ID3D11Device::CreateBuffer() failed.");
        }
	}
}