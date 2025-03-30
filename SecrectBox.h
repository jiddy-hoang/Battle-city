#pragma once
#include <SDL.h>
#include <cstdlib>  // rand(), srand()
#include <ctime>    // time()
#include <vector>
#include "EnemyTank.h"

using namespace std;

enum ItemType { EXTRA_ENEMY, ADD_HEART, ADD_TIME, NOTHING };
SDL_Texture* loadTexture(const char*, SDL_Renderer*);

class SecrectBox {
private:
    ItemType item;
    bool active;
    Uint32 lastSpawnTime;
    int x, y;
    SDL_Texture* texture;
    SDL_Rect boxRect;
public:
    SecrectBox(int initX, int initY, Uint32 gameStartTime) {
        x = initX;
        y = initY;
        boxRect = { x, y, TITLE_SIZE, TITLE_SIZE };
        active = false;
        lastSpawnTime = gameStartTime;
        item = NOTHING; 
        texture = NULL;
    }

    void update(Uint32 currentTime) {
        Uint32 elapsedTime = (currentTime - lastSpawnTime) / 1000;

        if (!active && elapsedTime >= 20) {  // appears every 40s
            spawn();
            lastSpawnTime = currentTime;
        }
        else if (active && elapsedTime >= 10) { // within 10s
            active = false;
        }
    }

    void spawn() {
        active = true;
        item = static_cast<ItemType>(rand() % 4); // random 1 of 4 items
    }

    void applyEffect(int& Enemy , int& playerLives, int& remainingTime) {
        if (!active) return;
        switch (item) {
        case EXTRA_ENEMY:
            Enemy++;
            break;
        case ADD_HEART:
            playerLives++;
            break;
        case ADD_TIME:
            remainingTime += 30;
            break;
        case NOTHING:
            break;
        }
        active = false; // after using, box disappear
    }

    void render(SDL_Renderer* renderer) {
        texture = loadTexture("spritesheet/tile_143.png", renderer);
        SDL_RenderCopy(renderer, texture, NULL , &boxRect);
    }

    bool isActive() const { return active; }
    SDL_Rect getRect() const { return boxRect; }
    ItemType getItem() const { return item; }
};
