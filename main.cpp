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

int main() {
	Disp disp("asdf", res[X], res[Y]);

	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.3, 0.7, 0.12));

	glm::mat4 view = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::perspective(glm::radians(45.0), res[X] / (double) res[Y], 0.1, 100.0);

	/* First */
	// data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// index
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (GLint), idx, GL_STATIC_DRAW);

	glm::vec4 vtxVec;
	for (int a = 0; a < 3; a++) {
		vtxVec[a] = vtx[a];
	}
	vtxVec[3] = 1;

	// Normalized device space
	glm::vec3 vtxNorm = util::ndc(vtxVec, model, view, proj);

	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof (GLfloat), &vtxNorm[0], GL_STATIC_DRAW);

	// shader
	Prog prog("ndc", "white");

	prog.use();

	/// attribute
	GLint attrPos = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPos);

	prog.unUse();

	/* Second */
	// data
	GLuint vaoSnd;
	glGenVertexArrays(1, &vaoSnd);
	glBindVertexArray(vaoSnd);

	// position
	GLuint vboSnd;
	glGenBuffers(1, &vboSnd);
	glBindBuffer(GL_ARRAY_BUFFER, vboSnd);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof (GLfloat), vtx, GL_STATIC_DRAW);

	// index
	GLuint iboSnd;
	glGenBuffers(1, &iboSnd);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboSnd);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (GLint), idx, GL_STATIC_DRAW);

	// matrix
	// shader
	Prog progSnd("world", "red");

	progSnd.use();

	/// attribute
	GLint attrPosSnd = glGetAttribLocation(progSnd._id, "pos");
	glVertexAttribPointer(attrPosSnd, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPosSnd);

	/// uniform
	GLint
		uniModelSnd = glGetUniformLocation(progSnd._id, "model"),
		uniViewSnd = glGetUniformLocation(progSnd._id, "view"),
		uniProjSnd = glGetUniformLocation(progSnd._id, "proj");

	glUniformMatrix4fv(uniModelSnd, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniViewSnd, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniProjSnd, 1, GL_FALSE, glm::value_ptr(proj));

	progSnd.unUse();

	SDL_Event e;
	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				SDL_Quit();
			}
		}

		disp.clear(0, 0, 0, 1);

		glPointSize(16);

		glBindVertexArray(vao);
		prog.use();

		glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (GLvoid*) 0);

		prog.unUse();
		glBindVertexArray(0);

		glPointSize(8);

		glBindVertexArray(vaoSnd);
		progSnd.use();

		glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (GLvoid*) 0);

		progSnd.unUse();
		glBindVertexArray(0);

		disp.update();
	}
}
