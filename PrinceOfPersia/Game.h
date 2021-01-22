#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE
#include "Scene.h"
#include "Intro.h"
#include "SoundFX.h"

#define SCREEN_WIDTH 32*10
#define SCREEN_HEIGHT (64*3 + 8)
#define LEVEL_EASY 0
#define LEVEL_NORMAL 1
#define LEVEL_HARD 2
// Game is a singleton (a class with a single instance) that represents our whole application

#define GL_KEY_RETURN 13
#define GL_KEY_ESC 27

class Game
{
	enum StateGame {INTRO, PLAYING};

public:
	Game() {}
	
	static Game &instance()
	{
		static Game G;
		return G;
	}
	
	void startGame();
	void nextLevel();
	void endGame(bool win);
	void playerDead();
	void timeOut();

	void addStaticSprite(int X, int Y, SSType sst);
	void changeSSAnimation(bool open, int X, int Y);
	void play(FX song);
	void stopThemeSong();
	bool colisionStaticSprite(int x, int y, glm::ivec2 pos, glm::ivec2 size);
	bool colisionEnemy(glm::ivec2 pos, glm::ivec2 size);
	bool isSSColisionable(int x, int y);
	bool getPoti(bool left);
	bool getSword(bool left);
	bool openedDoor(bool left);
	glm::vec2 openedPortal(bool left);
	void closePortal(int X, int Y);
	bool hasSSprite(SSType type, int X, int Y);
	int	getEnemyAtRange(glm::vec2 pPos); //-1 if there is not an enemy in range
	Enemy* getEnemy(int i);
	int getActualLevel();

	void setLevel(int level);
	int getLevel();

	bool getWin();

	void init();
	bool update(int deltaTime);
	void render();

	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	bool getModifierKey(int key) const;
	bool getKeyPressed(int key);
	bool getSpecialKeyPressed(int key);
	bool getModifierKeyPressed(int key);

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	void modifiersPressed(int mKey);

private: 
	void needRestartScene();
	void needLoadLevelScene();
	void goToMenu();

private:
	Scene scene;                      // Scene to render
	bool keysPressed[256], specialKeysPressed[256], modifierKeysPressed[5];
	bool keys[256], specialKeys[256]; // Store key states so that 
									  // we can have access at any time
	SoundFX sound;
	int modifKeys;
	int level = LEVEL_NORMAL;
	Intro intro;
	StateGame state;
	int lvl;
	bool restart, player_Dead, loadLv, win;
	
};


#endif // _GAME_INCLUDE


