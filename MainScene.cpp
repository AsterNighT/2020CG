#include "MainScene.h"
#include "Utility.h"
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

std::string num2str(int x)
{
	std::ostringstream os; //����һ������ַ�������������Ϊ�� 
	os << x; //������ַ����������int����i������ 
	return os.str(); //�����ַ�������str������ȡ���е�����
}

MainScene::MainScene(Camera* camera, int width, int height) :Scene(width, height), colorShader(camera) {
	this->camera = camera;
}
void MainScene::updatetextureMapID(int ItemID, int TextureID) {
	items[ItemID]->texture=Textures[TextureID];
}
void MainScene::updateItemWorldMatrix(int ItemID, mat4 WorldMatrix) {
	items[ItemID]->updateWorldMatrix(WorldMatrix);
}
void MainScene::updateItemName(int ItemID, std::string ItemRename) {
	items[ItemID]->name=ItemRename;
	std::cout << "Items "<< ItemID<< " name "<<items[ItemID]->name  << std::endl;
}
void MainScene::updateItemSpecpower(int ItemID, float specPower) {
	items[ItemID]->specPower = specPower;
}
void MainScene::updateLight(vec3 pos, vec3 tar, float strengh) {
	light.UpdateLight(pos, tar, strengh);
};

void MainScene::updateExpObj(bool _fExpObj, std::string _meshFilename) {
	fExpObj = _fExpObj;
	meshFilename = _meshFilename;
};
std::vector<std::string> MainScene::ImportItems(std::string ImportMeshFilename) {
	std::vector<Item*>vectorOfItem = Model::loadModel(ImportMeshFilename);
	std::cout << vectorOfItem.size() << std::endl;

	Item* item;
	std::vector<std::string> ret;
	ret.clear();

	for (int i = 0; i < vectorOfItem.size(); i++) {
		std::cout << vectorOfItem.size() << std::endl;
		item = vectorOfItem[i];
		ret.push_back(item->name);
		item->texture = new Texture(colorShader.GetShader(), "asset/floor.jpg", "colorTexture", 0);
		items.emplace_back(item);
	}
	return ret;
}
void MainScene::draw() {
	shadowShader.GetShader()->bind();
	shadowShader.configurate();
	light.configurateDepth(&shadowShader);
	for (auto item : items) {
		item->configurateDepth(&shadowShader);
		item->drawDepth();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	colorShader.GetShader()->bind();
	colorShader.configurate();
	light.configurate(&colorShader);
	if (fExpObj) {
		std::cout << meshFilename << std::endl;
		std::ofstream file(meshFilename);
		file << toObjFile();
	}
	for (auto item : items) {
		item->configurate(&colorShader);
		item->draw();
	}
}

void MainScene::initialize() {
	colorShader.CompileFromFile("shader/color0");
	colorShader.initialize();
	shadowShader.CompileFromFile("shader/depth0");
	shadowShader.initialize();
	light.initialize(&colorShader);
	light.position = vec3(0, 1, 3);
	light.color = vec3(1, 1, 1);
	light.target = vec3(0, 0, 0);
	light.strength = 10.0f;

	//texture preload
	Textures.clear();
	Texture* newtex = new Texture(colorShader.GetShader(), "asset/floor.jpg", "colorTexture", 0);
	Textures.push_back(newtex);
	for (int i = 1; i <= 40; i++) {
		static std::string filename;
		std::string tmp = num2str(i);
		if (tmp.length() == 1) tmp = "0" + tmp;
		filename = "asset/nasa/nasa" + tmp + ".jpg";
		//filename = "asset/meya.jpg";
		std::cout << filename << std::endl;
		Texture * newtex=new Texture(colorShader.GetShader(), filename.c_str(), "colorTexture", 0);
		Textures.push_back(newtex);
	}
	

	auto t = Model::loadModel("obj/cube.obj");
	t.at(0)->name = "cube1";
	//items.insert(items.end(), t.begin(), t.end());
	tinynurbs::RationalSurface3f srf;
	srf.degree_u = 3;
	srf.degree_v = 3;
	srf.knots_u = { 0, 0, 0, 0, 1, 1, 1, 1 };
	srf.knots_v = { 0, 0, 0, 0, 1, 1, 1, 1 };
	// 4x4 grid (tinynurbs::array2) of control points and weights
	// https://www.geometrictools.com/Documentation/NURBSCircleSphere.pdf
	srf.control_points = { 4, 4,
						  {glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1),
						   glm::vec3(2, 0, 1), glm::vec3(2, 4, 1),  glm::vec3(-2, 4, 1),  glm::vec3(-2, 0, 1),
						   glm::vec3(2, 0, -1), glm::vec3(2, 4, -1), glm::vec3(-2, 4, -1), glm::vec3(-2, 0, -1),
						   glm::vec3(0, 0, -1), glm::vec3(0, 0, -1), glm::vec3(0, 0, -1), glm::vec3(0, 0, -1)
						  }
	};
	srf.weights = { 4, 4,
				   {1,       1.f / 3.f, 1.f / 3.f, 1,
					1.f / 3.f, 1.f / 9.f, 1.f / 9.f, 1.f / 3.f,
					1.f / 3.f, 1.f / 9.f, 1.f / 9.f, 1.f / 3.f,
					1,       1.f / 3.f, 1.f / 3.f, 1
				   }
	};
	items.clear();
	auto item = new Item();
	item->mesh = new Mesh(srf);
	item->texture = new Texture(colorShader.GetShader(), "asset/floor.jpg", "colorTexture", 0);
	items.emplace_back(item);

	item = Model::loadModel("obj/plane.obj").at(0);
	item->texture = new Texture(colorShader.GetShader(), "asset/floor.jpg", "colorTexture", 0);
	items.emplace_back(item);
	
	item = new Item();
	item->mesh = Mesh::createPrism();
	item->texture = new Texture(colorShader.GetShader(), "asset/floor.jpg", "colorTexture", 0);
	items.emplace_back(item);
	
	item = Model::loadModel("obj/bunny_1k.obj").at(0);
	item->texture = new Texture(colorShader.GetShader(), "asset/meya.jpg", "colorTexture", 0);
	items.emplace_back(item);
	//toObjFile();
}

std::string MainScene::toObjFile() {
	std::string s;
	int texCount = 1;
	int vertCount = 1;
	int normalCount = 1;
	for (auto& item : items) {
		s+=item->toObjFile(vertCount,texCount,normalCount);
	}
	//std::cout << s << std::endl;
	return s;
}
