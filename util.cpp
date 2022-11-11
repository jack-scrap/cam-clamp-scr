#include <fstream>
#include <iostream>

#include "util.h"

std::string util::rd(std::string name) {
	std::ifstream in;
	in.open("./" + name);

	std::string total;
	for (
		std::string l;
		std::getline(in, l);
	) {
		total += l + "\n";
	}

	in.close();

	return total;
}

glm::vec2 util::ndc(glm::vec4 vtx, glm::mat4 model, glm::mat4 view, glm::mat4 proj) {
	// Matrix is left-hand operand given being column-major
	// World space
	vtx = model * vtx;

	// Camera space
	vtx = view * vtx;

	// Clip space
	vtx = proj * vtx;

	glm::vec3 _;
	for (int a = 0; a < 3; a++) {
		_[a] = vtx[a] / vtx[3];
	}

	return glm::vec2(_);
}
