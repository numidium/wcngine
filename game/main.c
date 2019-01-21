#include <SDL.h>
#include <stdio.h>

int main(int argc, char** argv) 
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) //sfdsfd
	{
		printf(SDL_GetError());
	}
	else
	{
		printf("SDL appears to be working.\n");
	}

	SDL_Window *window = SDL_CreateWindow(
				"WCngine",
				SDL_WINDOWPOS_UNDEFINED,
				SDL_WINDOWPOS_UNDEFINED,
				640,
				480,
				0);
	if (window == NULL)
	{
		printf(SDL_GetError());
		return 1;
	}

	SDL_Event e;
	while (SDL_WaitEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			printf("User has quit.\n");
			break;
		}
	}

	SDL_Quit();

	return 0;
}