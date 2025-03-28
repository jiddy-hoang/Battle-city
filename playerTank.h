#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>
#include <algorithm>
#include <SDL_image.h>
#include "wall.h"
#include "Bullet.h"
using namespace std;

SDL_Texture* loadTexture(const char*, SDL_Renderer*);
void renderCharacter(SDL_Renderer*, SDL_Texture*, SDL_Rect*, int, int);

class playerTank {
public:
	int x, y;
	int dirX, dirY; //for tank move
	SDL_Rect tank_rect;
	vector<Bullet> bullets;
	SDL_Texture* playertexture = NULL;
	playerTank(int initX, int initY) {
		x = initX;
		y = initY; 
		tank_rect = { x,y,WPLayer, HPlayer };
		dirX = 0;
		dirY = -1; //default direction up
	}
	void move(int dx, int dy, vector<Wall>& walls) {
		int newX = x + dx;
		int newY = y + dy;
		dirX = dx;
		dirY = dy;
		SDL_Rect newRect = { newX, newY, WPLayer, HPlayer };
		for (const auto &wall : walls) {
			if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
				return;	// Prevent movement if colliding with a wall
			}
		}
		if (newX >= 0 && newX <= SCREEN_WIDTH - TITLE_SIZE * 4 + 20 && newY >= 0 && newY < SCREEN_HEIGHT - 15) { //limit moving only in the map
			x = newX;
			y = newY;
			tank_rect.x = x;
			tank_rect.y = y;
		}
	}
	void shoot() {
		//crate a bullet in the pos of tank
		if (dirX == 0 && dirY <= -1) {	//up
			bullets.push_back(Bullet(x + WPLayer/4 + 3, y - 5 , this->dirX, -3));
		}
		else if (dirX == 0 && dirY >= 1) {	//down
			bullets.push_back(Bullet(x + WPLayer / 4 + 3, y + HPlayer + 1/2, this->dirX, 3));
		}
		else if (dirX <= -1 && dirY == 0) {	//left
			bullets.push_back(Bullet(x - 5, y + HPlayer/4 +3, -3, this->dirY));
		}
		else {	//right
			bullets.push_back(Bullet(x + WPLayer + 1 / 2, y + HPlayer / 4 + 3, 3, this->dirY));
		}
	}
	void updateBullets() {
		for (auto& bullet : bullets) {
			bullet.move();
		}
		bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) {return !b.active;}), bullets.end()); //delete !active-bullet
	}
	void render(SDL_Renderer* renderer) {
		playertexture = loadTexture("C:/Users/Admin/Desktop/Jiddy/GameSDL/LTNC-prj/spritesheet/manBlue_machine.png", renderer);
		renderCharacter(renderer, playertexture, &tank_rect, dirX, dirY);
		for (auto& bullet : bullets) {
			bullet.render(renderer);
		}
	}
};
