#pragma once

#include <string>
#include <glm/glm.hpp>

namespace util {
	std::string rd(std::string name);

	glm::vec2 ndc(glm::vec4 vtx, glm::mat4 model, glm::mat4 view, glm::mat4 proj);
}
