#include <iostream>
#include <string>

#include <GL/glew.h>

#include "prog.h"
#include "util.h"

Shad::Shad(std::string name, bool vtx) {
	std::string ext;
	GLenum type;
	if (vtx) {
		ext = "vs";
		type = GL_VERTEX_SHADER;
	} else {
		ext = "fs";
		type = GL_FRAGMENT_SHADER;
	}

	std::string path = "res/" + std::string(name) + std::string(".") + ext;

	std::string txt = util::rd(path);
	const char* src = txt.c_str();

	GLint succ;
	char buff[] = "";

	_id = glCreateShader(type);
	glShaderSource(_id, 1, &src, NULL);
	glCompileShader(_id);

	glGetShaderiv(_id, GL_COMPILE_STATUS, &succ);
	if (!succ) {
		glGetShaderInfoLog(_id, 512, NULL, buff);
		std::cout << "Error: " << std::endl;
		std::cout << buff << std::endl;
	}
}

Prog::Prog(std::string vtx, std::string frag) {
	/* shader */
	/// vertex
	Shad _vtx(vtx, 1);

	/// fragment
	Shad _frag(frag, 0);

	/// program
	_id = glCreateProgram();
	glAttachShader(_id, _vtx._id);
	glAttachShader(_id, _frag._id);
	glLinkProgram(_id);

	glBindFragDataLocation(_id, 0, "col");
}

void Prog::use() {
	glUseProgram(_id);
}

void Prog::unUse() {
	glUseProgram(0);
}
