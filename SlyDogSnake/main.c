#include <stdio.h>
#include <stdint.h>
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

	SDL_Surface* screen = SDL_GetWindowSurface(window);
	SDL_Surface* pImg = SDL_LoadBMP("Resources/stick_man.bmp");
	for (int x = 0; x < pImg->w; x++) {
		for (int y = 0; y < pImg->h; y++) {
			SDL_LockSurface(pImg);
			uint32_t pixel = (uint32_t) (uint8_t) pImg->pixels + y * pImg->pitch + x * pImg->format->BytesPerPixel;
			uint8_t rgb[3];
			SDL_GetRGB(pixel, pImg->format, &rgb[0], &rgb[1], &rgb[2]);

			if (rgb[0] >= 10) {
				SDL_UnlockSurface(pImg);
				SDL_FillRect(pImg, &(SDL_Rect) { x, y, 1, 1 }, SDL_MapRGB(pImg->format, 10, 155, 155));
			}
		}
	}

	SDL_SaveBMP(pImg, "saved.bmp");

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}