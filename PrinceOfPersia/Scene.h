#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "StaticSprite.h"
#include "Interface.h"
#include "Portal.h"
#include "Button.h"

#define NUM_ENEMIES_EASY 1
#define NUM_ENEMIES_NORMAL 2
#define NUM_ENEMIES_HARD 4

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void start(int lvl);
	void update(int deltaTime);
	void render();
	void addStaticSprite(int X, int Y, SSType sst);
	void changeSSAnimation(bool open, int X, int Y);
	bool colisionStaticSprite(int x, int y, glm::ivec2 pos, glm::ivec2 size);
	bool colisionEnemy(glm::ivec2 pos, glm::ivec2 size);
	bool isSSColisionable(int x, int y);
	bool hasSSprite(SSType type, int X, int Y);
	bool getPoti(bool left);
	bool getSword(bool left);
	bool openedDoor(bool left);
	void closePortal(int X, int Y);
	glm::vec2 openedPortal(bool left);
	Enemy* getEnemy(int i);
	void loadLevel(int level);
	void restart(int lvl);
	void write(string s); 
	void writeWin(bool win);
	void writeLose(bool lose);
	void takeOffSword();


private:
	void cheat();
	bool updatePlayer();
	void initShaders();
	void updateSceneBox();
	void updateSSprite(int deltaTime);
	void checkButton();
	void changeBoxLevel(int X, int Y);
	void setStaticSprites(int level);
	void initializeEnemies(int level);
	void pushEnemy(bool typeEnemy, int boxX, int desX, int boxY, int desY);

private:
	ShaderProgram texProgram, simpleProgram;
	glm::mat4 projection;
	glm::ivec2 BoxLevel, numRooms, mapSize;
	Texture SpriteSheet, EnemySheet, BossSheet, TileSheetF,TileSheetB;
	TileMap *mapBack,*mapFront;
	Interface *interf;
	Portal *portals;
	Button *buttons;
	Player *player;
	vector<Enemy *> enemies;
	vector<StaticSprite *> ssvector;
	int *cmap;
	float currentTime, x, y, ini_player_x, ini_player_y;
	bool first_time_ini, pause;
};


#endif // _SCENE_INCLUDE

