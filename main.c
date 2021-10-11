#include <stdio.h>
#include <stdint.h>
#include <SDL.h>
#include <stdbool.h>

uint32_t getPixel(SDL_Surface *surface, int x, int y){
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    uint8_t* p = (uint8_t*) surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1: return *p;
        case 2: return *(uint16_t*) p;
        case 3: return p[0] | p[1] << 8 | p[2] << 16;
        case 4: return *(uint32_t*) p;
        default: return 0;
    }
}

int* getBackground(SDL_Surface* s, int* size) {
    int* bp = malloc(8 * s->w * s->h * 2);
    int c = 0;
    uint8_t rF, gF, bF;
    SDL_GetRGB(getPixel(s, 0, 0), s->format, &rF, &gF, &bF);

    for (int x = 0; x < s->w; x++) {
        for (int y = 0; y < s->h; y++) {
            uint8_t r, g, b;
            SDL_GetRGB(getPixel(s, x, y), s->format, &r, &g, &b);
            if (r == rF && g == gF && b == bF) {
                bp[c++] = x;
                bp[c++] = y;
            }
        }
    }
    *size = c;
    return bp;
}

void fillArea(SDL_Surface* surface, uint32_t pixel, uint32_t color) {
    
}

int main(int argc, char** argv) {
    bool mouseDown = false;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("Error initializing SDL:%s\n", SDL_GetError());
		return 1;
	}
    SDL_Window* window = SDL_CreateWindow("Photo Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
//    SDL_Surface* pImg = SDL_LoadBMP("/Users/jacksonhutton/Desktop/SlyDogSnake-PhotoEditor/Resources/stick_man.bmp");
    SDL_Surface* pImg = SDL_CreateRGBSurface(0, 500, 500, 32, 0, 0, 0, 0);
    SDL_FillRect(pImg, 0, SDL_MapRGB(pImg->format, 255, 255, 255));
    if (window == NULL) {
		printf("Error creatiang window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_SetWindowSize(window, pImg->w + 70, pImg->h);
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    uint32_t penRgb = SDL_MapRGB(pImg->format, 0, 0, 0);

    // Add Buttons
    SDL_FillRect(screen, &(SDL_Rect){screen->w - 65, 5, 60, 60}, SDL_MapRGB(screen->format, 200, 200, 200));
    SDL_FillRect(screen, &(SDL_Rect){screen->w - 60, 10, 50, 50}, SDL_MapRGB(screen->format, 255, 0, 0));
    SDL_FillRect(screen, &(SDL_Rect){screen->w - 65, 75, 60, 60}, SDL_MapRGB(screen->format, 200, 200, 200));
    SDL_FillRect(screen, &(SDL_Rect){screen->w - 60, 80, 50, 50}, SDL_MapRGB(screen->format, 0, 255, 0));
    SDL_FillRect(screen, &(SDL_Rect){screen->w - 65, 145, 60, 60}, SDL_MapRGB(screen->format, 200, 200, 200));
    SDL_FillRect(screen, &(SDL_Rect){screen->w - 60, 150, 50, 50}, SDL_MapRGB(screen->format, 0, 0, 255));

    SDL_BlitSurface(pImg, NULL, SDL_GetWindowSurface(window), NULL);
	SDL_UpdateWindowSurface(window);
    bool quit = false;
	while (!quit) {
        SDL_Event e;
        while (!quit) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    mouseDown = true;
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    if (x > screen->w - 70) {
                        int s = 0;
                        int* bp = getBackground(pImg, &s);
                        if (y < 70)
                            for (int i = 0; i < s-1; i += 2)
                                penRgb = SDL_MapRGB(pImg->format, 255, 0, 0);
                        else if (y < 140)
                            for (int i = 0; i < s-1; i+= 2)
                                penRgb = SDL_MapRGB(pImg->format, 0, 255, 0);
                        else if (y < 210)
                            for (int i = 0; i < s-1; i+= 2)
                                penRgb = SDL_MapRGB(pImg->format, 0, 0, 255);
                        mouseDown = false;
                    }
                }
                if (e.type == SDL_MOUSEBUTTONUP) {
                    mouseDown = false;
                }
            }

            if (mouseDown) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x < screen->w - 70)
                    SDL_FillRect(pImg, &(SDL_Rect) { x, y, 4, 4 }, penRgb);
            }
            SDL_BlitSurface(pImg, NULL, screen, NULL);
            SDL_UpdateWindowSurface(window);
        }
	}

    SDL_SaveBMP(pImg, "/Users/jacksonhutton/Desktop/SlyDogSnake-PhotoEditor/saved.bmp");
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
