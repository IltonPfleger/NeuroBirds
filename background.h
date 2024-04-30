typedef struct
{
	int base_reset_controller;
} GAME_Background;

GAME_Background GAME_CreateBackground()
{
	GAME_Background background;
	background.base_reset_controller = 0;
	return background;
}

void GAME_UpdateBackground(SDL_Renderer *renderer, GAME_Background * background, SDL_Texture * base_t)
{
	background->base_reset_controller += GAME_SPEED;

	if (background->base_reset_controller > SCREEN_WIDTH)
	{
		background->base_reset_controller = 0;
	};

	for (int i = 0; i < 2; i++)
	{
		SDL_Rect base_rect;
		base_rect.x = i * SCREEN_WIDTH - background->base_reset_controller;
		base_rect.y = SCREEN_HEIGHT - BASE_HEIGHT;
		base_rect.w = SCREEN_WIDTH;
		base_rect.h = BASE_HEIGHT;
		SDL_RenderCopy(renderer, base_t, NULL, &base_rect);
	}
};