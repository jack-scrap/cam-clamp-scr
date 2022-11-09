#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "disp.h"
#include "prog.h"
#include "util.h"

int main() {
	Disp disp("asdf", 800, 600);

	// data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat vtc[3] = {
		0.0, 0.0, 0.0
	};

	// index
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	GLuint idc[1] = {
		0
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof idc, idc, GL_STATIC_DRAW);

	// matrix
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.3, 0.7, 0.12));

	glm::mat4 view = glm::lookAt(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0);

	glm::vec4 vtx = glm::vec4(vtc[0], vtc[1], vtc[2], 1.0);

	// Matrix is left-hand operand given being column-major
	// World space
	vtx = model * vtx;

	// Camera space
	vtx = view * vtx;

	// Clip space
	vtx = proj * vtx;

	// Normalized device space
	for (int a = 0; a < 3 + 1; a++) {
		vtx[a] = vtx[a] / vtx[3];
	}

	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof (GLfloat), &vtx[0], GL_STATIC_DRAW);

	// shader
	Prog prog("main", "white");

	/// attribute
	GLint attrPos = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPos);

	/// uniform
	GLint
		uniModel = glGetUniformLocation(prog._id, "model"),
		uniView = glGetUniformLocation(prog._id, "view"),
		uniProj = glGetUniformLocation(prog._id, "proj");

	// initialize
	prog.use();

	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	prog.unUse();

	SDL_Event e;
	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				SDL_Quit();
			}
		}

		glBindVertexArray(vao);
		prog.use();

		disp.clear(0, 0, 0, 1);

		glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (GLvoid*) 0);

		prog.unUse();
		glBindVertexArray(0);

		disp.update();
	}
}
