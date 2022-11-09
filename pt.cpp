#include "pt.h"

const GLfloat vtx[3] = {
	0.0, 0.0, 0.0
};

const GLuint idx[1] = {
	0
};

Pt::Pt(GLfloat* vtx, std::string nameVtx, std::string nameFrag) :
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

void Pt::draw() {
	glBindVertexArray(_vao);
	prog.use();

	glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (GLvoid*) 0);

	prog.unUse();
	glBindVertexArray(0);
}
