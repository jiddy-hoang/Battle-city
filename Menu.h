#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

using namespace std;

enum GameState { MENU, GAME, EXIT };

class Menu {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Color textColor = { 0, 0, 139, 255 }; // Dark BLUE
    GameState gameState = MENU;

    void renderText(const char* text, int x, int y) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
        if (!textSurface) {
            cerr << "Unable to render text surface! TTF_Error: " << TTF_GetError() << endl;
            return;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture) {
            cerr << "Unable to create texture from text! SDL_Error: " << SDL_GetError() << endl;
            SDL_FreeSurface(textSurface);
            return;
        }

        SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
        SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    void renderMenu() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        renderText("Play", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50);
        renderText("Exit", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 50);

        SDL_RenderPresent(renderer);
    }

    void handleMenuEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameState = EXIT;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX > SCREEN_WIDTH / 2 - 50 && mouseX < SCREEN_WIDTH / 2 + 50) {
                    if (mouseY > SCREEN_HEIGHT / 2 - 50 && mouseY < SCREEN_HEIGHT / 2) {
                        gameState = GAME;
                    }
                    else if (mouseY > SCREEN_HEIGHT / 2 + 50 && mouseY < SCREEN_HEIGHT / 2 + 100) {
                        gameState = EXIT;
                    }
                }
            }
        }
    }

public:
    Menu(SDL_Window* gameWindow, SDL_Renderer* gameRenderer, TTF_Font* gameFont)
        : window(gameWindow), renderer(gameRenderer), font(gameFont) {
    }

    ~Menu() {}

    GameState getGameState() const {
        return gameState;
    }

    void runMenu() {
        while (gameState == MENU) {
            handleMenuEvents();
            renderMenu();
            SDL_Delay(16);
        }
    }
};