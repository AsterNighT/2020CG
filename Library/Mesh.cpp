#include "Mesh.h"
#include "Model.h"
#include "Material.h"
#include <assimp/scene.h>
#include <fstream>
#include <sstream>
#include <algorithm>  
#include "../GLMHelper.h"

Mesh::Mesh(Model& model, aiMesh& mesh, const std::string& tangentFileName)
	: mModel(model), mMaterial(nullptr), mName(mesh.mName.C_Str()), mVertices(), mNormals(), mTangents(), mBiNormals(), mTextureCoordinates(), mVertexColors(),
	mFaceCount(0), mIndices(), mVertexBuffer(), mIndexBuffer(), HasBoneFlag(false) {
	mMaterial = mModel.Materials().at(mesh.mMaterialIndex);

	struct Ver {
		vec3 pos;
		vec2 uv;
		vec3 n;
		vec3 t;
		vec3 b;
	};
	std::vector<Ver> data_0, data_1;
	std::vector<ivec3> triangles;
	int NumData;
	int faceNum;
	if (!tangentFileName.empty()) {
		std::ifstream fin(tangentFileName, std::ios::in || std::ios::_Nocreate);
		std::string s, str, s1, s2, s3, s4, t1, t2, t3;
		std::getline(fin, s);
		std::istringstream in(s);
		in >> s1 >> s2;
		in.clear();

		NumData = 0.5 * std::stoi(s1);
		faceNum = std::stoi(s2);

		data_0.resize(NumData);
		data_1.resize(NumData);

		for (int i = 0; i < NumData; i++) {
			//Position
			std::getline(fin, str);
			in.str(str);
			in >> s1 >> s2 >> s3;
			data_0.at(i).pos = vec3(std::stof(s1), std::stof(s2), std::stof(s3));
			in.clear();
			//UV
			std::getline(fin, str);
			in.str(str);
			in >> s1 >> s2 >> s3;
			assert(std::stoi(s3) == 0);
			data_0.at(i).uv = vec2(std::stof(s1), std::stof(s2));
			in.clear();
			//n
			std::getline(fin, str);
			in.str(str);
			in >> s1 >> s2 >> s3;
			data_0.at(i).n = vec3(std::stof(s1), std::stof(s2), std::stof(s3));
			in.clear();
			//t&b
			std::getline(fin, str);
			in.str(str);
			in >> s1 >> s2 >> s3 >> t1 >> t2 >> t3;
			data_0.at(i).t = vec3(std::stof(s1), std::stof(s2), std::stof(s3));
			data_0.at(i).b = vec3(std::stof(t1), std::stof(t2), std::stof(t3));
			in.clear();
		}
		for (int i = 0; i < NumData; i++) {
			//Position
			std::getline(fin, str);
			in.str(str);
			in >> s1 >> s2 >> s3;
			data_1.at(i).pos = vec3(std::stof(s1), std::stof(s2), std::stof(s3));
			in.clear();
			//UV
			std::getline(fin, str);
			in.str(str);
			in >> s1 >> s2 >> s3;
			assert(std::stoi(s3) == 1);
			data_1.at(i).uv = vec2(std::stof(s1), std::stof(s2));
			in.clear();
			//n
			std::getline(fin, str);
			in.str(str);
			in >> s1 >> s2 >> s3;
			data_1.at(i).n = vec3(std::stof(s1), std::stof(s2), std::stof(s3));
			in.clear();
			//t&b
			std::getline(fin, str);
			in.str(str);
			in >> s1 >> s2 >> s3 >> t1 >> t2 >> t3;
			data_1.at(i).t = vec3(std::stof(s1), std::stof(s2), std::stof(s3));
			data_1.at(i).b = vec3(std::stof(t1), std::stof(t2), std::stof(t3));
			in.clear();
		}

		for (int i = 0; i < faceNum; i++) {
			std::getline(fin, str);
			in.str(str);
			in.str(str);
			in >> s1 >> s2 >> s3 >> s4;

			std::vector<int> indices;
			if (std::stoi(s1) < NumData)
				indices.push_back(std::stoi(s1));
			if (std::stoi(s2) < NumData)
				indices.push_back(std::stoi(s2));
			if (std::stoi(s3) < NumData)
				indices.push_back(std::stoi(s3));
			if (std::stoi(s4) < NumData)
				indices.push_back(std::stoi(s4));

			if (indices.size() == 3) {
				vec3 n0 = data_1.at(indices.at(0)).n;
				vec3 n1 = data_1.at(indices.at(1)).n;
				vec3 n2 = data_1.at(indices.at(2)).n;
				vec3 n_avg = vec3((n0.x + n1.x + n2.x) / 3.0,
					(n0.y + n1.y + n2.y) / 3.0,
					(n0.z + n1.z + n2.z) / 3.0);
				vec3 p0 = data_1.at(indices.at(0)).pos;
				vec3 p1 = data_1.at(indices.at(1)).pos;
				vec3 p2 = data_1.at(indices.at(2)).pos;

				vec3 p1_0 = vec3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
				vec3 p2_0 = vec3(p2.x - p0.x, p2.y - p0.y, p2.z - p0.z);
				vec3 v = cross(p1_0, p2_0);

				float d = dot(n_avg, v);
				if (d < 0.0)
					std::reverse(indices.begin(), indices.end());
				triangles.push_back(ivec3(indices[0] + 1, indices[1] + 1, indices[2] + 1));
			}
			in.clear();
		}
	}

	// Vertices
	if (!tangentFileName.empty()) {
		mVertices.reserve(NumData);
		for (UINT i = 0; i < NumData; i++) {
			mVertices.push_back(vec3(data_1.at(i).pos));
		}
	} else {
		mVertices.reserve(mesh.mNumVertices);
		for (UINT i = 0; i < mesh.mNumVertices; i++) {
			mVertices.push_back(GLMHelper::getVec3(reinterpret_cast<const float*>(&mesh.mVertices[i])));
		}
	}

	// Normals
	if (!tangentFileName.empty()) {
		mNormals.reserve(NumData);
		for (UINT i = 0; i < NumData; i++) {
			mNormals.push_back(vec3(data_1.at(i).n));
		}
	} else if (mesh.HasNormals()) {
		mNormals.reserve(mesh.mNumVertices);
		for (UINT i = 0; i < mesh.mNumVertices; i++) {
			mNormals.push_back(GLMHelper::getVec3(reinterpret_cast<const float*>(&mesh.mNormals[i])));
		}
	}

	// Tangents and Binormals
	if (!tangentFileName.empty()) {
		mTangents.reserve(NumData);
		mBiNormals.reserve(NumData);
		for (UINT i = 0; i < NumData; i++) {
			mTangents.push_back(vec3(data_1.at(i).t));
			mBiNormals.push_back(vec3(data_1.at(i).b));
		}
	} else if (mesh.HasTangentsAndBitangents()) {
		mTangents.reserve(mesh.mNumVertices);
		mBiNormals.reserve(mesh.mNumVertices);
		for (UINT i = 0; i < mesh.mNumVertices; i++) {
			mTangents.push_back(GLMHelper::getVec3(reinterpret_cast<const float*>(&mesh.mTangents[i])));
			mBiNormals.push_back(GLMHelper::getVec3(reinterpret_cast<const float*>(&mesh.mBitangents[i])));
		}
	}

	// Texture Coordinates
	if (!tangentFileName.empty()) {
		UINT uvChannelCount = mesh.GetNumUVChannels();
		for (UINT i = 0; i < uvChannelCount; i++) {
			std::vector<vec3>* textureCoordinates = new std::vector<vec3>();
			textureCoordinates->reserve(NumData);
			mTextureCoordinates.push_back(textureCoordinates);

			for (UINT j = 0; j < NumData; j++) {
				textureCoordinates->push_back(vec3(data_1.at(i).uv.x, data_1.at(i).uv.y, 0));
			}
		}
	} else {
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
	if (!tangentFileName.empty()) {
		mFaceCount = triangles.size();
		for (UINT i = 0; i < mFaceCount; i++) {
			ivec3 face = triangles.at(i);
			mIndices.push_back(face.x);
			mIndices.push_back(face.y);
			mIndices.push_back(face.z);
		}
	} else if (mesh.HasFaces()) {
		mFaceCount = mesh.mNumFaces;
		for (UINT i = 0; i < mFaceCount; i++) {
			aiFace* face = &mesh.mFaces[i];

			for (UINT j = 0; j < face->mNumIndices; j++) {
				mIndices.push_back(face->mIndices[j]);
			}
		}
	}
}

Mesh::~Mesh() {
	for (std::vector<vec3>* textureCoordinates : mTextureCoordinates) {
		delete textureCoordinates;
	}

	for (std::vector<vec4>* vertexColors : mVertexColors) {
		delete vertexColors;
	}
}

Model& Mesh::GetModel() {
	return mModel;
}

ModelMaterial* Mesh::GetMaterial() {
	return mMaterial;
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

void Mesh::CreateIndexBuffer() {
	mIndexBuffer.setData(sizeof(UINT) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);
}