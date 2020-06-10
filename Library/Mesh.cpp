#include "Mesh.h"
#include "Model.h"
#include "Material.h"
#include <assimp/scene.h>
#include <fstream>
#include <sstream>
#include <algorithm>  

#include "VertexDeclarations.h"
#include "../GLMHelper.h"

void Mesh::bind() {
	mVertexArray.bind();
	if (!indexBufferCached && !mIndices.empty()) {
		CreateIndexBuffer();
		mIndexBuffer.bind();
		indexBufferCached = true;
	}
	if (!vertexBufferCached) {
		CreateVertexBuffer();
		mVertexBuffer.bind();
		vertexBufferCached = true;
	}
}

Mesh* Mesh::createPrism() {
	std::vector<vec3>* textureCoordinates = new std::vector<vec3>();
	auto* mesh = new Mesh();
	mesh->mTextureCoordinates.push_back(textureCoordinates);
	mesh->mVertices.emplace_back(vec3(0, 0, 0));
	textureCoordinates->emplace_back(vec3(0,0,0));
	mesh->mVertices.emplace_back(vec3(1, 0, 0));
	textureCoordinates->emplace_back(vec3(1, 0, 0));
	mesh->mVertices.emplace_back(vec3(1, 1, 0));
	textureCoordinates->emplace_back(vec3(1, 1, 0));
	for (int i = 0; i < 3; i++) mesh->mNormals.emplace_back((vec3(0, 0, -1)));
	mesh->mVertices.emplace_back(vec3(0, 0, 0));
	textureCoordinates->emplace_back(vec3(0, 0, 0));
	mesh->mVertices.emplace_back(vec3(1, 0, 0));
	textureCoordinates->emplace_back(vec3(1, 0, 0));
	mesh->mVertices.emplace_back(vec3(1, 0, 1));
	textureCoordinates->emplace_back(vec3(1, 1, 0));
	for (int i = 0; i < 3; i++) mesh->mNormals.emplace_back((vec3(0, -1, 0)));
	mesh->mVertices.emplace_back(vec3(1, 0, 0));
	textureCoordinates->emplace_back(vec3(0, 0, 0));
	mesh->mVertices.emplace_back(vec3(1, 1, 0));
	textureCoordinates->emplace_back(vec3(1, 0, 0));
	mesh->mVertices.emplace_back(vec3(1, 0, 1));
	textureCoordinates->emplace_back(vec3(0, 1, 0));
	for (int i = 0; i < 3; i++) mesh->mNormals.emplace_back((vec3(1, 0, 0)));
	mesh->mVertices.emplace_back(vec3(0, 0, 0));
	textureCoordinates->emplace_back(vec3(0, 0, 0));
	mesh->mVertices.emplace_back(vec3(1, 1, 0));
	textureCoordinates->emplace_back(vec3(0, 1, 0));
	mesh->mVertices.emplace_back(vec3(1, 0, 1));
	textureCoordinates->emplace_back(vec3(1, 1, 0));
	for (int i = 0; i < 3; i++) mesh->mNormals.emplace_back(normalize(vec3(-1, 1, 1)));
	for (int i = 0; i < 12; i++) mesh->mIndices.emplace_back(i);
	return mesh;
}

