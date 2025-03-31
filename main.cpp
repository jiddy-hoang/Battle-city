#include <iostream>
#include <ctime>
#include <SDL.h>
#include <vector>
#include <SDL_image.h>
#include <string>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <fstream>

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
#include "AudioManager.h"
#include "SecrectBox.h"
#include "Zombie.h"

SDL_Texture* loadTexture(const char*, SDL_Renderer*);

int tileMap[19][34] = {
//	 0	1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33
	{5, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 6, 0, 0, 0},	//0
	{2, 0, 0, 0, 0, 0, 0,24,22,25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},	//1
	{2, 0, 0, 0, 0, 0, 0, 0,23,24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},	//2
	{2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,20, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},	//3
	{0, 0, 0, 0,57,55,58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,11,21, 0, 0, 0, 0, 0, 0, 0, 0},	//4
	{0, 0, 0, 0,61,49, 0, 0, 0, 0, 0, 0, 0, 0,47,39,39,39,39,39,39,38, 0,12,13,14, 0, 0, 0, 0, 0, 0, 0, 0},	//5
	{0, 0, 0, 0,61,49, 0, 0, 0, 0, 0, 0, 0, 0, 0,26,26,26,26,26,26,42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//6
	{0, 0, 0,59,61,49, 0, 0, 0, 0, 0, 0, 0, 0, 0,26,26,26,26,26,26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//7
	{0, 0, 0,51,54,50, 0, 0, 0, 0, 0, 0, 0, 0,41,26,26,26,26,26,26, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//8
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,40,26,26,26,26,26,26,41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//9
	{0, 0, 0, 0, 0, 0, 0,37,39,39,39,38, 0, 0,40,26,26,26,26,26,26,40, 0, 0, 0, 0,60,54,53, 0, 0, 0, 0, 0},	//10
	{0,15,16, 0, 0, 0,37,35,45,45,45,36,39,39,46,39,48,26,26,47,39,43, 0, 0, 0, 0,49,61,49, 0, 0, 0, 0, 0},	//11
	{0,17,18, 0, 0, 0,40,27,27,27,27,27,27,27,40,26,26,26,26,26,26,40, 0, 0, 0, 0,49,61,49, 0, 0, 0, 0, 0},	//12
	{0, 0, 0, 0, 0, 0,42,27,27,27,27,27,27,27,42,26,26,26,26,26,26,40, 0, 0, 0, 0,49,61,49, 0, 0, 0, 0, 0},	//13
	{0, 0, 0, 0, 0, 0, 0,27,27,27,27,27,27,27,26,26,26,26,26,26,26,40, 0, 0, 0, 0,56,54,50, 0, 0, 0, 0, 0},	//14
	{2, 0, 0, 0, 0, 0, 0,27,27,27,27,27,27,27,26,26,26,26,26,26,26,40, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},	//15	
	{2, 0, 0, 0, 0, 0,41,27,27,27,27,27,27,27,41,26,26,26,26,26,26,40, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},	//16
	{2, 0, 0, 0, 0, 0,36,39,39,39,39,39,39,39,44,39,39,39,39,39,39,35, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0},	//17
	{7, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 8, 0, 0, 0}	//18
};

class Game {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font; // Font render text
	SDL_Color textColor = { 0, 0, 139, 255 }; // Dark BLUE
	int lives; // when player die -))
	Uint32 gameStartTime; // time game start
	Uint32 lastBulletTime = 0; 
	int TIME_LIMIT = 120; 
	int remainingTime;
	const int Max_bullet = 6;
	int Pbullet = Max_bullet;
	string UIbullet;
	bool gameWon; 
	SDL_Rect savegame_button = { 31 * TITLE_SIZE, 16 * TITLE_SIZE, 110, 31 };
	SDL_Rect exitgame_button = { 31 * TITLE_SIZE, 17 * TITLE_SIZE, 110, 31 };
	SDL_Rect music_button = { 31 * TITLE_SIZE, 18 * TITLE_SIZE, 47, 31 };
public:
	bool running;
	vector<pair<Wall,SDL_Texture*>> walls;
	vector<pair<Wall,SDL_Texture*>> initwall;	
	vector<pair<Wall, SDL_Texture*>> woods;
	vector<SDL_Texture*> tx;
	vector<Wall> grass;
	vector<Wall> allWall;	//for all wall
	playerTank player = playerTank(TITLE_SIZE, TITLE_SIZE);
	int enemyNumber = 5;
	int current_enemy = 5;
	int zombie = 3;
	int current_zombie = 3;
	vector<EnemyTank> enemies;
	vector<Zombie> zombies;
	SDL_Texture* grasstexture = NULL;
	SDL_Texture* walltexture = NULL;
	SDL_Texture* woodtexture = NULL;
	SDL_Texture* UIplayer = NULL;
	SDL_Texture* savebutton = NULL;
	SDL_Texture* exitbutton = NULL;
	SDL_Texture* musicbutton = NULL;
	AudioManager audio;
	int number;
	SecrectBox secrectBox = { 15 * TITLE_SIZE, TITLE_SIZE, SDL_GetTicks() };
	Uint32 lastHitTime = 0;
	int lastGetItem = 0;
	bool turn_on = true;
	void saveGame(const string& filename) {	//save game
		ofstream file(filename);
		if (!file) {
			cerr << "cant open file to save!" << endl;
			return;
		}

		file << lives << " " << TIME_LIMIT << " " << remainingTime << endl;
		file << player.x << " " << player.y << endl;
		file << enemies.size() << endl;

		for (const auto& enemy : enemies) {
			file << enemy.x << " " << enemy.y << endl;
		}

		file << zombies.size() << endl;
		for (const auto& zombie : zombies) {
			file << zombie.x << " " << zombie.y << endl;
		}

		file.close();
		cout << "Save!" << endl;
	}
	void loadGame(const string& filename) {	//load game to continue the last game have save
		ifstream file(filename);
		if (!file) {
			cerr << "cant open file to load!" << endl;
			return;
		}

		file >> lives >> TIME_LIMIT >> remainingTime;
		file >> player.x >> player.y;

		int enemyCount;
		file >> enemyCount;
		enemies.clear();
		for (int i = 0; i < enemyCount; i++) {
			int x, y;
			file >> x >> y;
			enemies.push_back({ x, y });
		}

		int zombieCount;
		file >> zombieCount;
		zombies.clear();
		for (int i = 0; i < zombieCount; i++) {
			int x, y;
			file >> x >> y;
			zombies.push_back({ x, y });
		}

		file.close();
		cout << "Load!" << endl;
	}
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
		SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
	}
	string INT_TO_STRING(int num_pos) {	//change int to string to support load texture
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
		for (int i = a; i <= b; i++) {	//load texture for a to b
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
		for (int i = 13; i <= 16; i++) {	//bed
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
		for (int j = 8; j <= 10; j++) {		//sofa
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
		for (int i = 14;i <= 15;i++) {		//door
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
		for (int i = 6;i <= 7;i++) {		//door
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
		for (int i = 7;i <= 8;i++) {		//door
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
		for (int i = 14;i <= 15;i++) {		//door
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
		for (int j = 17;j <= 18;j++) {		//door
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
		for (int j = 8;j <= 10;j++) {		//kitchen
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
		Wall egg(9* TITLE_SIZE, 12 * TITLE_SIZE);	//egg
		egg.SetTexture(tx[start]);
		walls.push_back({ egg, tx[start] });
		allWall.push_back(egg);
		start++;
		for (int j = 9;j <= 11;j++) {		//table kitchen
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
		for (int j = 9;j <= 10;j++) {		//plate and knife
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
		for (int j = 9;j <= 11;j += 2) {	//chair kitchen
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
		start++;
		for (int i = 13;i >= 12; i--) {		//fish tank
			if (start > b) break;
			Wall wall(15 * TITLE_SIZE, i * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
			start++;
		}
		for (int j = 15;j <=16 ; j++) {		//flower pot
			if (start > b) break;
			Wall wall(j * TITLE_SIZE, 10 * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
		}
		start++;
		for (int j = 17;j <= 18;j++) {		//carpet
			if (start > b) break;
			Wall wall(j * TITLE_SIZE, 7 * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
		}
		start++;
		for (int i = 9;i <= 10;i++) {		//broken stuff
			for (int j = 19;j <= 20;j++) {
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
		Wall wall(20* TITLE_SIZE, 6 * TITLE_SIZE);	//flower pot
		wall.SetTexture(tx[start]);
		walls.push_back({ wall, tx[start] });
		allWall.push_back(wall);
		start++;
		for (int j = 16;j <= 18;j++) {		//sofa
			Wall wall(j* TITLE_SIZE, 6 * TITLE_SIZE);
			if (start >= tx.size() || !tx[start]) {
				std::cerr << "Error: tx[" << a << "] is invalid" << std::endl;
				continue;
			}
			wall.SetTexture(tx[start]);
			walls.push_back({ wall, tx[start] });
			allWall.push_back(wall);
			start++;
		}
		Wall rock(16 * TITLE_SIZE, 7 * TITLE_SIZE);	//rock
		rock.SetTexture(tx[start]);
		walls.push_back({ rock, tx[start] });
		allWall.push_back(rock);
		start++;
		for (int j = 18;j <= 19;j++) {		//rock
			Wall wall(j* TITLE_SIZE, 8 * TITLE_SIZE);
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
	void spawnZombies() {
		zombies.clear();
		for (int i = 0; i < zombie; i++) {
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
			zombies.push_back({ ex, ey });
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
	Game() {	//init in constructor
		running = false;
		window = nullptr;
		renderer = nullptr;
		font = nullptr;
		gameStartTime = 0; 
		gameWon = false; 
		lives = 0; 
		remainingTime = 0; 
		number = 0;
	}
	void init() {
		running = true;
		gameWon = false;
		lives = 5;
		gameStartTime = SDL_GetTicks();
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
		if (TTF_Init() == -1) {
			cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << endl;
			running = false;
		}
		font = TTF_OpenFont("C:/Users/Admin/Desktop/Jiddy/GameSDL/LTNC-prj/Battlecity/open-sans.regular.ttf", 20);
		if (!font) {
			cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << endl;
			running = false;
		}
		UIplayer = loadTexture("C:/Users/Admin/Desktop/Jiddy/GameSDL/LTNC-prj/Battlecity/spritesheet/blueman.png", renderer);
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
		generatewall(100, 143);
		spawnEnemies();
		spawnZombies();
		if (!audio.init()) {
			running = false;
		}
		audio.loadMusic("C:/Users/Admin/Desktop/Jiddy/GameSDL/LTNC-prj/Battlecity/music-sound/music.mp3");
		audio.loadSound("shoot", "C:/Users/Admin/Desktop/Jiddy/GameSDL/LTNC-prj/Battlecity/music-sound/shoot-233473.wav");
		audio.loadSound("endbullet", "C:/Users/Admin/Desktop/Jiddy/GameSDL/LTNC-prj/Battlecity/music-sound/empty-gun-shot-6209.wav");
		audio.loadSound("zombie", "C:/Users/Admin/Desktop/Jiddy/GameSDL/LTNC-prj/Battlecity/music-sound/zombie.wav");
		savebutton = loadTexture("menuGame/save.png", renderer);
		exitbutton = loadTexture("menuGame/exit.png", renderer);
		musicbutton = loadTexture("menuGame/music.png", renderer);
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

		for (auto& zombie : zombies) {
			zombie.render(renderer);
		}

		// show heart in UI
		string livesText = "Lives: " + to_string(lives);
		renderText(livesText.c_str(), 31*TITLE_SIZE, 5*TITLE_SIZE);

		// show left time in UI
		Uint32 currentTime = SDL_GetTicks();
		string timeText = "Time: " + to_string(remainingTime) + "s";
		renderText(timeText.c_str(), 31 * TITLE_SIZE, 6 * TITLE_SIZE);

		//secrect box
		if (secrectBox.isActive()) {
			secrectBox.render(renderer);
		}
		if (number != 7) {
			string first_announcement = "You have...";
			string announcement = "";
			if (number == 1) {
				announcement += "enemy";
			}
			else if (number == 3) {
				announcement += "add 30s";
			}
			else if (number == 2) {
				announcement += "heart";
			}
			else if (number == 4) {
				announcement += "NOTHING :))";
			}
			else announcement += "zombie";
			renderText(first_announcement.c_str(), 31 * TITLE_SIZE, 7 * TITLE_SIZE);
			renderText(announcement.c_str(), 31 * TITLE_SIZE, 8 * TITLE_SIZE);
		}

		//show UI
		string music = "";
		if (turn_on) music += "ON";
		else music += "OFF";
		renderText(music.c_str(), 32 * TITLE_SIZE, 18 * TITLE_SIZE);
		SDL_RenderCopy(renderer, savebutton, NULL, &savegame_button);
		SDL_RenderCopy(renderer, exitbutton, NULL, &exitgame_button);
		SDL_RenderCopy(renderer, musicbutton, NULL, &music_button);

		//	show bullet
		UIbullet = "bullet: " + to_string(Pbullet);
		renderText(UIbullet.c_str(), 31 * TITLE_SIZE, 2 * TITLE_SIZE);
		if (Pbullet == 0) {
			string time = "Loading " + to_string((currentTime - lastBulletTime) / 1000) + "s";
			renderText(time.c_str(), 31 * TITLE_SIZE, 3 * TITLE_SIZE);
		}

		if (!running) {
			if (gameWon) {
				renderText("You Win!", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2);
			}
			else {
				renderText("Game Over!", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2);
			}
		}

		SDL_Rect rect = { 31 * TITLE_SIZE, 11 * TITLE_SIZE, 122, 135 };
		SDL_RenderCopy(renderer, UIplayer, NULL, &rect);

		SDL_RenderPresent(renderer);
	}
	void update() {
		player.updateBullets();

		Uint32 currentTime = SDL_GetTicks();
		int elapsedTime = (currentTime - gameStartTime) / 1000;
		remainingTime = TIME_LIMIT - elapsedTime;
		if (remainingTime < 0) remainingTime = 0;

		if (currentTime - lastGetItem > 2000) {
			number = 7;
		}
		secrectBox.update(currentTime);
		SDL_Rect secretBoxRect = secrectBox.getRect();
		if (SDL_HasIntersection(&player.tank_rect, &secretBoxRect)) {
			secrectBox.applyEffect(current_enemy, lives, TIME_LIMIT, current_zombie, number);
			lastGetItem = currentTime;
		}

		if (current_enemy>enemyNumber) {
			enemies.push_back({ TITLE_SIZE, TITLE_SIZE });
			enemyNumber = current_enemy;
		}

		if (current_zombie>zombie) {
			zombies.push_back({ TITLE_SIZE, 17 * TITLE_SIZE });
			zombie = current_zombie;
		}

		if (remainingTime <= 0 && !enemies.empty()) {
			running = false;
			return;
		}

		for (auto& enemy : enemies) {
			enemy.move(allWall);
			enemy.updateBullets();
			if (rand() % 200 < 2) {
				enemy.shoot();
				audio.playSound("shoot");
			}

		}

		for (auto& zombie : zombies) {
			zombie.move(allWall);
		}
		if (rand() % 400 < 2) {
			audio.playSound("zombie");
		}
		for (auto& enemy : enemies) {	//wall and bullet of enemy gonna dissapear when they collide
			for (auto& bullet : enemy.bullets) {
				for (auto it = walls.begin(); it != walls.end();) {
					auto& wallPair = *it;
					auto& wall = wallPair.first;
					if (wall.active && SDL_HasIntersection(&bullet.bullet_rect, &wall.rect)) {
						int x = wall.x;
						int y = wall.y;
						wall.active = false;
						bullet.active = false;
						for (auto it2 = allWall.begin(); it2 != allWall.end();) { // Sửa ở đây
							if (it2->x == x && it2->y == y) {
								it2 = allWall.erase(it2); // Xóa khỏi allWall ngay lập tức
								break;
							}
							else {
								++it2;
							}
						}
						it = walls.erase(it);
					}
					else {
						++it;
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
			for (auto& zombie : zombies) {
				if (SDL_HasIntersection(&bullet.bullet_rect, &zombie.zom_rect)) {
					bullet.active = false;
					zombie.active = false;
				}
			}
		}

		for (auto& zombie : zombies) {
			if (SDL_HasIntersection(&zombie.zom_rect, &player.tank_rect)) {
				Uint32 currentTime = SDL_GetTicks();
				if (currentTime - lastHitTime >= 1000) {
					lives--;
					lastHitTime = currentTime;
					if (lives <= 0) {
						running = false;
						return;
					}
				}
			}
			for (auto& enemy : enemies) {
				if (SDL_HasIntersection(&zombie.zom_rect, &enemy.ETank_rect)) {
					enemy.active = false;
					break;
				}
			}
		}

		zombies.erase(std::remove_if(zombies.begin(), zombies.end(), [](Zombie& e) {return !e.active; }), zombies.end());	//delete !zombie.active
		enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](EnemyTank& e) {return !e.active; }), enemies.end()); //delete !enemy.active
		for (auto& bullet : player.bullets) {	//wall and bullet of player gonna dissapear when they collide
			for (auto it = walls.begin(); it != walls.end();) {
				auto& wallPair = *it;
				auto& wall = wallPair.first;
				if (wall.active && SDL_HasIntersection(&bullet.bullet_rect, &wall.rect)) {
					int x = wall.x;
					int y = wall.y;
					wall.active = false;
					bullet.active = false;
					for (auto it2 = allWall.begin(); it2 != allWall.end();) { 
						if (it2->x == x && it2->y == y) {
							it2 = allWall.erase(it2); 
							break;
						}
						else {
							++it2;
						}
					}
					it = walls.erase(it);
				}
				else {
					++it;
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

		if (enemies.empty()&&zombies.empty()) {	//kill all zombie and enemy => win
			gameWon = true;
			running = false;
			return;
		}

		for (auto& enemy : enemies) {	//player collide with enemy.bullet => loss a heart
			for (auto& bullet : enemy.bullets) {
				if (SDL_HasIntersection(&bullet.bullet_rect, &player.tank_rect)) {
					bullet.active = false;
					lives--;
					if (lives <= 0) {
						running = false;
						return;
					}
				}
			}
		}
		Player_bullet_update(Pbullet);	//check bullet
		walls.erase(std::remove_if(walls.begin(), walls.end(), [](const std::pair<Wall, SDL_Texture*>& wallPair) {return !wallPair.first.active;}), walls.end());	//delete !wall.active

		allWall.erase(std::remove_if(allWall.begin(), allWall.end(), [](const Wall& wall) {return !wall.active;}), allWall.end());	//delete !allwall.active
	}
	void Player_bullet_update(int& Pbullet) {
		Uint32 currentTime = SDL_GetTicks(); //get real time
		if (Pbullet == 0 && currentTime - lastBulletTime >= 6000) { // time = 6s && endBullet => refill bullet
			Pbullet = Max_bullet; 
			lastBulletTime = currentTime; // update time refill
		}
	}
	void handleEvents() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
			else if (event.type == SDL_KEYDOWN) {	//move and shoot
				switch (event.key.keysym.sym) {
					case SDLK_UP: player.move(0, -5, allWall); break;
					case SDLK_DOWN: player.move(0, 5, allWall); break;
					case SDLK_LEFT: player.move(-5, 0, allWall); break;
					case SDLK_RIGHT: player.move(5, 0, allWall); break;
					case SDLK_SPACE: 
						if (Pbullet > 0) {
							player.shoot();
							audio.playSound("shoot");
							Pbullet--;
							lastBulletTime = SDL_GetTicks();
						}
						else {
							audio.playSound("endbullet");
						}
						break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				int x = event.button.x;
				int y = event.button.y;

				if (x >= exitgame_button.x && x <= exitgame_button.x + exitgame_button.w &&	//quit game
					y >= exitgame_button.y && y <= exitgame_button.y + exitgame_button.h) {
					cout << "New Game selected!" << endl;
					running = false;
				}

				if (x >= savegame_button.x && x <= savegame_button.x + savegame_button.w &&
					y >= savegame_button.y && y <= savegame_button.y + savegame_button.h) {
					cout << "Load Game selected!" << endl;
					running = false;
					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);
					renderer = nullptr;
					window = nullptr;

					loadGame("savefile.txt");	//save and quit
					running = false;
				}

				if (x >= music_button.x && x <= music_button.x + music_button.w &&	//on off music
					y >= music_button.y && y <= music_button.y + music_button.h) {
					if (turn_on == false) {
						turn_on = true;
						audio.resumeMusic();
						audio.resumeSounds();
					}
					else {
						turn_on = false;
						audio.pauseMusic();
						audio.stopAllSounds();
					}
				}
			}
		}
	}
	void run() {
		audio.playMusic();
		init();
		while (running) {
			handleEvents();
			update();
			render();
			SDL_Delay(16);
		}
	}          
	~Game() {
		audio.close();
		SDL_DestroyTexture(UIplayer);
		SDL_DestroyTexture(woodtexture);
		SDL_DestroyTexture(walltexture);
		SDL_DestroyTexture(grasstexture);
		for (auto& tex : tx) {
			if (tex) {
				SDL_DestroyTexture(tex);
			}
		}
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_CloseFont(font);
		TTF_Quit();
		SDL_Quit();
	}
};
class Menu {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* background;
	SDL_Texture* newGameButton;
	SDL_Texture* loadGameButton;
	SDL_Texture* exitButton;
	SDL_Rect newGameRect;
	SDL_Rect loadGameRect;
	SDL_Rect exitRect;
	bool running;
public:
	Menu(int width, int height) {
		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_PNG);

		window = SDL_CreateWindow("Game Menu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		background = loadTexture("menuGame/menu.png", renderer);
		newGameButton = loadTexture("menuGame/newgame.png", renderer);
		loadGameButton = loadTexture("menuGame/loadgame.png", renderer);
		exitButton = loadTexture("menuGame/exitgame.png", renderer);

		newGameRect = { 300, 120, 140, 41 };
		loadGameRect = { 300, 150, 140, 50 };
		exitRect = { 300, 200, 140, 44 };
		running = true;
	}

	void handleEvent(SDL_Event& event, Game& game) {
		if (event.type == SDL_QUIT) {
			running = false;
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			int x = event.button.x;
			int y = event.button.y;

			if (x >= newGameRect.x && x <= newGameRect.x + newGameRect.w &&
				y >= newGameRect.y && y <= newGameRect.y + newGameRect.h) {
				cout << "New Game selected!" << endl;
				running = false;
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				renderer = nullptr;
				window = nullptr;

				game.init();
				game.run();
			}

			if (x >= loadGameRect.x && x <= loadGameRect.x + loadGameRect.w &&
				y >= loadGameRect.y && y <= loadGameRect.y + loadGameRect.h) {
				cout << "Load Game selected!" << endl;
				running = false;
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				renderer = nullptr;
				window = nullptr;

				game.loadGame("savefile.txt");
				game.run();
			}

			if (x >= exitRect.x && x <= exitRect.x + exitRect.w &&
				y >= exitRect.y && y <= exitRect.y + exitRect.h) {
				cout << "Exit selected!" << endl;
				running = false;
			}
		}
	}

	void render() {
		SDL_RenderClear(renderer);
		if (background) SDL_RenderCopy(renderer, background, NULL, NULL);
		if (newGameButton) SDL_RenderCopy(renderer, newGameButton, NULL, &newGameRect);
		if (loadGameButton) SDL_RenderCopy(renderer, loadGameButton, NULL, &loadGameRect);
		if (exitButton) SDL_RenderCopy(renderer, exitButton, NULL, &exitRect);
		SDL_RenderPresent(renderer);
	}

	~Menu() {
		SDL_DestroyTexture(background);
		SDL_DestroyTexture(newGameButton);
		SDL_DestroyTexture(loadGameButton);
		SDL_DestroyTexture(exitButton);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
	}

	void run(Game& game) {
		while (running) {
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				handleEvent(event, game);
			}
			render();
		}
	}
};
int main(int argc, char* argv[]) {
	srand(static_cast<unsigned int>(time(NULL)));
	Menu menu(800, 450);
	Game game;
	menu.run(game);
	return 0;
}