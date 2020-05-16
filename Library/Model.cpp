#include "Model.h"
#include "Scene.h"
#include "Mesh.h"
#include "ModelMaterial.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(Scene& bScene, const std::string& filename, bool flipUVs, const std::string& tangentFileName) : mScene(bScene), mMeshes(), mMaterials(){
	Assimp::Importer importer;

	UINT flags;

	flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_FlipWindingOrder | aiProcess_CalcTangentSpace;

	if (flipUVs) {
		flags |= aiProcess_FlipUVs;
	}

	const aiScene* scene = importer.ReadFile(filename, flags);

	if (scene->HasMaterials()) {
		for (UINT i = 0; i < scene->mNumMaterials; i++) {
			mMaterials.push_back(new ModelMaterial(*this, scene->mMaterials[i]));
		}
	}

	if (scene->HasMeshes()) {
		for (UINT i = 0; i < scene->mNumMeshes; i++) {
			Mesh* mesh;
			if (!tangentFileName.empty())
				mesh = new Mesh(*this, *(scene->mMeshes[i]), tangentFileName);
			else
				mesh = new Mesh(*this, *(scene->mMeshes[i]));
			mMeshes.push_back(mesh);
		}
	}
}

Model::~Model() {
	for (Mesh* mesh : mMeshes) {
		delete mesh;
	}
}

Scene& Model::GetScene() {
	return mScene;
}

bool Model::HasMeshes() const {
	return (mMeshes.size() > 0);
}

bool Model::HasMaterials() const {
	return (mMaterials.size() > 0);
}

const std::vector<Mesh*>& Model::Meshes() const {
	return mMeshes;
}

const std::vector<ModelMaterial*>& Model::Materials() const {
	return mMaterials;
}