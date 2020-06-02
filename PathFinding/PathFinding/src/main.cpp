#include <iostream>
#include <vector>
#include <time.h>

#define SDL_MainHandled
#include <SDL.h>
#include "SDL2_gfxPrimitives.h"

#include "grid.h"
#include "AStar.h"

//#define FULL_SCREEN
#define FPS 60

void drawGrid(SDL_Renderer* renderer, Grid& grid, std::vector<std::pair<int, int>> path, int width, int height)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	SDL_Rect rect;
	rect.w = width / grid.width;
	rect.h = height / grid.height;
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	for (const std::pair<int, int>& pos : path)
	{
		rect.x = pos.first * rect.w;
		rect.y = pos.second * rect.h;
		SDL_RenderFillRect(renderer, &rect);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	for (int y = 0; y < grid.height; y++)
	{
		for (int x = 0; x < grid.width; x++)
		{
			if (grid[y][x])
			{
				rect.x = x * rect.w;
				rect.y = y * rect.h;
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	int mapped;

	for (int y = 0; y < grid.height; y++)
	{
		mapped = y * rect.h;
		SDL_RenderDrawLine(renderer, 0, mapped, width, mapped);
		SDL_RenderDrawLine(renderer, 0, mapped + rect.h - 1, width, mapped + rect.h - 1);
	}

	for (int x = 0; x < grid.width; x++)
	{
		mapped = x * rect.w;
		SDL_RenderDrawLine(renderer, mapped, 0, mapped, height);
		SDL_RenderDrawLine(renderer, mapped + rect.w - 1, 0, mapped + rect.w - 1, height);
	}
}

void loop(SDL_Renderer* renderer)
{
	int width, height;
	SDL_GetRendererOutputSize(renderer, &width, &height);
	SDL_Event event;
	double timeDifference = 1.0 / FPS;
	double time = SDL_GetTicks() / 1000.0 - timeDifference;
	double newTime;
	double dt;
	bool continuer = true;
	int pressed = 0;

	Grid grid(width / 20, height / 20);
	AStar astar(grid);
	astar.findPath(0, 0, 1, 1);

	while (continuer)
	{
		newTime = SDL_GetTicks() / 1000.0;
		/* la variation de temps entre 2 frame (en ms) utile pour les systèmes physiques. */
		dt = newTime - time;

		while (SDL_PollEvent(&event))
		{
			/* code de gestion d'évenements */

			switch (event.type)
			{
				case SDL_QUIT:
					continuer = 0;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							continuer = 0;
							break;
						case SDLK_1:
							grid.clear(false);
							break;
						case SDLK_2:
							grid.clear(true);
							break;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (event.button.button == SDL_BUTTON_LEFT && pressed == 1 ||
						event.button.button == SDL_BUTTON_RIGHT && pressed == 2)
						pressed = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (!pressed && event.button.button == SDL_BUTTON_LEFT)
						pressed = 1;
					else if (!pressed && event.button.button == SDL_BUTTON_RIGHT)
						pressed = 2;

					break;
			}

			if (pressed)
			{
				int x = event.button.x * grid.width / width;
				int y = event.button.y * grid.height / height;

				if (grid.isInside(x, y))
					grid[y][x] = 2 - pressed;
			}
		}

		if (dt >= timeDifference)
		{
			std::vector<std::pair<int, int>> path = astar.findPath(0, 0, grid.width - 1, grid.height - 1);
			time = newTime;
			drawGrid(renderer, grid, path, width, height);
			SDL_RenderPresent(renderer);
		}
	}
}

int main(int argc, char** argv)
{
	#ifdef SDL_MainHandled
	SDL_SetMainReady();
	#endif

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Echec de l'initalisation de la SDL : %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	#ifdef FULL_SCREEN

	SDL_DisplayMode current;

	if (SDL_GetCurrentDisplayMode(0, &current))
	{
		SDL_Log("Could not get display mode for video display 0: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_Window* window = SDL_CreateWindow("Fenetre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, current.w, current.h, SDL_WINDOW_FULLSCREEN);

	#else

	SDL_Window* window = SDL_CreateWindow("Fenetre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, 0);

	#endif

	if (window == NULL)
	{
		fprintf(stderr, "Echec de la création de la fenêtre : %s", SDL_GetError());
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL)
	{
		fprintf(stderr, "Erreur à la création du renderer : %s", SDL_GetError());
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	std::srand((unsigned int) time(NULL));
	loop(renderer);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}