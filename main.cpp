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

const unsigned int res[2] = {
	800,
	600
};

const GLfloat vtx[3] = {
	0.0, 0.0, 0.0
};

const GLuint idx[1] = {
	0
};

class Pt {
	private:
		GLuint _vao;

	public:
		Prog prog;

		Pt(GLfloat* vtx, std::string nameVtx, std::string nameFrag) :
			prog(nameVtx, nameFrag) {
				glGenVertexArrays(1, &_vao);
				glBindVertexArray(_vao);

				// Position
				GLuint vbo;
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, 3 * sizeof (GLfloat), vtx, GL_STATIC_DRAW);

				// Index
				GLuint ibo;
				glGenBuffers(1, &ibo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (GLuint), idx, GL_STATIC_DRAW);

				prog.use();

				// Attribute
				GLint attrPos = glGetAttribLocation(prog._id, "pos");
				glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
				glEnableVertexAttribArray(attrPos);

				prog.unUse();
			}

		void draw() {
			glBindVertexArray(_vao);
			prog.use();

			glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (GLvoid*) 0);

			prog.unUse();
			glBindVertexArray(0);
		}
};

int main() {
	Disp disp("asdf", res[X], res[Y]);

	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.3, 0.7, 0.12));

	glm::mat4 view = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::perspective(glm::radians(45.0), res[X] / (double) res[Y], 0.1, 100.0);

	/* Uploaded to GPU */
	Pt pt((GLfloat*) vtx, "world", "white");

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
		vtxVec[a] = vtx[a];
	}
	vtxVec[3] = 1;

	// Normalized device space
	glm::vec3 vtxNorm = util::ndc(vtxVec, model, view, proj);

	Pt pt1(&vtxNorm[0], "ndc", "red");

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

		pt1.draw();

		disp.update();
	}
}
