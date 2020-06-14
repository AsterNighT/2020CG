#include "Item.h"

Item::Item() {
	texture = nullptr;
	mesh = nullptr;
	worldMatrix = glm::identity<mat4>();
}

void Item::updateWorldMatrix(mat4 tWorldMatrix) {
	worldMatrix = tWorldMatrix;
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
		vec4 v = worldMatrix * vec4(vertices[i], 1);
		v = v / v.w;
		s += "v " + std::to_string(v.x) + " " + std::to_string(v.y) + " " + std::to_string(v.z) + "\n";
	}
	if (hasTexCoord) {
		auto texCoords = tex[0];
		for (int i = 0; i < texCoords->size(); i++) {
			s += "vt " + std::to_string(texCoords->at(i).x) + " " + std::to_string(texCoords->at(i).y) + "\n";
		}
	}
	for (int i = 0; i < normals.size(); i++) {
		vec4 n = worldMatrix * vec4(normals[i], 0);
		s += "vn " + std::to_string(n.x) + " " + std::to_string(n.y) + " " + std::to_string(n.z) + "\n";
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
	normalCount += normals.size();
	if (hasTexCoord) texCoordCount += tex[0]->size();
	return s;
}
