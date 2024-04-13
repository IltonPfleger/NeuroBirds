#define SCREEN_HEIGHT 2000
#define SCREEN_WIDTH 1200
#define PIPE_HOLE_SIZE 80
#define PIPE_WIDTH 120
#define PIPE_TIP_HEIGHT 25
#define BASE_HEIGHT 250
#define GAME_SPEED 10
#define BIRD_SIZE 50
#define JUMP_SIZE 30
#define NUMBER_OF_BIRDS_PER_GENERATION 100

#include "SDL2/SDL.h"
#include "network.c"
#include "pipes.h"
#include "birds.h"
#include "background.h"

int main()
{
	srand(time(NULL));
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

	SDL_Surface *pipe_bmp = SDL_LoadBMP("./sources/pipe.bmp");
	SDL_Surface *pipe_tip_bmp = SDL_LoadBMP("./sources/pipe_tip.bmp");
	SDL_Surface *bird_bmp = SDL_LoadBMP("./sources/bird.bmp");
	SDL_Texture *pipe_texture = SDL_CreateTextureFromSurface(renderer, pipe_bmp);
	SDL_Texture *pipe_tip_texture = SDL_CreateTextureFromSurface(renderer, pipe_tip_bmp);
	SDL_Texture *bird_texture = SDL_CreateTextureFromSurface(renderer, bird_bmp);
	GAME_Background background = GAME_CreateBackground(renderer);
	GAME_Pipe *pipes = GAME_CreatePipe();
	GAME_Bird birds[NUMBER_OF_BIRDS_PER_GENERATION];
	GAME_CreateFirstBirdGeneration(birds);

	while (1)
	{
		SDL_SetRenderDrawColor(renderer, 85, 200, 200, 255);
		SDL_RenderClear(renderer);
		GAME_UpdateBackground(renderer, &background);
		GAME_UpdateBirds(renderer, bird_texture, birds, pipes);
		GAME_UpdatePipes(renderer, &pipes, pipe_texture, pipe_tip_texture);

		if (GAME_GetNumberOfBirdsAlive(birds) == 0)
		{
			GAME_CalculateFit(birds);
			memcpy(&birds, GAME_CreateNextBirdsGeneration(birds), sizeof(GAME_Bird) * NUMBER_OF_BIRDS_PER_GENERATION);
			GAME_DestroyAllPipes(&pipes);
			pipes = GAME_CreatePipe();
		};

		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				break;
			};
		};

		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	};
	SDL_Quit();
	return 0;
};