#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>
#include "Bullet.h"
#include "wall.h"
using namespace std;

SDL_Texture* loadTexture(const char*, SDL_Renderer*);
void renderCharacter(SDL_Renderer*, SDL_Texture*, SDL_Rect*, int, int);

class Zombie {
public:
	int x, y;
	int dirX, dirY;
	int moveDelay;
	SDL_Rect zom_rect;
	bool active;
	SDL_Texture* texture = NULL;
	Zombie(int initX, int initY) {
		moveDelay = 15;
		x = initX;
		y = initY;
		zom_rect = { x, y, WPLayer, HPlayer };
		dirX = 0;
		dirY = -1;
		active = true;
	}
	void move(const vector<Wall>& walls) {
		if (--moveDelay < 0) return;	//moveDelay-=1 through each frame 
		moveDelay = 200;
		int t = rand() % 30;
		if (t < 2) {	//Reduce the rate of changing direction
			int r = rand() % 4;
			if (r == 0) {	//up
				dirX = 0;
				dirY = -5;
			}
			else if (r == 1) {	//down
				dirX = 0;
				dirY = 5;
			}
			else if (r == 2) {	//left
				dirX = -5;
				dirY = 0;
			}
			else {
				dirX = 5;	//right
				dirY = 0;
			}
		}
		int newX = x + dirX;
		int newY = y + dirY;
		SDL_Rect newRect = { newX,newY,WPLayer,HPlayer };
		for (const auto& wall : walls) {
			if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) return;
		}
		if (newX >= 0 && newX <= SCREEN_WIDTH - TITLE_SIZE * 4 + 20 && newY >= 0 && newY < SCREEN_HEIGHT - 15) { //limit moving only in the map
			x = newX;
			y = newY;
			zom_rect.x = x;
			zom_rect.y = y;
		}
	}
	void render(SDL_Renderer* renderer) {
		texture = loadTexture("spritesheet/zombie.png", renderer);
		renderCharacter(renderer, texture, &zom_rect, dirX, dirY);
	}
};
