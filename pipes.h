typedef struct
{
	int hole_start;
	int hole_middle;
	int hole_end;
	int velocity;
	int last;
	SDL_Rect top;
	SDL_Rect bottom;
} GAME_Pipe;

GAME_Pipe GAME_CreatePipe()
{
	GAME_Pipe pipe;
	int random = (int)(rand() % (SCREEN_HEIGHT - PIPE_HOLE_SIZE * 8 - BASE_HEIGHT)) + PIPE_HOLE_SIZE * 4;
	int hole_start = random - PIPE_HOLE_SIZE;
	int hole_end = random + PIPE_HOLE_SIZE;
	SDL_Rect top = {SCREEN_WIDTH, 0, PIPE_WIDTH, hole_start};
	SDL_Rect bottom = {SCREEN_WIDTH, hole_end, PIPE_WIDTH, SCREEN_HEIGHT - hole_end - BASE_HEIGHT};
	pipe.top = top;
	pipe.velocity = rand()%6 -3;
	pipe.bottom = bottom;
	pipe.hole_start = hole_start;
	pipe.hole_middle = random;
	pipe.hole_end = hole_end;
	pipe.last = 1;
	return pipe;
};

void GAME_CreateFirstPipe(GAME_Pipe *pipes)
{
	pipes[0] = GAME_CreatePipe();
};

void GAME_MovePipeToLeft(GAME_Pipe *pipe)
{
	pipe->top.x -= GAME_SPEED;
	pipe->bottom.x -= GAME_SPEED;
};

void GAME_MovePipeVertical(GAME_Pipe *pipe)
{
	if(pipe->top.h + pipe->velocity <= PIPE_HOLE_SIZE*1.5 || pipe->bottom.y + pipe->velocity >= SCREEN_HEIGHT - BASE_HEIGHT)
		pipe->velocity = -pipe->velocity;
	pipe->top.h += pipe->velocity;
	pipe->bottom.y += pipe->velocity;
	pipe->bottom.h -= pipe->velocity;
	pipe->hole_start += pipe->velocity;
	pipe->hole_end += pipe->velocity;
	pipe->hole_middle += pipe->velocity;
};

void GAME_RenderPipe(SDL_Renderer *renderer, GAME_Pipe *pipe, SDL_Texture *texture)
{
	SDL_RenderCopy(renderer, texture, NULL, &pipe->top);
	SDL_RenderCopy(renderer, texture, NULL, &pipe->bottom);
}

void GAME_RenderPipeTip(SDL_Renderer *renderer, GAME_Pipe *pipe, SDL_Texture *texture)
{
	SDL_RenderCopy(renderer, texture, NULL, &(SDL_Rect){pipe->top.x, pipe->hole_start - PIPE_TIP_HEIGHT, PIPE_WIDTH, PIPE_TIP_HEIGHT});
	SDL_RenderCopy(renderer, texture, NULL, &(SDL_Rect){pipe->top.x, pipe->hole_end, PIPE_WIDTH, PIPE_TIP_HEIGHT});
}

void GAME_UpdatePipes(SDL_Renderer *renderer, GAME_Pipe *pipes, SDL_Texture *pipe_texture, SDL_Texture *pipe_tip_texture)
{
	for (int i = 0; i < N_MAX_PIPES; i++)
	{
		if (pipes[i].last && pipes[i].top.x < SCREEN_WIDTH / 1.4)
		{
			pipes[i].last = 0;
			pipes[i + 1] = GAME_CreatePipe();
		};

		GAME_MovePipeToLeft(&pipes[i]);
		GAME_MovePipeVertical(&pipes[i]);
		GAME_RenderPipe(renderer, &pipes[i], pipe_texture);
		GAME_RenderPipeTip(renderer, &pipes[i], pipe_tip_texture);
		if (pipes[i].last)
			break;
	}

	if (pipes[0].top.x < -PIPE_WIDTH)
	{
		for (int i = 0; i < N_MAX_PIPES; i++)
		{
			if (pipes[i].last)
				break;
			pipes[i] = pipes[i + 1];
		};
	};
}

void GAME_RestartPipes(GAME_Pipe *pipes)
{
	GAME_CreateFirstPipe(pipes);
}
