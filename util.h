#pragma once

#include <string>
#include <glm/glm.hpp>

namespace util {
	std::string rd(std::string name);

	glm::vec3 ndc(glm::vec4 clip);
}
