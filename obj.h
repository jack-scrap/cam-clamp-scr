#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "prog.h"

class Obj {
	private:
		unsigned int _noEl;

		glm::mat4 _model;

		GLuint _vao;

		GLint _uni[3];

		Prog _prog;

		enum matrix {
			MODEL,
			VIEW,
			PROJ
		};

	public:
		Obj(GLfloat* vtc, GLuint* idc, unsigned int noEl, std::string nameVtx, std::string nameFrag);

		void draw();
};
