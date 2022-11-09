#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "obj.h"
#include "math.h"

extern unsigned int res[2];

extern glm::vec3 camPos;

extern glm::mat4 view;

extern glm::mat4 proj;

Obj::Obj(GLfloat* vtc, GLuint* idc, unsigned int noEl, std::string nameVtx, std::string nameFrag) :
	_prog(nameVtx, nameFrag),
	_noEl(noEl),
	_model(glm::mat4(1.0)) {
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
		_prog.use();

		GLint attrPos = glGetAttribLocation(_prog._id, "pos");
		glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
		glEnableVertexAttribArray(attrPos);

		_uni[MODEL] = glGetUniformLocation(_prog._id, "model");
		_uni[VIEW] = glGetUniformLocation(_prog._id, "view");
		_uni[PROJ] = glGetUniformLocation(_prog._id, "proj");

		glUniformMatrix4fv(_uni[MODEL], 1, GL_FALSE, glm::value_ptr(_model));
		glUniformMatrix4fv(_uni[VIEW], 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(_uni[PROJ], 1, GL_FALSE, glm::value_ptr(proj));

		_prog.unUse();
	}

void Obj::draw() {
	glBindVertexArray(_vao);
	_prog.use();

	glUniformMatrix4fv(_uni[MODEL], 1, GL_FALSE, glm::value_ptr(_model));
	glUniformMatrix4fv(_uni[VIEW], 1, GL_FALSE, glm::value_ptr(view));

	glDrawElements(GL_TRIANGLES, _noEl, GL_UNSIGNED_INT, (GLvoid*) 0);

	_prog.unUse();
	glBindVertexArray(0);
}
