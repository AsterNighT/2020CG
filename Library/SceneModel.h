#pragma once

#include "Common.h"
#include "Model.h"
#include "../VAO.h"
#include "Material.h"
#include "../IBO.h"
#include "../Texture.h"
class SceneModel {
public:
	SceneModel(Model* model, Material* mat, bool isTess, bool ismWireframe);
	~SceneModel();
	//void addModel(Model* model, Material* mat);
	const GLuint getMeshCount() const;
	const Model* GetSceneModel() const;
	const std::vector<VAO*>& GetModelVertexArrayVector() const;
	const std::vector<IBO*>& GetModelIndexBufferVector() const;
	const std::vector<UINT>& GetModelIndexCountVector() const;
	const std::vector<Material*>& GetModelMaterialVector() const;
	const std::vector<Color>& GetModelAmbientColorVetor() const;
	const std::vector<Color>& GetModelDiffuseColorVetor() const;
	const std::vector<Color>& GetModelSpecularColorVetor() const;
	const std::vector<float>& GetModelSpecularPowerVetor() const;
	const std::vector<bool>& GetModelIsHaveBoneFlagVector() const;
	const std::vector<Texture*>& GetModelDiffuseTextureArrayVector() const;

private:
	SceneModel(const SceneModel& rhs);
	SceneModel& operator=(const SceneModel& rhs);

	void InitializeModelData(Model* model, Material* mat, bool isTess, bool ismWireframe);
	GLuint meshCount;
	Model* mModel;
	std::vector<VAO*> mModelVertexArrayVector;
	std::vector<IBO*> mModelIndexBufferVector;
	std::vector<UINT> mModelIndexCountVector;
	std::vector<Material*> mModelMaterialVector;

	std::vector<Color> mModelAmbientColorVetor;
	std::vector<Color> mModelDiffuseColorVetor;
	std::vector<Color> mModelSpecularColorVetor;
	std::vector<float> mModelSpecularPowerVetor;
};
