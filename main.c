#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1000
#define PIPE_HOLE_SIZE 85
#define PIPE_WIDTH 60
#define PIPE_TIP_HEIGHT 12
#define BASE_HEIGHT 100
#define GAME_SPEED 8
#define BIRD_SIZE 30
#define JUMP_SIZE 28
#define GRAVITY 5
#define N_MAX_BIRDS 200
#define N_MAX_PIPES 64

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

	SDL_Surface *pipe_s = SDL_LoadBMP("./sources/pipe.bmp");
	SDL_Surface *pipe_tip_s = SDL_LoadBMP("./sources/pipe_tip.bmp");
	SDL_Surface *bird_s = SDL_LoadBMP("./sources/bird.bmp");
	SDL_Surface *base_s = SDL_LoadBMP("./sources/base.bmp");
	SDL_Texture *base_texture = SDL_CreateTextureFromSurface(renderer, base_s);
	SDL_Texture *pipe_texture = SDL_CreateTextureFromSurface(renderer, pipe_s);
	SDL_Texture *pipe_tip_texture = SDL_CreateTextureFromSurface(renderer, pipe_tip_s);
	SDL_Texture *bird_texture = SDL_CreateTextureFromSurface(renderer, bird_s);
	
	
	GAME_Background background = GAME_CreateBackground();
	GAME_Pipe pipes[N_MAX_PIPES];
	GAME_Bird birds[N_MAX_BIRDS];
	GAME_CreateFirstPipe(pipes);
	GAME_CreateFirstBirdGeneration(birds);

	while (1)
	{
		SDL_SetRenderDrawColor(renderer, 85, 200, 200, 255);
		SDL_RenderClear(renderer);
		
		GAME_UpdateBirds(renderer, bird_texture, birds, pipes);
		GAME_UpdatePipes(renderer, pipes, pipe_texture, pipe_tip_texture);
		GAME_UpdateBackground(renderer, &background, base_texture);

		if (GAME_GetNumberOfBirdsAlive(birds) == 0)
		{
			GAME_CalculateFit(birds);
			GAME_CreateNextBirdsGeneration(birds);
			GAME_RestartPipes(pipes);
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
		SDL_Delay(20);
	};
	
	SDL_Quit();
	return 0;
};
