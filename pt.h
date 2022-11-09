#pragma once

#include <string>
#include <GL/glew.h>

#include "prog.h"

extern const GLfloat vtx[3];

extern const GLuint idx[1];

class Pt {
	private:
		GLuint _vao;

	public:
		Prog prog;

		Pt(GLfloat* vtx, std::string nameVtx, std::string nameFrag);

		void draw();
};
