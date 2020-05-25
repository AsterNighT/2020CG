#pragma once

#include "Common.h"
#include "VBO.h"
#include "../IBO.h"
#include "../VAO.h"

struct aiMesh;

class Material;
class ModelMaterial;

class Mesh {
	friend class Model;

public:
	~Mesh();

	const std::string& Name() const;

	const std::vector<vec3>& Vertices() const;
	const std::vector<vec3>& Normals() const;
	const std::vector<vec3>& Tangents() const;
	const std::vector<vec3>& BiNormals() const;
	const std::vector<std::vector<vec3>*>& TextureCoordinates() const;
	const std::vector<std::vector<vec4>*>& VertexColors() const;
	UINT FaceCount() const;
	const std::vector<UINT>& Indices() const;

	VBO& VertexBuffer();
	IBO& IndexBuffer();

	bool HasCachedVertexBuffer() const;
	bool HasCachedIndexBuffer() const;

	void CreateIndexBuffer();
	void CreateVertexBuffer();

	void bind();

private:
	Mesh(aiMesh& mesh);

	std::string mName;
	std::vector<vec3> mVertices;
	std::vector<vec3> mNormals;
	std::vector<vec3> mTangents;
	std::vector<vec3> mBiNormals;
	std::vector<std::vector<vec3>*> mTextureCoordinates;
	std::vector<std::vector<vec4>*> mVertexColors;
	UINT mFaceCount;
	std::vector<UINT> mIndices;

	std::vector<vec3> mFileTangents;
	std::vector<vec3> mFileBiTangents;

	VBO mVertexBuffer;
	IBO mIndexBuffer;
	VAO mVertexArray;
	bool indexBufferCached;
	bool vertexBufferCached;
};