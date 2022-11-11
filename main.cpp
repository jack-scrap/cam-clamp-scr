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

glm::vec3 camPos = glm::vec3(3.0, 3.0, 7.0);

glm::mat4 view = glm::lookAt(camPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

glm::mat4 proj = glm::perspective(glm::radians(45.0), res[X] / (double) res[Y], 0.1, 100.0);

bool save = false;

bool scr(std::string fPath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer) {
	SDL_Surface* surfSave = NULL;
	SDL_Surface* surfInfo = SDL_GetWindowSurface(SDLWindow);

	if (surfInfo == NULL) {
		std::cerr << "Failed to create info surface from window in save(string), SDL_GetError() - " << SDL_GetError() << "\n";
	} else {
		unsigned char* pix = new (std::nothrow) unsigned char[surfInfo->w * surfInfo->h * surfInfo->format->BytesPerPixel];
		if (!pix) {
			std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";

			return false;
		} else {
			if (SDL_RenderReadPixels(SDLRenderer, &surfInfo->clip_rect, surfInfo->format->format, pix, surfInfo->w * surfInfo->format->BytesPerPixel) != 0) {
				std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";

				delete[] pix;

				return false;
			} else {
				surfSave = SDL_CreateRGBSurfaceFrom(pix, surfInfo->w, surfInfo->h, surfInfo->format->BitsPerPixel, surfInfo->w * surfInfo->format->BytesPerPixel, surfInfo->format->Rmask, surfInfo->format->Gmask, surfInfo->format->Bmask, surfInfo->format->Amask);

				if (surfSave == NULL) {
					std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";

					delete[] pix;

					return false;
				}

				SDL_SaveBMP(surfSave, fPath.c_str());
				SDL_FreeSurface(surfSave);
				surfSave = NULL;
			}

			delete[] pix;
		}

		SDL_FreeSurface(surfInfo);
		surfInfo = NULL;
	}

	return true;
}

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

	Obj cube(vtcCube, idcCube, sizeof idcCube / sizeof *idcCube, "cube", "cube");

	/* Points */
	glm::mat4 model = glm::mat4(1.0);

	glm::vec3 vtcNorm[(sizeof vtcCube / sizeof *vtcCube) / 3];
	for (int i = 0; i < sizeof vtcNorm / sizeof *vtcNorm; i++) {
		// Calculated prior
		glm::vec4 vtxVec;
		for (int a = 0; a < 3; a++) {
			vtxVec[a] = vtcCube[(i * 3) + a];
		}
		vtxVec[3] = 1;

		// Normalized device space
		vtcNorm[i] = util::ndc(vtxVec, model, view, proj);
	}

	GLfloat bound[2][2];
	for (int a = 0; a < 2; a++) {
		for (int b = 0; b < 2; b++) {
			bound[a][b] = 0.0;
		}
	}
	Pt* ptBound[2][2];
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

	/* Bound */
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Position
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat vtc[2 * 2 * 3] = {
		bound[X][MIN], bound[Y][MIN], 0.0,
		bound[X][MAX], bound[Y][MIN], 0.0,
		bound[X][MIN], bound[Y][MAX], 0.0,
		bound[X][MAX], bound[Y][MAX], 0.0
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof vtc, vtc, GL_STATIC_DRAW);

	// Index
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	const GLuint idc[] = {
		0, 1, 2,
		2, 1, 3
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof idc, idc, GL_STATIC_DRAW);

	Prog prog("ndc", "trans");

	prog.use();

	GLint attrPos = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPos);

	prog.unUse();

	SDL_Event e;
	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_RIGHT) {
					camPos[X]++;
				}

				if (e.key.keysym.sym == SDLK_LEFT) {
					camPos[X]--;
				}

				if (e.key.keysym.sym == SDLK_UP) {
					camPos[Y]++;
					view = glm::lookAt(camPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
				}

				if (e.key.keysym.sym == SDLK_DOWN) {
					camPos[Y]--;
				}

				view = glm::lookAt(camPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

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

		glPointSize(16);

		for (int a = 0; a < 2; a++) {
			for (int b = 0; b < 2; b++) {
				ptBound[a][b]->draw();
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
