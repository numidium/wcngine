#include <stdio.h>
#include <SDL.h>
#include <GL/glew.h>
#include <gl/GL.h>
#include <GL/freeglut.h>
#include <SDL_opengl.h>
#pragma comment(lib, "glew32.lib")

void RenderScene();
void moveCam(float, float, float);

float camX, camY, camZ, camYaw;

int main(int argc, char** argv) 
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL failed to initialize: %s\n", SDL_GetError());
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_Window* window = SDL_CreateWindow("WCngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 768, SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		printf(SDL_GetError());
		return 1;
	}

	// Initialize GLEW
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL)
	{
		printf("Could not create GLContext: %s\n", SDL_GetError());
	}
	GLenum glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK)
	{
		printf("GLEW Initialization failed: %s\n", glewGetErrorString(glewInitResult));
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderSetLogicalSize(renderer, 640, 480);

	// GL rendering settings
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, 640.0 / 480.0, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);

	camX = 0.0f;
	camY = 0.0f;
	camZ = 2.0f;
	camYaw = 0.0f;

	SDL_GL_MakeCurrent(window, glContext);
	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	glViewport(0, 0, width, height);
	SDL_Event e;
	while (SDL_WaitEvent(&e))
	{
		int hasQuit = 0;
		switch (e.type)
		{
			case SDL_QUIT:
				hasQuit = 1;
				printf("User has quit.\n");
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
					default:
						break;
				}
				break;
			default:
				break;
		}

		camYaw += 1.0f;

		// rendering
		glLoadIdentity();
		glPushMatrix();
			glRotatef(-camYaw, 0.0f, 1.0f, 0.0f);
			glTranslatef(-camX, -camY, -camZ);
			glPushMatrix();
				glLoadIdentity();
				RenderScene();
		glPopMatrix();
			RenderScene();
		glPopMatrix();
		glFlush();

		SDL_GL_SwapWindow(window);

		if (hasQuit)
		{
			break;
		}
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void moveCam(float camYaw, float dist, float dir)
{
	float rad = (camYaw + dir) * (float)M_PI / 180.0f;      //convert the degrees into radians
	camX -= sinf(rad) * dist;    //calculate the new coorinate, if you don't understand, draw a right triangle with the datas, you have
	camZ -= cosf(rad) * dist;    //and try to calculate the new coorinate with trigonometric functions, that should help
}

void RenderScene()
{
	static float color[8][3] = {
	{1.0, 1.0, 0.0},
	{1.0, 0.0, 0.0},
	{0.0, 0.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 1.0},
	{1.0, 1.0, 1.0},
	{1.0, 0.0, 1.0},
	{0.0, 0.0, 1.0}
	};
	static float cube[8][3] = {
		{0.5, 0.5, -0.5},
		{0.5, -0.5, -0.5},
		{-0.5, -0.5, -0.5},
		{-0.5, 0.5, -0.5},
		{-0.5, 0.5, 0.5},
		{0.5, 0.5, 0.5},
		{0.5, -0.5, 0.5},
		{-0.5, -0.5, 0.5}
	};

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);

	glColor3fv(color[0]);
	glVertex3fv(cube[0]);
	glColor3fv(color[1]);
	glVertex3fv(cube[1]);
	glColor3fv(color[2]);
	glVertex3fv(cube[2]);
	glColor3fv(color[3]);
	glVertex3fv(cube[3]);

	glColor3fv(color[3]);
	glVertex3fv(cube[3]);
	glColor3fv(color[4]);
	glVertex3fv(cube[4]);
	glColor3fv(color[7]);
	glVertex3fv(cube[7]);
	glColor3fv(color[2]);
	glVertex3fv(cube[2]);

	glColor3fv(color[0]);
	glVertex3fv(cube[0]);
	glColor3fv(color[5]);
	glVertex3fv(cube[5]);
	glColor3fv(color[6]);
	glVertex3fv(cube[6]);
	glColor3fv(color[1]);
	glVertex3fv(cube[1]);

	glColor3fv(color[5]);
	glVertex3fv(cube[5]);
	glColor3fv(color[4]);
	glVertex3fv(cube[4]);
	glColor3fv(color[7]);
	glVertex3fv(cube[7]);
	glColor3fv(color[6]);
	glVertex3fv(cube[6]);

	glColor3fv(color[5]);
	glVertex3fv(cube[5]);
	glColor3fv(color[0]);
	glVertex3fv(cube[0]);
	glColor3fv(color[3]);
	glVertex3fv(cube[3]);
	glColor3fv(color[4]);
	glVertex3fv(cube[4]);

	glColor3fv(color[6]);
	glVertex3fv(cube[6]);
	glColor3fv(color[1]);
	glVertex3fv(cube[1]);
	glColor3fv(color[2]);
	glVertex3fv(cube[2]);
	glColor3fv(color[7]);
	glVertex3fv(cube[7]);

	glEnd();

	glMatrixMode(GL_MODELVIEW);
}