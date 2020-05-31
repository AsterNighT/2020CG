#include "MainScene.h"
#include "Utility.h"
MainScene::MainScene(Camera* camera, int width, int height) :Scene(width, height), colorShader(camera) {
	this->camera = camera;
}

void MainScene::draw() {
	colorShader.GetShader()->bind();
	colorShader.configurate();
	for (auto item : items) {
		item->configurate(&colorShader);
		item->draw();
	}
}

void MainScene::initialize() {
	colorShader.CompileFromFile("shader/color0");
	colorShader.initialize();
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
	auto item = new Item();
	item->mesh = new Mesh(srf);
	item->worldMatrix = glm::translate(glm::identity<mat4>(),vec3(2, 2, 2));
	items.emplace_back(item);
}
