#include <stdio.h>
#include <SDL.h>

int main(int argc, char** argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("Error initializing SDL:%s\n", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Error creating window:%s\n", SDL_GetError());
		return 1;
	}

	SDL_Surface* surface = SDL_GetWindowSurface(window);
	SDL_Surface* playerImage = SDL_LoadBMP("Resources/stick_man.bmp");
	int playerPitch = playerImage->pitch;

	for (int i = 0; i < playerImage->w; i++) {
		int p = i / (playerImage->w/255 + 1);
		for (int j = 0; j < surface->h; j++) {
			SDL_Rect* r = &(SDL_Rect) { i, j, 1, 1 };
			if (((Uint32*)playerImage->pixels)[j*playerPitch*playerImage->w+i] < 255255255)
				SDL_FillRect(playerImage, r, SDL_MapRGB(surface->format, p, p, p));
		}
	}
	SDL_UpdateWindowSurface(window);

	SDL_SaveBMP(playerImage, "saved.bmp");

	SDL_DestroyWindow(window);
	window = NULL;
	surface = NULL;

	SDL_Quit();
	return 0;
}