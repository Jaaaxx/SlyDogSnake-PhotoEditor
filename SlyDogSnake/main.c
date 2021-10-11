#include <stdio.h>
#include <stdint.h>
#include <SDL.h>

Uint32 get_pixel32(SDL_Surface* surface, int x, int y) {
	//Convert the pixels to 32 bit
	Uint32* pixels = (Uint32*)surface->pixels;

	//Get the requested pixel
	return pixels[(y * surface->w) + x];
}

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
	SDL_UpdateWindowSurface(window);
	for (int x = 0; x < pImg->w; x++) {
		for (int y = 0; y < pImg->h; y++) {
			uint8_t rgb[3];
			SDL_GetRGB(get_pixel32(pImg,x,y), pImg->format, &rgb[0], &rgb[1], &rgb[2]);
			printf("%d/%dw | %d/%dh\n", x, pImg->w, y, pImg->h);
			if (rgb[2] >= 100)
				SDL_FillRect(pImg, &(SDL_Rect) { x, y, 1, 1 }, SDL_MapRGB(pImg->format, 255, 0, 0));
		}
	}
	SDL_SetWindowSize(window, pImg->w, pImg->h);
	screen = SDL_GetWindowSurface(window);
	SDL_BlitSurface(pImg, NULL, screen, &(SDL_Rect){ 0,0 });
	SDL_UpdateWindowSurface(window);
	SDL_SaveBMP(pImg, "saved.bmp");

	while (1);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}