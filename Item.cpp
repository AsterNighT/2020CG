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

std::string Item::toObjFile(int& vertCount, int& texCoordCount, int& normalCount) {
	std::string s = "o " + name + "\n";
	auto& vertices = mesh->Vertices();
	auto& normals = mesh->Normals();
	auto& tex = mesh->TextureCoordinates();
	auto& indices = mesh->Indices();
	bool hasTexCoord = !tex.empty();
	bool hasIndices = !indices.empty();
	for (int i = 0; i < vertices.size(); i++) {
		s += "v " + std::to_string(vertices[i].x) + " " + std::to_string(vertices[i].y) + " " + std::to_string(vertices[i].z) + "\n";
	}
	if (hasTexCoord) {
		auto texCoords = tex[0];
		for (int i = 0; i < texCoords->size(); i++) {
			s += "vt " + std::to_string(texCoords->at(i).x) + " " + std::to_string(texCoords->at(i).y) + "\n";
		}
	}
	for (int i = 0; i < normals.size(); i++) {
		s += "vn " + std::to_string(normals[i].x) + " " + std::to_string(normals[i].y) + " " + std::to_string(normals[i].z) + "\n";
	}
	if (hasIndices) {
		for (int i = 0; i < indices.size(); i += 3) {
			s += "f";
			for (int j = 0; j < 3; j++) {
				s += " ";
				s += std::to_string(indices[i + j] + vertCount);
				s += "/";
				if (hasTexCoord) s += std::to_string(indices[i + j] + texCoordCount);
				s += "/";
				s += std::to_string(indices[i + j] + normalCount);
			}
			s += "\n";
		}
	}
	s += "\n";
	vertCount += vertices.size();
	normalCount += normals.capacity();
	if (hasTexCoord) texCoordCount += tex[0]->size();
	return s;
}
