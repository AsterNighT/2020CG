#include "SceneModel.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "Material.h"
#include "Mesh.h"

SceneModel::SceneModel(Model* model, Material* mat, bool isTess, bool ismWireframe) :
	mModel(NULL),
	mModelVertexArrayVector(NULL),
	mModelIndexBufferVector(NULL),
	mModelIndexCountVector(NULL),
	mModelMaterialVector(NULL),
	mModelAmbientColorVetor(NULL),
	mModelDiffuseColorVetor(NULL),
	mModelSpecularColorVetor(NULL),
	mModelSpecularPowerVetor(NULL) {
	InitializeModelData(model, mat, isTess, ismWireframe);
}

SceneModel::~SceneModel() {

}

const GLuint SceneModel::getMeshCount() const{
	return meshCount;
}

const Model* SceneModel::GetSceneModel() const {
	return mModel;
}
const std::vector<VAO*>& SceneModel::GetModelVertexArrayVector() const {
	return mModelVertexArrayVector;
}
const std::vector<IBO*>& SceneModel::GetModelIndexBufferVector() const {
	return mModelIndexBufferVector;
}
const std::vector<UINT>& SceneModel::GetModelIndexCountVector() const {
	return mModelIndexCountVector;
}
const std::vector<Material*>& SceneModel::GetModelMaterialVector() const {
	return mModelMaterialVector;
}
const std::vector<Color>& SceneModel::GetModelAmbientColorVetor() const {
	return mModelAmbientColorVetor;
}
const std::vector<Color>& SceneModel::GetModelDiffuseColorVetor() const {
	return mModelDiffuseColorVetor;
}
const std::vector<Color>& SceneModel::GetModelSpecularColorVetor() const {
	return mModelSpecularColorVetor;
}
const std::vector<float>& SceneModel::GetModelSpecularPowerVetor() const {
	return mModelSpecularPowerVetor;
}

void SceneModel::InitializeModelData(Model* model, Material* mat, bool isTess, bool ismWireframe) {

	mModel = model;

	UINT ModelMeshCount = model->Meshes().size();
	meshCount = ModelMeshCount;
	mModelVertexArrayVector.resize(ModelMeshCount);
	mModelIndexCountVector.resize(ModelMeshCount);
	mModelIndexBufferVector.resize(ModelMeshCount);
	for (UINT i = 0; i < ModelMeshCount; i++) {

		Mesh* mesh = model->Meshes().at(i);
		mModelVertexArrayVector[i] = new VAO();
		mModelVertexArrayVector[i]->bind();
		mat->CreateVertexBuffer(mModelVertexArrayVector[i], *mesh);
		mesh->CreateIndexBuffer();
		mesh->IndexBuffer().bind();
		mModelIndexBufferVector[i] = &(mesh->IndexBuffer());
		mModelIndexCountVector[i] = mesh->Indices().size();

		mModelMaterialVector.push_back(mat);

		mModelDiffuseColorVetor.push_back(model->Meshes().at(i)->GetMaterial()->GetDiffuseColor());
		mModelSpecularColorVetor.push_back(model->Meshes().at(i)->GetMaterial()->GetSpecularColor());
		mModelAmbientColorVetor.push_back(model->Meshes().at(i)->GetMaterial()->GetAmbientColor());
		mModelSpecularPowerVetor.push_back(model->Meshes().at(i)->GetMaterial()->GetNs());
	}
}