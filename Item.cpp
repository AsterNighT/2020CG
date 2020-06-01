#include "Item.h"

Item::Item() {
	texture = nullptr;
	mesh = nullptr;
	worldMatrix = glm::identity<mat4>();
}

void Item::draw() {
	if (texture != nullptr) texture->bind();
	mesh->bind();
	if (!mesh->Indices().empty()) {
		glDrawElements(GL_TRIANGLES, mesh->Indices().size(), GL_UNSIGNED_INT, 0);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, mesh->Vertices().size());
	}
}

void Item::drawDepth() {
	mesh->bind();
	if (!mesh->Indices().empty()) {
		glDrawElements(GL_TRIANGLES, mesh->Indices().size(), GL_UNSIGNED_INT, 0);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, mesh->Vertices().size());
	}
}

void Item::configurate(ShadePass* shadePass) {
	*(*shadePass)[std::string("worldMatrix")] << worldMatrix;
	*(*shadePass)[std::string("ambientPower")] << 0.1f;
	*(*shadePass)[std::string("specPower")] << 1.0f;
}

void Item::configurateDepth(ShadePass* shadePass) {
	*(*shadePass)[std::string("worldMatrix")] << worldMatrix;
}
