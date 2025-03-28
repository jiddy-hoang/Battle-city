#include <iostream>
#include <ctime>
#include <SDL.h>
#include <vector>
#include <SDL_image.h>

using namespace std;

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 760;
const int TITLE_SIZE = 40;
const int MAP_WIDTH = SCREEN_WIDTH / TITLE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TITLE_SIZE;
const int WPLayer = 23;
const int HPlayer = 20;

#include "wall.h"
#include "playerTank.h"
#include "bullet.h"
#include "EnemyTank.h"

SDL_Texture* loadTexture(const char*, SDL_Renderer*);

int tileMap[19][34] = {
	{5, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 6, 0, 0, 0},
	{2, 0, 0, 0, 0, 0, 0,24,22,25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},
	{2, 0, 0, 0, 0, 0, 0, 0,23,24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},
	{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,20, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},
	{0, 0, 0, 0,57,55,58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,11,21, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0,61,49, 0, 0, 0, 0, 0, 0, 0, 0,47,39,39,39,39,39,39,38, 0,12,13,14, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0,61,49, 0, 0, 0, 0, 0, 0, 0, 0, 0,26,26,26,26,26,26,42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0,59,61,49, 0, 0, 0, 0, 0, 0, 0, 0, 0,26,26,26,26,26,26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0,51,54,50, 0, 0, 0, 0, 0, 0, 0, 0,41,26,26,26,26,26,26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,40,26,26,26,26,26,26,41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0,37,39,39,39,38, 0, 0,40,26,26,26,26,26,26,40, 0, 0, 0, 0,60,54,53, 0, 0, 0, 0, 0},
	{0,15,16, 0, 0, 0,37,35,45,45,45,36,39,39,46,39,48,26,26,47,39,43, 0, 0, 0, 0,49,61,49, 0, 0, 0, 0, 0},
	{0,17,18, 0, 0, 0,40,27,27,27,27,27,27,27,40,26,26,26,26,26,26,40, 0, 0, 0, 0,49,61,49, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0,42,27,27,27,27,27,27,27,42,26,26,26,26,26,26,40, 0, 0, 0, 0,49,61,49, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0,27,27,27,27,27,27,27,26,26,26,26,26,26,26,40, 0, 0, 0, 0,56,54,50, 0, 0, 0, 0, 0},
	{2, 0, 0, 0, 0, 0, 0,27,27,27,27,27,27,27,26,26,26,26,26,26,26,40, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},
	{2, 0, 0, 0, 0, 0,41,27,27,27,27,27,27,27,41,26,26,26,26,26,26,40, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},
	{2, 0, 0, 0, 0, 0,36,39,39,39,39,39,39,39,44,39,39,39,39,39,39,35, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},
	{7, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 8, 0, 0, 0}
};

