#pragma once



#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

enum EnemyAnims {
	MOVE_LEFT, MOVE_RIGHT, ATTACK_LEFT, ATTACK_RIGHT, DIE_LEFT, DIE_RIGHT,
	RETREAT_L, RETREAT_R
};

class Enemy
{
public:
	Enemy(bool boss, Texture &SpriteSheet_Enemy);
	~Enemy();

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 iniPos);
	void update(int deltaTime, glm::ivec2 playerPos, Player *, int PlayerAnimation);
	void render(glm::vec2 cuadranteTraslate);
	void free();

	bool sameRoom(glm::ivec2 playerPos);
	bool collisionable(glm::ivec2 pos, glm::ivec2 size);
	int getHealth();
	int getMaxHealth();
	void die_fighting(bool left);
	bool decreaseHealth(int amount);
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();

private:

	void setAnimationSprite();
	void PlayFX();
	void modifyPosX(float incX);
	void modifyPosY(float incY);
	void calcMoviment(glm::ivec2 playerPos, int playerAnimation, Player *p);
	void setPositionToRender();

private:
	int health;
	int maxHealth;
	int lastAttackFrame;
	bool first_time;
	bool retreating;
	int num_f_retreated;
	int lastWalkFrame;
	glm::ivec2 tileMapDispl, posEnemy;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	glm::ivec2 BoxCollision, QuadSize;
};