Mesh::Mesh(aiMesh& mesh)
	: mName(mesh.mName.C_Str()), mVertices(), mNormals(), mTangents(), mBiNormals(), mTextureCoordinates(), mVertexColors(),
	mFaceCount(0), mIndices(), mVertexBuffer(), mIndexBuffer() {
	indexBufferCached = false;
	vertexBufferCached = false;

	// Vertices

	mVertices.reserve(mesh.mNumVertices);
	for (UINT i = 0; i < mesh.mNumVertices; i++) {
		mVertices.push_back(GLMHelper::getVec3(reinterpret_cast<const float*>(&mesh.mVertices[i])));
	}


	// Normals
	if (mesh.HasNormals()) {
		mNormals.reserve(mesh.mNumVertices);
		for (UINT i = 0; i < mesh.mNumVertices; i++) {
			mNormals.push_back(GLMHelper::getVec3(reinterpret_cast<const float*>(&mesh.mNormals[i])));
		}
	}

	// Tangents and Binormals
	if (mesh.HasTangentsAndBitangents()) {
		mTangents.reserve(mesh.mNumVertices);
		mBiNormals.reserve(mesh.mNumVertices);
		for (UINT i = 0; i < mesh.mNumVertices; i++) {
			mTangents.push_back(GLMHelper::getVec3(reinterpret_cast<const float*>(&mesh.mTangents[i])));
			mBiNormals.push_back(GLMHelper::getVec3(reinterpret_cast<const float*>(&mesh.mBitangents[i])));
		}
	}

	// Texture Coordinates

	UINT uvChannelCount = mesh.GetNumUVChannels();
	for (UINT i = 0; i < uvChannelCount; i++) {
		std::vector<vec3>* textureCoordinates = new std::vector<vec3>();
		textureCoordinates->reserve(mesh.mNumVertices);
		mTextureCoordinates.push_back(textureCoordinates);

		aiVector3D* aiTextureCoordinates = mesh.mTextureCoords[i];
		for (UINT j = 0; j < mesh.mNumVertices; j++) {
			textureCoordinates->push_back(GLMHelper::getVec3(reinterpret_cast<const float*>(&aiTextureCoordinates[j])));
		}
	}


	// Vertex Colors
	UINT colorChannelCount = mesh.GetNumColorChannels();
	for (UINT i = 0; i < colorChannelCount; i++) {
		std::vector<vec4>* vertexColors = new std::vector<vec4>();
		vertexColors->reserve(mesh.mNumVertices);
		mVertexColors.push_back(vertexColors);

		aiColor4D* aiVertexColors = mesh.mColors[i];
		for (UINT j = 0; j < mesh.mNumVertices; j++) {
			vertexColors->push_back(GLMHelper::getVec4(reinterpret_cast<const float*>(&aiVertexColors[j])));
		}
	}

	// Faces (note: could pre-reserve if we limit primitive types)
	if (mesh.HasFaces()) {
		mFaceCount = mesh.mNumFaces;
		for (UINT i = 0; i < mFaceCount; i++) {
			aiFace* face = &mesh.mFaces[i];

			for (UINT j = 0; j < face->mNumIndices; j++) {
				mIndices.push_back(face->mIndices[j]);
			}
		}
	}
}

Mesh::Mesh() {
	indexBufferCached = false;
	vertexBufferCached = false;
}

Mesh::~Mesh() {
	for (std::vector<vec3>* textureCoordinates : mTextureCoordinates) {
		delete textureCoordinates;
	}

	for (std::vector<vec4>* vertexColors : mVertexColors) {
		delete vertexColors;
	}
}

#define calcCord(x,y) (x)*sizeU+(y)
Mesh::Mesh(tinynurbs::RationalSurface3f nurbs, float stepU, float stepV) :mVertices(), mNormals(), mTangents(), mBiNormals(), mTextureCoordinates(), mVertexColors(),
mFaceCount(0), mIndices(), mVertexBuffer(), mIndexBuffer() {
	indexBufferCached = false;
	vertexBufferCached = false;
	int sizeU = 1 / stepU + 1;
	int sizeV = 1 / stepV + 1;
	std::vector<vec3>* textureCoordinates = new std::vector<vec3>();
	for (int i = 0; i < sizeU; i++) {
		for (int j = 0; j < sizeV; j++) {
			mVertices.push_back(tinynurbs::surfacePoint(nurbs, i * stepU, j * stepV));
			vec3 p = tinynurbs::surfacePoint(nurbs, i * stepU, j * stepV);
			//std::cout << '(' << p.x << ' ' << p.y << ' ' << p.z << ')';
			mNormals.push_back(-tinynurbs::surfaceNormal(nurbs, i * stepU, j * stepV));
			textureCoordinates->push_back(vec3(i * stepU, j * stepV, 0));
		}
		//std::cout << std::endl;
	}
	mTextureCoordinates.push_back(textureCoordinates);
	for (int i = 0; i < sizeU - 1; i++) {
		for (int j = 0; j < sizeV - 1; j++) {
			mIndices.push_back(calcCord(i, j));
			mIndices.push_back(calcCord(i + 1, j));
			mIndices.push_back(calcCord(i + 1, j + 1));
			mIndices.push_back(calcCord(i + 1, j + 1));
			mIndices.push_back(calcCord(i, j + 1));
			mIndices.push_back(calcCord(i, j));
		}
	}

}

