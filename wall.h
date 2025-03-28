#pragma once
#ifndef TITLE_SIZE
#define TITLE_SIZE 40
#endif
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

SDL_Texture* loadTexture(const char*, SDL_Renderer*);

class Wall {
private:
	SDL_Texture* wall;
public:
	int x, y;
	SDL_Rect rect;
	bool active;
	Wall(int initX, int initY) {
		x = initX;
		y = initY;
		active = true;
		rect = { x,y,TITLE_SIZE,TITLE_SIZE };
		wall = nullptr;
	};
	void SetTexture(SDL_Texture* texture) {
		wall = texture;
	}
	void render(SDL_Renderer* renderer) {
		if (active&&wall) {
			SDL_RenderCopy(renderer, wall, NULL, &rect);
		}
	}
};