class Game {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
public:
	bool running;
	vector<pair<Wall,SDL_Texture*>> walls;
	vector<pair<Wall,SDL_Texture*>> initwall;	
	vector<pair<Wall, SDL_Texture*>> woods;
	vector<SDL_Texture*> tx;
	vector<Wall> grass;
	vector<Wall> allWall;	//for all wall
	playerTank player = playerTank(TITLE_SIZE, TITLE_SIZE);
	int enemyNumber = 4;
	vector<EnemyTank> enemies;
	SDL_Texture* grasstexture = NULL;
	SDL_Texture* walltexture = NULL;
	SDL_Texture* woodtexture = NULL;
	string INT_TO_STRING(int num_pos) {
		string num = "";
		int x;
		char c;
		while (num_pos) {
			x = num_pos % 10;
			c = (char)(x + '0');
			num = c + num;
			num_pos /= 10;
		}
		return num;
	}
	void generatewall(int a, int b) {
		string path;
		if (tx.size() < b + 1) tx.resize(b + 1);
		for (int i = a; i <= b; i++) {
			path = "spritesheet/tile_";
			string num = INT_TO_STRING(i);
			path += num + ".PNG";
			walltexture = loadTexture(path.c_str(), renderer);
			if (!walltexture) {
				std::cerr << "Error loading texture for tile_" << i << std::endl;
				continue;
			}
			tx[i] = walltexture;
		}
		if (a > b) {
			std::cerr << "Error: a exceeded b in generatewall" << std::endl;
			return;
		}
		int start = a;
		for (int i = 13; i <= 16; i++) {
			for (int j = 18; j <= 20; j++) {
				if (start > b) break;
				Wall wall(j * TITLE_SIZE, i * TITLE_SIZE);
				if (start >= tx.size() || !tx[start]) {
					std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
					continue;
				}
				wall.SetTexture(tx[start]);
				walls.push_back({ wall, tx[start] });
				allWall.push_back(wall);
				start++;
			}
		}
		for (int j = 8; j <= 10; j++) {
			if (start > b) break;
			Wall wall(j * TITLE_SIZE, 16 * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
			start++;
		}
		for (int i = 14;i <= 15;i++) {
			if (start > b) break;
			Wall wall(6 * TITLE_SIZE, i * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
		}
		for (int i = 6;i <= 7;i++) {
			if (start > b) break;
			Wall wall(14 * TITLE_SIZE, i * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
		}
		start++;
		for (int i = 7;i <= 8;i++) {
			if (start > b) break;
			Wall wall(21 * TITLE_SIZE, i * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
		}
		start++;
		for (int i = 14;i <= 15;i++) {
			if (start > b) break;
			Wall wall(14 * TITLE_SIZE, i * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
		}
		start++;
		for (int j = 17;j <= 18;j++) {
			if (start > b) break;
			Wall wall(j * TITLE_SIZE, 11 * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
		}
		start++;
		for (int j = 8;j <= 10;j++) {
			if (start > b) break;
			Wall wall(j * TITLE_SIZE, 11 * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
			start++;
		}
		Wall egg(9* TITLE_SIZE, 12 * TITLE_SIZE);
		egg.SetTexture(tx[start]);
		walls.push_back({ egg, tx[start] });
		allWall.push_back(egg);
		start++;
		for (int j = 9;j <= 11;j++) {
			if (start > b) break;
			Wall wall(j * TITLE_SIZE, 14 * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
			start++;
		}
		for (int j = 9;j <= 10;j++) {
			if (start > b) break;
			Wall wall(j * TITLE_SIZE, 14 * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
			start++;
		}
		for (int j = 9;j <= 11;j += 2) {
			if (start > b) break;
			Wall wall(j * TITLE_SIZE, 13 * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
		}
	}
	void generatewood(int num_pos) {
		for (int i = 0;i < 19; i++) {	//for Oy
			for (int j = 0;j < 34; j++) {	//for Ox
				string num = INT_TO_STRING(num_pos);
				string path = "spritesheet/tile_" + num + ".PNG";
				woodtexture = loadTexture(path.c_str(), renderer);
				if (tileMap[i][j] == num_pos) {
					Wall newwood(j * TITLE_SIZE, i * TITLE_SIZE);
					newwood.SetTexture(woodtexture);
					woods.push_back({ newwood, walltexture });
				}
			}
		}
		woodtexture = loadTexture("spritesheet/tile_26.png", renderer);
		Wall newwood2(14 * TITLE_SIZE, 6 * TITLE_SIZE);
		newwood2.SetTexture(woodtexture);
		woods.push_back({ newwood2, woodtexture });
		Wall newwood3(14 * TITLE_SIZE, 7 * TITLE_SIZE);
		newwood3.SetTexture(woodtexture);
		woods.push_back({ newwood3, woodtexture });
		woodtexture = loadTexture("spritesheet/tile_27.png", renderer);
		Wall newwood4(6 * TITLE_SIZE, 14 * TITLE_SIZE);
		newwood4.SetTexture(woodtexture);
		woods.push_back({ newwood4, woodtexture });
		Wall newwood5(6 * TITLE_SIZE, 15 * TITLE_SIZE);
		newwood5.SetTexture(woodtexture);
		woods.push_back({ newwood5, woodtexture });
	}
	void generategrass() {
		for (int i = 0;i < SCREEN_WIDTH - 4*TITLE_SIZE;i += TITLE_SIZE) {
			for (int j = 0;j < SCREEN_HEIGHT;j+=TITLE_SIZE) {
				Wall grassBlock(i, j);
				grassBlock.SetTexture(grasstexture);
				grass.push_back(grassBlock);
			}
		}
	}	
	void initwalls(int num_pos) {
		for (int i = 0;i < 19; i++) {	//for Oy
			for (int j = 0;j < 34; j++) {	//for Ox
				string num = INT_TO_STRING(num_pos);
				string path = "spritesheet/tile_"+ num +".PNG";
				walltexture = loadTexture(path.c_str(), renderer);
				if (tileMap[i][j] == num_pos) {
					Wall newWall(j * TITLE_SIZE, i * TITLE_SIZE);
					newWall.SetTexture(walltexture);  
					initwall.push_back({ newWall, walltexture });
					allWall.push_back(newWall);
				}
			}
		}
	}
	void spawnEnemies() {
		enemies.clear();
		for (int i = 0; i < enemyNumber; i++) {
			int ex, ey;
			bool validPos = false;
			while (!validPos) {
				ex = (rand() % 30) * TITLE_SIZE;  
				ey = (rand() % 19) * TITLE_SIZE;  
				if (ex < 40) ex = 40;
				if (ey < 40) ey = 40;
				validPos = true;
				for (const auto& wall : allWall) {
					if (wall.active && ex == wall.x && ey == wall.y) {
						validPos = false;
						break;
					}
				}
			}
			enemies.push_back({ ex, ey });
		}
	}
	Game() {
		running = true;
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			cerr << "SDL could not initalize! SDL_ERROR: " << SDL_GetError() << endl;
			running = false;
		}
		window = SDL_CreateWindow("Battlecity", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (!window) {
			cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
			running = false;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (!renderer) {
			cerr << "Renderer could not be created! SDL_Error " << SDL_GetError() << endl;
			running = false;
		}
		grasstexture = loadTexture("spritesheet/tile_01.png", renderer);
		if (!grasstexture) {
			cerr << "Failed to load grass texture Error: " << endl;
			running = false;
		}
		for (int i = 2;i <= 25;i++) {
			initwalls(i);
		}
		for (int i = 26;i <= 27;i++) {
			generatewood(i);
		}
		for (int i = 34;i <= 61;i++) {
			initwalls(i);
		}
		generategrass();
		generatewall(100, 130);
		spawnEnemies();
	}
	void render() {
		SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); //set default backgroung: gray
		SDL_RenderClear(renderer);	
		for (int i = 0;i < grass.size();i++) {
			grass[i].render(renderer); 
		}
		for (const auto &wall : initwall) {
			Wall iwall = wall.first;
			iwall.render(renderer);
		}
		for (const auto& wood : woods) {
			Wall iwood = wood.first;
			iwood.render(renderer);
		}
		for (const auto& wall : walls) {
			Wall iwall = wall.first;
			iwall.render(renderer);
		}
		player.render(renderer);
		for (auto& enemy : enemies) {
			enemy.render(renderer);
		}
		SDL_RenderPresent(renderer);
	}
	void update() {
		player.updateBullets();
		for (auto& enemy : enemies) {
			enemy.move(allWall);
			enemy.updateBullets();
			/*if (rand() % 200 < 2) {
				enemy.shoot();
			}*/

		}
		for (auto &enemy : enemies) {
			for (auto& bullet : enemy.bullets) {
				for (auto& wallPair : walls) {
					auto& wall = wallPair.first;
					if (wall.active && SDL_HasIntersection(&wall.rect, &bullet.bullet_rect)) {	//bullet and wall gonna disappear when they collide
						wall.active = false;
						bullet.active = false;
						int x = wall.x;
						int y = wall.y;
						for (auto& allWall : allWall) {
							if (allWall.x == x && allWall.y == y) {
								allWall.active = false;
								break;  // Thoát vòng lặp sau khi tìm thấy và cập nhật
							}
						}
						break;
					}
				}
			}
		}
		for (auto& enemy : enemies) {
			for (auto& bullet : enemy.bullets) {
				for (auto& wallPair : initwall) {
					auto& wall = wallPair.first;
					if (wall.active && SDL_HasIntersection(&wall.rect, &bullet.bullet_rect)) {	
						bullet.active = false;
						break;
					}
				}
			}
		}
		for (auto& bullet : player.bullets) {	//player.bullet collide with enemy => enemy die
			for (auto& enemy : enemies) {
				if (SDL_HasIntersection(&bullet.bullet_rect, &enemy.ETank_rect)) {
					bullet.active = false;
					enemy.active = false;
				}
			}
		}
		enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](EnemyTank& e) {return !e.active; }), enemies.end()); //delete !enemy.active
		for (auto& bullet : player.bullets) {
			for (auto& wallPair : walls) {
				auto& wall = wallPair.first;
				if (wall.active && SDL_HasIntersection(&bullet.bullet_rect, &wall.rect)) {
					int x = wall.x;
					int y = wall.y;
					wall.active = false;
					bullet.active = false;
					for (auto& allWall : allWall) {
						if (allWall.x == x && allWall.y == y) {
							allWall.active = false;
							break; 
						}
					}
					break;
				}
			}
		}
		for (auto& bullet : player.bullets) {
			for (auto& wallPair : initwall) {
				auto& wall = wallPair.first;
				if (wall.active && SDL_HasIntersection(&bullet.bullet_rect, &wall.rect)) {
					bullet.active = false;
					break;
				}
			}
		}
		if (enemies.empty()) running = false; 
		for (auto& enemy : enemies) {	//player collide with enemy.bullet => die
			for (auto& bullet : enemy.bullets) {
				if (SDL_HasIntersection(&bullet.bullet_rect, &player.tank_rect)) {
					running = false;
					return;
				}
			}
		}
	}
	void handleEvents() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
			else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_UP: player.move(0, -5, allWall); break;
					case SDLK_DOWN: player.move(0, 5, allWall); break;
					case SDLK_LEFT: player.move(-5, 0, allWall); break;
					case SDLK_RIGHT: player.move(5, 0, allWall); break;
					case SDLK_SPACE: player.shoot(); break;
				}
			}
		}
	}
	void run() {
		while (running) {
			handleEvents();
			update();
			render();
			SDL_Delay(16);
		}
	}          
	~Game() {
		SDL_DestroyTexture(woodtexture);
		SDL_DestroyTexture(walltexture);
		SDL_DestroyTexture(grasstexture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
};
int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(NULL)));
	Game game;
	if (game.running) {
		game.run();
	}
	return 0;
}