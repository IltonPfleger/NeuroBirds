typedef struct
{
	SDL_Rect rect;
	NN_Network *nn;
	int score;
	double fit;
	int is_alive;
	int velocity;
	int gravity;
} GAME_Bird;

GAME_Bird GAME_CreateBird()
{
	GAME_Bird bird;
	bird.nn = NN_CreateNetwork(3, (int[]){1, 3, 1});
	bird.score = 0;
	bird.fit = 0;
	bird.velocity = 0;
	bird.gravity = 5;
	bird.is_alive = 1;
	bird.rect.x = (int)(rand() % (SCREEN_WIDTH / 4)) + SCREEN_WIDTH / 8;
	bird.rect.y = SCREEN_HEIGHT / 2;
	bird.rect.w = BIRD_SIZE;
	bird.rect.h = BIRD_SIZE;
	return bird;
};

void GAME_CreateFirstBirdGeneration(GAME_Bird *bird)
{
	for (int i = 0; i < NUMBER_OF_BIRDS_PER_GENERATION; i++)
		bird[i] = GAME_CreateBird();
}

void GAME_RenderBird(SDL_Renderer *renderer, SDL_Texture *texture, GAME_Bird bird)
{
	SDL_RenderCopyEx(renderer, texture, NULL, &bird.rect, bird.velocity / 1.1, NULL, SDL_FLIP_NONE);
};

void GAME_ApplyBirdGravity(GAME_Bird *bird)
{
	if (bird->rect.y < SCREEN_HEIGHT - BASE_HEIGHT - BIRD_SIZE)
	{
		bird->velocity += bird->gravity;
		if (bird->rect.y + bird->velocity > SCREEN_HEIGHT - BASE_HEIGHT - BIRD_SIZE * 2)
			bird->rect.y = SCREEN_HEIGHT - BASE_HEIGHT - BIRD_SIZE;
		else
			bird->rect.y += bird->velocity;
	}
}

void GAME_RenderBirdLines(SDL_Renderer *renderer, GAME_Bird bird, GAME_Pipe *pipe)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);
	SDL_RenderDrawLine(renderer, bird.rect.x + BIRD_SIZE / 2, bird.rect.y + BIRD_SIZE / 2, pipe->top.x, pipe->hole_start);
	SDL_RenderDrawLine(renderer, bird.rect.x + BIRD_SIZE / 2, bird.rect.y + BIRD_SIZE / 2, pipe->top.x, pipe->hole_end);
}

int GAME_CheckCollide(GAME_Bird bird, GAME_Pipe *pipe)
{
	int is_alive = 0;
	do
	{
		if (bird.rect.y < 0)
			break;
		if (bird.rect.y > SCREEN_HEIGHT - BASE_HEIGHT - BIRD_SIZE * 2)
			break;
		if (bird.rect.x + bird.rect.w > pipe->top.x && bird.rect.x < pipe->top.x + pipe->top.w)
			if (bird.rect.y < pipe->hole_start || bird.rect.y + bird.rect.h > pipe->hole_end)
				break;
		is_alive = 1;
	} while (0);
	return is_alive;
}

void GAME_BirdJump(GAME_Bird *bird)
{
	bird->velocity = -JUMP_SIZE;
};

void GAME_BirdThink(GAME_Bird *bird, GAME_Pipe *pipe)
{
	double inputs[1];
	inputs[0] = (double)(bird->rect.y - pipe->hole_middle)/(double)SCREEN_HEIGHT;
	NN_FeedNetwork(bird->nn, inputs);
	if (bird->nn->O[bird->nn->size - 1][0] > 0)
		GAME_BirdJump(bird);
};

