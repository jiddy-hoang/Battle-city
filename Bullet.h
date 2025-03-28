#pragma once
#include <iostream>
#include <SDL.h>

using namespace std;

class Bullet {
public:
	int x, y;
	int dx, dy; //for bullet move 
	SDL_Rect bullet_rect;
	bool active;
	Bullet(int initX, int initY, int dirX, int dirY) {
		x = initX;
		y = initY;
		dx = dirX;
		dy = dirY;
		active = true;
		bullet_rect = { x , y, 5, 5 }; //square shape bullet 
	}
	void move() {
		x += dx;
		y += dy;
		bullet_rect.x = x;
		bullet_rect.y = y;
		if ((x<0 || x>SCREEN_WIDTH - TITLE_SIZE * 4 + 30) || (y<0 || y>SCREEN_HEIGHT)) { //limit only moving in the map
			active = false;
		}
	}
	void render(SDL_Renderer* renderer) {
		if (active) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //white-bullet
			SDL_RenderFillRect(renderer, &bullet_rect);
		}
	}
};