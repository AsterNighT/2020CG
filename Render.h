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
	void updatetextureMapID(int ID);
	void updateItemWorldMatrix(int ItemID, mat4 WorldMatrix);
	void updateLight(vec3 pos, vec3 tar, float strengh);
	void updateCameraPos(vec3 pos);
	void updateCameraFront(vec3 pos);
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

