#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer) {
	SDL_Surface* surface = IMG_Load(path);
	if (!surface) {
		cerr << "Failed to load surface Error: " << IMG_GetError() << endl;
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);	//surface = NULL
	return texture;
}

void renderCharacter(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* destRect, int dirX, int dirY) {
    double angle = 0.0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;

	if (dirX == 0 && dirY <= -1) {        // up
		angle = 0;
	}
	else if (dirX == 0 && dirY >= 1) {  // down
		angle = 180;
	}
	else if (dirX <= -1 && dirY == 0) { // left
		angle = -90;
	}
	else {	//right
		angle = 90;
	}

    SDL_RenderCopyEx(renderer, texture, NULL, destRect, angle, NULL, flip);
}
