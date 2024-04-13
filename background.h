typedef struct
{
	int base_reset_controller;
	SDL_Texture *base_texture;

} GAME_Background;

GAME_Background GAME_CreateBackground(SDL_Renderer *renderer)
{
	SDL_Surface *base_bmp = SDL_LoadBMP("./sources/base.bmp");
	SDL_Texture *base_texture = SDL_CreateTextureFromSurface(renderer, base_bmp);

	GAME_Background background;
	background.base_texture = base_texture;
	background.base_reset_controller = 0;
	return background;
}

void GAME_UpdateBackground(SDL_Renderer *renderer, GAME_Background *background)
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

		SDL_RenderCopy(renderer, background->base_texture, NULL, &base_rect);
	}
};