const std::string& Mesh::Name() const {
	return mName;
}

const std::vector<vec3>& Mesh::Vertices() const {
	return mVertices;
}

const std::vector<vec3>& Mesh::Normals() const {
	return mNormals;
}

const std::vector<vec3>& Mesh::Tangents() const {
	return mTangents;
}

const std::vector<vec3>& Mesh::BiNormals() const {
	return mBiNormals;
}

const std::vector<std::vector<vec3>*>& Mesh::TextureCoordinates() const {
	return mTextureCoordinates;
}

const std::vector<std::vector<vec4>*>& Mesh::VertexColors() const {
	return mVertexColors;
}

UINT Mesh::FaceCount() const {
	return mFaceCount;
}

const std::vector<UINT>& Mesh::Indices() const {
	return mIndices;
}

VBO& Mesh::VertexBuffer() {
	return mVertexBuffer;
}

IBO& Mesh::IndexBuffer() {
	return mIndexBuffer;
}

bool Mesh::HasCachedVertexBuffer() const {
	Mesh* mesh = const_cast<Mesh*>(this);

	return true;
}

bool Mesh::HasCachedIndexBuffer() const {
	Mesh* mesh = const_cast<Mesh*>(this);

	return true;
}

bool Mesh::HasTexCoord() {
	return hasTexCoord;
}

void Mesh::CreateIndexBuffer() {
	mIndexBuffer.setData(sizeof(UINT) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);
}

void Mesh::CreateVertexBuffer() {
	const std::vector<vec3>& sourceVertices = Vertices();
	std::vector<vec3>* textureCoordinates = nullptr;
	if (TextureCoordinates().size() > 0) {
		hasTexCoord = true;
		textureCoordinates = TextureCoordinates().at(0);
		assert(textureCoordinates->size() == sourceVertices.size());
	} else {
		hasTexCoord = false;
	}
	const std::vector<vec3>& normals = Normals();
	assert(normals.size() == sourceVertices.size());

	std::vector<VertexPositionTextureNormal> vertices;
	vertices.reserve(sourceVertices.size());
	for (UINT i = 0; i < sourceVertices.size(); i++) {
		vec3 position = sourceVertices.at(i);
		vec3 uv;
		if (textureCoordinates != nullptr) {
			uv = textureCoordinates->at(i);
		} else {
			uv = vec3(1, 1, 1);
		}
		vec3 normal = normals.at(i);

		vertices.push_back(VertexPositionTextureNormal(vec4(position.x, position.y, position.z, 1.0f),
			vec2(uv.x, uv.y), normal));
	}
	GLsizei totalSize = sizeof(VertexPositionTextureNormal) * sourceVertices.size();
	mVertexBuffer.setData(totalSize, &vertices[0]);
	std::vector<Attribute> attributes;
	attributes.push_back(Attribute(0, GL_FLOAT, 4, 4 * sizeof(GLfloat)));
	attributes.push_back(Attribute(1, GL_FLOAT, 2, 2 * sizeof(GLfloat)));
	attributes.push_back(Attribute(2, GL_FLOAT, 3, 3 * sizeof(GLfloat)));
	GLuint stride = 0;
	GLuint startPos = 0;
	for (auto& attrib : attributes) {
		stride += attrib.size;
	}
	for (auto& attrib : attributes) {
		glVertexAttribPointer(attrib.pos, attrib.count, attrib.type, attrib.normalized, stride, (GLvoid*)startPos);
		glEnableVertexAttribArray(attrib.pos);
		startPos += attrib.size;
	}
}
