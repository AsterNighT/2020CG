#pragma once
#include "ShaderProgram.h"
#include "Scene.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "Camera.h"
class Camera;
class FrameBuffer;
class Scene;
class ShaderProgram;
class Render {
public:
	Render(int, int);
	void draw();
	std::vector<std::string> ImportItems(std::string InportMeshFilename);
	void updatetextureMapID(int ItemID, int TextureID);
	void updateItemWorldMatrix(int ItemID, mat4 WorldMatrix);
	void updateLight(vec3 pos, vec3 tar, float strengh);
	void updateItemName(int ItemID, std::string ItemRename);
	void updateCameraPos(vec3 pos);
	void updateCameraFront(vec3 pos);
	void updateCameraLookAt(vec3 pos);
	void updateCameraFOV(float fov);
	vec3 getCameraPos();
	vec3 getCameraFront();
	float getCameraFOV();
	void updateExpObj(bool fExpObj, std::string meshFilename);

	void initializeOpenGLObject();
	void initialize();

	float aspectRatio() const;
	int width;
	int height;
private:
	ShaderProgram* shaderProgram1;
	ShaderProgram* shaderProgram2;
	Scene* scene;
	Camera* camera;
	Texture* firstPassTargetTexture;
	FrameBuffer* firstPassTarget;
};

