#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class GLMHelper {
public:
	static glm::vec3 getVec3(const float *p){
		return glm::vec3(p[0], p[1], p[2]);
	}
	static glm::vec4 getVec4(const float* p) {
		return glm::vec4(p[0], p[1], p[2],p[3]);
	}
};

