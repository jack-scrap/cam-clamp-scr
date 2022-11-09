#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "disp.h"
#include "prog.h"
#include "util.h"
#include "math.h"
#include "pt.h"

const unsigned int res[2] = {
	800,
	600
};

int main() {
	Disp disp("asdf", res[X], res[Y]);

	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.3, 0.7, 0.12));

	glm::mat4 view = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 proj = glm::perspective(glm::radians(45.0), res[X] / (double) res[Y], 0.1, 100.0);

	/* Uploaded to GPU */
	Pt pt((GLfloat*) Pt::vtx, "world", "white");

	pt.prog.use();

	GLint uniModel = glGetUniformLocation(pt.prog._id, "model");
	GLint uniView = glGetUniformLocation(pt.prog._id, "view");
	GLint uniProj = glGetUniformLocation(pt.prog._id, "proj");

	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	pt.prog.unUse();

	/* Calculated prior */
	glm::vec4 vtxVec;
	for (int a = 0; a < 3; a++) {
		vtxVec[a] = Pt::vtx[a];
	}
	vtxVec[3] = 1;

	// Normalized device space
	glm::vec3 vtxNorm = util::ndc(vtxVec, model, view, proj);

	Pt ptNorm(glm::value_ptr(vtxNorm), "ndc", "red");

	SDL_Event e;
	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				SDL_Quit();
			}
		}

		disp.clear(0, 0, 0, 1);

		glPointSize(16);

		pt.draw();

		glPointSize(8);

		ptNorm.draw();

		disp.update();
	}
}
