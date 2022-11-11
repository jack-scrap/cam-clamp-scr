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
#include "obj.h"

const unsigned int res[2] = {
	800,
	600
};

glm::vec3 camPos;

glm::mat4 model;

glm::mat4 view;

glm::mat4 proj;

GLfloat bound[2][2];
Pt* ptBound[2][2];

GLfloat vtcCube[2 * 2 * 2 * 3];

glm::vec2 vtcNorm[(sizeof vtcCube / sizeof *vtcCube) / 3];

GLfloat vtcBound[2 * 2 * 2];

GLuint vbo;

bool save = false;

const GLfloat speed = 0.1;

void calcBound() {
	for (int i = 0; i < sizeof vtcNorm / sizeof *vtcNorm; i++) {
		glm::vec4 vtxVec;
		for (int a = 0; a < 3; a++) {
			vtxVec[a] = vtcCube[(i * 3) + a];
		}
		vtxVec[3] = 1;

		// Normalized device space
		vtcNorm[i] = util::ndc(vtxVec, model, view, proj);
	}

	for (int a = 0; a < 2; a++) {
		for (int b = 0; b < 2; b++) {
			bound[a][b] = vtcNorm[a][b];

			ptBound[a][b] = nullptr;
		}
	}

	for (int i = 0; i < sizeof vtcNorm / sizeof *vtcNorm; i++) {
		if (vtcNorm[i][X] < bound[X][MIN]) {
			bound[X][MIN] = vtcNorm[i][X];

			ptBound[X][MIN] = new Pt(glm::value_ptr(vtcNorm[i]), "ndc", "white");
		}
		if (vtcNorm[i][X] > bound[X][MAX]) {
			bound[X][MAX] = vtcNorm[i][X];

			ptBound[X][MAX] = new Pt(glm::value_ptr(vtcNorm[i]), "ndc", "white");
		}

		if (vtcNorm[i][Y] < bound[Y][MIN]) {
			bound[Y][MIN] = vtcNorm[i][Y];

			ptBound[Y][MIN] = new Pt(glm::value_ptr(vtcNorm[i]), "ndc", "white");
		}
		if (vtcNorm[i][Y] > bound[Y][MAX]) {
			bound[Y][MAX] = vtcNorm[i][Y];

			ptBound[Y][MAX] = new Pt(glm::value_ptr(vtcNorm[i]), "ndc", "white");
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	vtcBound[0] = bound[X][MIN];
	vtcBound[1] = bound[Y][MIN];

	vtcBound[2] = bound[X][MAX];
	vtcBound[3] = bound[Y][MIN];

	vtcBound[4] = bound[X][MIN];
	vtcBound[5] = bound[Y][MAX];

	vtcBound[6] = bound[X][MAX];
	vtcBound[7] = bound[Y][MAX];

	glBufferData(GL_ARRAY_BUFFER, sizeof vtcBound, vtcBound, GL_STATIC_DRAW);
}

bool scr(std::string fPath, SDL_Window* win, SDL_Renderer* rend) {
	SDL_Surface* surfSave = NULL;
	SDL_Surface* surfInfo = SDL_GetWindowSurface(win);

	if (!surfInfo) {
		std::cerr << "Failed to create info surface from window in save(string), SDL_GetError() - " << SDL_GetError() << std::endl;
	} else {
		unsigned char pix[surfInfo->w * surfInfo->h * surfInfo->format->BytesPerPixel];
		if (!pix) {
			std::cerr << "Unable to allocate memory for screenshot pixel data buffer!" << std::endl;

			return false;
		} else {
			if (SDL_RenderReadPixels(rend, &surfInfo->clip_rect, surfInfo->format->format, pix, surfInfo->w * surfInfo->format->BytesPerPixel) != 0) {
				std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << std::endl;

				return false;
			} else {
				surfSave = SDL_CreateRGBSurfaceFrom(pix, surfInfo->w, surfInfo->h, surfInfo->format->BitsPerPixel, surfInfo->w * surfInfo->format->BytesPerPixel, surfInfo->format->Rmask, surfInfo->format->Gmask, surfInfo->format->Bmask, surfInfo->format->Amask);

				if (!surfSave) {
					std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << std::endl;

					return false;
				}

				SDL_SaveBMP(surfSave, fPath.c_str());
				SDL_FreeSurface(surfSave);
				surfSave = NULL;
			}
		}

		SDL_FreeSurface(surfInfo);
		surfInfo = NULL;
	}

	return true;
}

int main() {
	Disp disp("asdf", res[X], res[Y]);

	camPos = glm::vec3(5.0, 5.0, 5.0);

	model = glm::mat4(1.0);

	view = glm::lookAt(camPos, camPos + glm::vec3(-10, -10, -10), glm::vec3(0, 1, 0));

	proj = glm::perspective(glm::radians(45.0), res[X] / (double) res[Y], 0.1, 100.0);

	/* Cube */
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

	Obj cube(vtcCube, idcCube, sizeof idcCube / sizeof *idcCube, "cube", "cube");

	/* Points */
	calcBound();

	/* Bound */
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Position
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	vtcBound[0] = bound[X][MIN];
	vtcBound[1] = bound[Y][MIN];

	vtcBound[2] = bound[X][MAX];
	vtcBound[3] = bound[Y][MIN];

	vtcBound[4] = bound[X][MIN];
	vtcBound[5] = bound[Y][MAX];

	vtcBound[6] = bound[X][MAX];
	vtcBound[7] = bound[Y][MAX];

	glBufferData(GL_ARRAY_BUFFER, sizeof vtcBound, vtcBound, GL_STATIC_DRAW);

	// Index
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	const GLuint idcBound[] = {
		0, 1, 2,
		2, 1, 3
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof idcBound, idcBound, GL_STATIC_DRAW);

	Prog prog("ndc", "trans");

	prog.use();

	GLint attrPos = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPos, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPos);

	prog.unUse();

	SDL_Event e;
	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_RIGHT) {
					camPos[X] += speed;
				}

				if (e.key.keysym.sym == SDLK_LEFT) {
					camPos[X] -= speed;
				}

				if (e.key.keysym.sym == SDLK_UP) {
					camPos[Y] += speed;
				}

				if (e.key.keysym.sym == SDLK_DOWN) {
					camPos[Y] -= speed;
				}

				view = glm::lookAt(camPos, camPos + glm::vec3(-10, -10, -10), glm::vec3(0, 1, 0));

				calcBound();

				if (e.key.keysym.sym == SDLK_F12) {
					save = true;
				}
			}

			if (e.type == SDL_QUIT) {
				SDL_Quit();
			}
		}

		disp.clear(0, 0, 0, 1);

		/* Objects */
		glEnable(GL_DEPTH_TEST);

		cube.draw();

		/* Points */
		glDisable(GL_DEPTH_TEST);

		for (int a = 0; a < 2; a++) {
			for (int b = 0; b < 2; b++) {
				if (ptBound[a][b]) {
					ptBound[a][b]->draw();
				}
			}
		}

		/* Bounds */
		glBindVertexArray(vao);
		prog.use();

		glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, (GLvoid*) 0);

		prog.unUse();
		glBindVertexArray(0);

		disp.update();

		if (save) {
			scr("o/scr.png", disp.win, disp.rend);

			save = false;
		}
	}
}