void GAME_UpdateBirds(SDL_Renderer *renderer, SDL_Texture *texture, GAME_Bird *birds, GAME_Pipe *pipe)
{
	for (int i = 0; i < NUMBER_OF_BIRDS_PER_GENERATION; i++)
	{
		if (birds[i].is_alive)
		{
			birds[i].score += 1;
			if (birds[i].rect.x > pipe->top.x + pipe->top.w && pipe->next != NULL)
			{
				GAME_BirdThink(&birds[i], pipe->next);
				GAME_RenderBirdLines(renderer, birds[i], pipe->next);
				birds[i].is_alive = GAME_CheckCollide(birds[i], pipe->next);
			}
			else
			{
				GAME_BirdThink(&birds[i], pipe);
				GAME_RenderBirdLines(renderer, birds[i], pipe);
				birds[i].is_alive = GAME_CheckCollide(birds[i], pipe);
			};
		}
		else
		{
			birds[i].rect.x -= GAME_SPEED;
			birds[i].gravity += 1;
		};
		GAME_ApplyBirdGravity(&birds[i]);
		GAME_RenderBird(renderer, texture, birds[i]);
	};
};

int GAME_GetNumberOfBirdsAlive(GAME_Bird *birds)
{
	int sum = 0;
	for (int i = 0; i < NUMBER_OF_BIRDS_PER_GENERATION; i++)
		sum += birds[i].is_alive == 1 ? 1 : 0;
	return sum;
}

void GAME_CalculateFit(GAME_Bird *birds)
{
	double sum = 0;
	for (int i = 0; i < NUMBER_OF_BIRDS_PER_GENERATION; i++)
	{
		sum += birds[i].score;
	}
	for (int i = 0; i < NUMBER_OF_BIRDS_PER_GENERATION; i++)
	{
		birds[i].fit = (double)birds[i].score / (double)sum;
	}
}

GAME_Bird GAME_PickOneBirdBasedOnFitness(GAME_Bird *birds)
{
	double random = (double)rand() / (double)(RAND_MAX);
	int i = 0;
	while (random > 0)
	{
		random -= birds[i].fit;
		i++;
	}
	GAME_Bird pick = GAME_CreateBird();
	NN_CopyNetwork(*birds[i - 1].nn, pick.nn);
	return pick;
}

GAME_Bird GAME_MutateBird(GAME_Bird bird)
{
	for (int i = 0; i < bird.nn->size - 1; i++)
	{
		for (int j = 0; j < bird.nn->sizes[i + 1]; j++)
		{
			for (int k = 0; k < bird.nn->sizes[i]; k++)
			{
				if ((double)rand() / (double)(RAND_MAX) > 0.85)
					bird.nn->W[i][j][k] += (double)(rand() / (RAND_MAX) - 0.5)/10;
				if ((double)rand() / (RAND_MAX) > 0.85)
					bird.nn->B[i][j] += (double)(rand() / (RAND_MAX) - 0.5)/10;
			}
		}
	}
	return bird;
}

void GAME_DestroyBird(GAME_Bird *bird)
{
	NN_DestroyNetwork(bird->nn);
}

GAME_Bird GAME_CrossOverBirds(GAME_Bird bird1, GAME_Bird bird2)
{
	for (int i = 0; i < bird1.nn->size - 1; i++)
	{
		for (int j = 0; j < bird1.nn->sizes[i + 1]; j++)
		{
			for (int k = 0; k < bird1.nn->sizes[i]; k++)
			{
				if ((double)rand() /  (double)(RAND_MAX) > 0.5)
				{
					bird1.nn->W[i][j][k] = bird2.nn->W[i][j][k];
				}
			}
		}
	}
	GAME_DestroyBird(&bird2);
	return bird1;
}

GAME_Bird *GAME_CreateNextBirdsGeneration(GAME_Bird *birds)
{
	GAME_Bird new_generation[NUMBER_OF_BIRDS_PER_GENERATION];

	for (int i = 0; i < NUMBER_OF_BIRDS_PER_GENERATION; i += 2)
	{
		GAME_Bird mutate = GAME_MutateBird(GAME_PickOneBirdBasedOnFitness(birds));
		GAME_Bird cross = GAME_CrossOverBirds(GAME_PickOneBirdBasedOnFitness(birds), GAME_PickOneBirdBasedOnFitness(birds));
		new_generation[i] = mutate;
		new_generation[i + 1] = cross;
	}

	for (int i = 0; i < NUMBER_OF_BIRDS_PER_GENERATION; i++)
		GAME_DestroyBird(&birds[i]);

	return new_generation;
}