typedef struct GAME_PipeNode
{
	struct GAME_PipeNode *next;
	int hole_start;
	int hole_middle;
	int hole_end;
	SDL_Rect top;
	SDL_Rect bottom;
} GAME_Pipe;

GAME_Pipe *GAME_CreatePipe()
{
	GAME_Pipe *pipe = (GAME_Pipe *)malloc(sizeof(GAME_Pipe));
	int random = (int)(rand() % (SCREEN_HEIGHT - PIPE_HOLE_SIZE * 8 - BASE_HEIGHT)) + PIPE_HOLE_SIZE*4;
	int hole_start = random - PIPE_HOLE_SIZE;
	int hole_end = random + PIPE_HOLE_SIZE;
	SDL_Rect top = {SCREEN_WIDTH, 0, PIPE_WIDTH, hole_start};
	SDL_Rect bottom = {SCREEN_WIDTH, hole_end, PIPE_WIDTH, SCREEN_HEIGHT - hole_end - BASE_HEIGHT};
	pipe->top = top;
	pipe->bottom = bottom;
	pipe->hole_start = hole_start;
	pipe->hole_middle = random;
	pipe->hole_end = hole_end;
	pipe->next = NULL;
	return pipe;
};

void GAME_MovePipeToLeft(GAME_Pipe *pipe)
{
	pipe->top.x -= GAME_SPEED;
	pipe->bottom.x -= GAME_SPEED;
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

void GAME_UpdatePipes(SDL_Renderer *renderer, GAME_Pipe **pipes, SDL_Texture *pipe_texture, SDL_Texture *pipe_tip_texture)
{
	GAME_Pipe *current = *pipes;
	while (current != NULL)
	{
		if (current->next == NULL)
		{
			if (current->top.x < SCREEN_WIDTH / 1.8)
			{
				current->next = GAME_CreatePipe();
				;
			}
		};

		if (current->top.x < -PIPE_WIDTH && pipes != NULL)
		{
			GAME_Pipe *tmp = *pipes;
			*pipes = current->next;
			free(tmp);
		}

		GAME_MovePipeToLeft(current);
		GAME_RenderPipe(renderer, current, pipe_texture);
		GAME_RenderPipeTip(renderer, current, pipe_tip_texture);
		current = current->next;
	}
}

void GAME_DestroyAllPipes(GAME_Pipe **pipes)
{
	GAME_Pipe *current = *pipes;
	while (current != NULL)
	{
		GAME_Pipe *temp = current->next;
		free(current);
		current = temp;
	}
}