#pragma once
#include <iostream>
#include <SDL.h>
#include <vector>
#include "Bullet.h"
#include "wall.h"
using namespace std;

SDL_Texture* loadTexture(const char*, SDL_Renderer*);
void renderCharacter(SDL_Renderer*, SDL_Texture*, SDL_Rect*, int, int);

class EnemyTank {
public:
	int x, y;
	int dirX, dirY;
	int moveDelay, shootDelay;
	SDL_Rect ETank_rect;
	bool active;
	vector<Bullet> bullets;
	SDL_Texture* texture = NULL;
	EnemyTank(int initX, int initY) {
		moveDelay = 15;
		shootDelay = 5;
		x = initX;
		y = initY;
		ETank_rect = {x, y, WPLayer, HPlayer};
		dirX = 0;
		dirY = -1;
		active = true;
	}
	void move(const vector<Wall> &walls) {
		if (--moveDelay < 0) return;	//moveDelay-=1 through each frame 
		moveDelay = 200;
		int t = rand() % 30;	
		if (t < 2) {	//Reduce the rate of changing direction
			int r = rand() % 4;
			if (r == 0) {	//up
				dirX = 0;
				dirY = -3;
			}
			else if (r == 1) {	//down
				dirX = 0;
				dirY = 3;
			}
			else if (r == 2) {	//left
				dirX = -3;
				dirY = 0;
			}
			else {
				dirX = 3;	//right
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
			ETank_rect.x = x;
			ETank_rect.y = y;
		}
	}
	void shoot() {
		if (--shootDelay < 0) return;	//samelike moveDelay
		shootDelay = 50;
		if (dirX == 0 && dirY <= -1) {	//up
			bullets.push_back(Bullet(x + WPLayer / 4 + 3, y - 5, this->dirX, -3));	//this->variable (point to a class member)
		}
		else if (dirX == 0 && dirY >= 1) {	//down
			bullets.push_back(Bullet(x + WPLayer / 4 + 3, y + HPlayer + 1 / 2, this->dirX, 3));
		}
		else if (dirX <= -1 && dirY == 0) {	//left
			bullets.push_back(Bullet(x - 5, y + HPlayer / 4 + 3, -3, this->dirY));
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
		texture = loadTexture("C:/Users/Admin/Desktop/Jiddy/GameSDL/LTNC-prj/spritesheet/manBrown_machine.png", renderer);
		renderCharacter(renderer, texture, &ETank_rect, dirX, dirY);
		for (auto& bullet : bullets) {
			bullet.render(renderer);
		}
	}
};