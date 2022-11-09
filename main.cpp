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

glm::mat4 view = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

glm::mat4 proj = glm::perspective(glm::radians(45.0), res[X] / (double) res[Y], 0.1, 100.0);

class Cube {
	private:
		unsigned int _noEl;

		GLuint _vao;

		GLint _uniModel;
		GLint _uniView;
		GLint _uniProj;

		Prog _prog;

	public:
		Cube(GLfloat* vtc, GLuint* idc, unsigned int noEl) :
			_prog("cube", "cube"),
			_noEl(noEl) {
				glGenVertexArrays(1, &_vao);
				glBindVertexArray(_vao);

				// Position
				GLuint vbo;
				glGenBuffers(1, &vbo);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, _noEl * 3 * sizeof (GLfloat), vtc, GL_STATIC_DRAW);

				// Index
				GLuint ibo;
				glGenBuffers(1, &ibo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, _noEl * sizeof (GLuint), idc, GL_STATIC_DRAW);

				// Matrix
				glm::mat4 model = glm::mat4(1.0);

				_prog.use();

				GLint attrPos = glGetAttribLocation(_prog._id, "pos");
				glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
				glEnableVertexAttribArray(attrPos);

				_uniModel = glGetUniformLocation(_prog._id, "model");
				_uniView = glGetUniformLocation(_prog._id, "view");
				_uniProj = glGetUniformLocation(_prog._id, "proj");

				glUniformMatrix4fv(_uniModel, 1, GL_FALSE, glm::value_ptr(model));
				glUniformMatrix4fv(_uniView, 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(_uniProj, 1, GL_FALSE, glm::value_ptr(proj));

				_prog.unUse();
			}

		void draw() {
			glBindVertexArray(_vao);
			_prog.use();

			glDrawElements(GL_TRIANGLES, _noEl, GL_UNSIGNED_INT, (GLvoid*) 0);

			_prog.unUse();
			glBindVertexArray(0);
		}
};

int main() {
	Disp disp("asdf", res[X], res[Y]);

	/* Cube */
	GLfloat vtcCube[2 * 2 * 2 * 3];
	int i = 0;
	for (int z = 0; z < 2; z++) {
		for (int y = 0; y < 2; y++) {
			for (int x = 0; x < 2; x++) {
				vtcCube[i] = x ? 1 : -1;
				vtcCube[i + 1] = y ? 1 : -1;
				vtcCube[i + 2] = z ? 1 : -1;

				i += 3;
			}
		}
	}

	GLuint idcCube[3 * 2 * 3 * 2] = {
		0, 1, 2,
		2, 1, 3,

		4, 5, 6,
		6, 5, 7,

		0, 4, 1,
		1, 4, 5,

		2, 6, 3,
		3, 6, 7,

		0, 4, 2,
		2, 4, 6,

		1, 5, 3,
		3, 5, 7
	};

	Cube cube(vtcCube, idcCube, sizeof idcCube / sizeof *idcCube);

	/* Points */
	glm::mat4 model = glm::mat4(1.0);

	// Calculated prior
	glm::vec4 vtxVec;
	for (int a = 0; a < 3; a++) {
		vtxVec[a] = vtcCube[a];
	}
	vtxVec[3] = 1;

	// Normalized device space
	glm::vec3 vtxNorm = util::ndc(vtxVec, model, view, proj);

	Pt ptNorm(glm::value_ptr(vtxNorm), "ndc", "white");

	SDL_Event e;
	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				SDL_Quit();
			}
		}

		disp.clear(0, 0, 0, 1);

		glEnable(GL_DEPTH_TEST);

		cube.draw();

		glDisable(GL_DEPTH_TEST);

		glPointSize(16);

		ptNorm.draw();

		disp.update();
	}
}
