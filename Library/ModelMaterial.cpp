#include "ModelMaterial.h"
#include "Utility.h"
#include <assimp/scene.h>

std::map<TextureType, UINT> ModelMaterial::sTextureTypeMappings;

ModelMaterial::ModelMaterial(Model& model)
	: mModel(model), mTextures() {
	InitializeTextureTypeMappings();
}

ModelMaterial::ModelMaterial(Model& model, aiMaterial* material)
	: mModel(model), mTextures() {
	InitializeTextureTypeMappings();

	aiString name;
	material->Get(AI_MATKEY_NAME, name);
	mName = name.C_Str();

	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	float shininess;
	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
		mDiffuseColor = Color(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
	}
	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular)) {
		mSpecularColor = Color(specular.r, specular.g, specular.b, specular.a);
	}
	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
		mAmbientColor = Color(ambient.r, ambient.g, ambient.b, ambient.a);
	}
	unsigned int max = 65536;
	if (AI_SUCCESS == aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max)) {
		mShininess = shininess * 0.25f;
	}

	for (TextureType textureType = (TextureType)0; textureType < TextureTypeEnd; textureType = (TextureType)(textureType + 1)) {
		aiTextureType mappedTextureType = (aiTextureType)sTextureTypeMappings[textureType];

		UINT textureCount = material->GetTextureCount(mappedTextureType);
		if (textureCount > 0) {
			std::vector<std::wstring>* textures = new std::vector<std::wstring>();
			mTextures.insert(std::pair<TextureType, std::vector<std::wstring>*>(textureType, textures));

			textures->reserve(textureCount);
			for (UINT textureIndex = 0; textureIndex < textureCount; textureIndex++) {
				aiString path;
				if (material->GetTexture(mappedTextureType, textureIndex, &path) == AI_SUCCESS) {
					std::wstring wPath;
					Utility::ToWideString(path.C_Str(), wPath);

					textures->push_back(wPath);
				}
			}
		}
	}
}

ModelMaterial::~ModelMaterial() {
	for (std::pair<TextureType, std::vector<std::wstring>*> textures : mTextures) {
		DeleteObject(textures.second);
	}
}

Model& ModelMaterial::GetModel() {
	return mModel;
}

Color ModelMaterial::GetDiffuseColor() {
	return mDiffuseColor;
}

Color ModelMaterial::GetSpecularColor() {
	return mSpecularColor;
}

Color ModelMaterial::GetAmbientColor() {
	return mAmbientColor;
}

float ModelMaterial::GetNs() {
	return mShininess;
}

const std::string& ModelMaterial::Name() const {
	return mName;
}

const std::map<TextureType, std::vector<std::wstring>*>& ModelMaterial::Textures() const {
	return mTextures;
}

void ModelMaterial::InitializeTextureTypeMappings() {
	if (sTextureTypeMappings.size() != TextureTypeEnd) {
		sTextureTypeMappings[TextureTypeDifffuse] = aiTextureType_DIFFUSE;
		sTextureTypeMappings[TextureTypeSpecularMap] = aiTextureType_SPECULAR;
		sTextureTypeMappings[TextureTypeAmbient] = aiTextureType_AMBIENT;
		sTextureTypeMappings[TextureTypeHeightmap] = aiTextureType_HEIGHT;
		sTextureTypeMappings[TextureTypeNormalMap] = aiTextureType_NORMALS;
		sTextureTypeMappings[TextureTypeSpecularPowerMap] = aiTextureType_SHININESS;
		sTextureTypeMappings[TextureTypeDisplacementMap] = aiTextureType_DISPLACEMENT;
		sTextureTypeMappings[TextureTypeLightMap] = aiTextureType_LIGHTMAP;
	}
}
