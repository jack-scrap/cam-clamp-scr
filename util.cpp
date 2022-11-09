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

glm::vec3 util::ndc(glm::vec4 clip) {
	glm::vec3 vtx;
	for (int a = 0; a < 3; a++) {
		vtx[a] = clip[a] / clip[3];
	}

	return vtx;
}
