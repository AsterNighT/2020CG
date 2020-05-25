#pragma once
#include <string>
#include "ModelMaterial.h"
#include <Scene.h>
#include "Mesh.h"
#include "../Item.h"
class Item;
class Scene;
class Model {
	friend class Mesh;

public:
	Model(Scene& bScene, const std::string& filename, bool flipUVs = false, const std::string& tangentFileName = "");
	~Model();

	Scene& GetScene();
	bool HasMeshes() const;
	bool HasMaterials() const;
	bool HasAnimations() const;

	const std::vector<Mesh*>& Meshes() const;
	const std::vector<ModelMaterial*>& Materials() const;
	static std::vector<Item*> loadModel(const std::string& filename, bool flipUVs = false);

private:
	Model(const Model& rhs);
	Model& operator=(const Model& rhs);

	void ValidateModel();

	Scene& mScene;
	std::vector<Mesh*> mMeshes;
	std::vector<ModelMaterial*> mMaterials;
};

