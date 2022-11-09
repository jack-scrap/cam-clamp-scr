#pragma once

#include <string>
#include <GL/glew.h>

#include "prog.h"

class Pt {
	private:
		GLuint _vao;

	public:
		static GLfloat vtx[3];

		static GLuint idx[1];

		Prog prog;

		Pt(GLfloat* vtx, std::string nameVtx, std::string nameFrag);

		void draw();
};
