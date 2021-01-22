#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum PlayerAnims
{
	BENDING_R, BENDED_R, BEND_STEP_R, WAKE_R,
	STAND_R, START_R, MOVE_R, STOP_R,
	SMALL_JUMP_INI_R, SMALL_JUMP_JUMP_R, SMALL_JUMP_LAND_R,
	BIG_JUMP_R, SLOW_SPIN_R, FAST_SPIN_R,
	ENFUNDAR_R, DESENFUNDAR_R, ATACK_R, DEFEND_R, SW_PAUSE_R,
	STEP_R, DRINK_R, CLIMB_R, DOOR_R,
	DIE_F_R, DIE_P_R, DIE_C_R, FALL_R,
	VERTICAL_JUMP_UP_S_R, VERTICAL_JUMP_UP_J_R,
	VERTICAL_JUMP_DW_R, WALK_SWORD_R, RETREAT_SWORD_R,
	DEAD_F_R,

	BENDING_L, BENDED_L, BEND_STEP_L, WAKE_L,
	STAND_L, START_L, MOVE_L, STOP_L,
	SMALL_JUMP_INI_L, SMALL_JUMP_JUMP_L, SMALL_JUMP_LAND_L,
	BIG_JUMP_L, SLOW_SPIN_L, FAST_SPIN_L,
	ENFUNDAR_L, DESENFUNDAR_L, ATACK_L, DEFEND_L, SW_PAUSE_L,
	STEP_L, DRINK_L, CLIMB_L, DOOR_L,
	DIE_F_L, DIE_P_L, DIE_C_L, FALL_L,
	VERTICAL_JUMP_UP_S_L, VERTICAL_JUMP_UP_J_L,
	VERTICAL_JUMP_DW_L, WALK_SWORD_L, RETREAT_SWORD_L,
	DEAD_F_L
};

class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render(glm::vec2 cuadranteTraslate);
	void restart();
	
	bool isDead();
	bool inCombatMode();
	int getHealth();
	int getMaxHealth();
	bool increaseHealth(int amount);
	bool decreaseHealth(int amount);
	bool setHealth(int amount);
	bool setMaxHealth(int amount);
	void die_fighting(bool left);
	int getAnimation();
	bool hasSword();
	bool collisionDown();
	bool jumping();
	bool facing_left();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getPosition();
	void takeOffSword();

private:
	void signals();
	void teleport();
	void initVector();
	
	void setAnimationSprite();
	void updateAnimation();
	void calcMoviment(int deltaTime);
	void attackCalc();
	void PlayFX();
	void setPositionToRender();
	bool modifyPosX(float incX);
	bool modifyPosY(float incY);

private:
	int health, maxHealth, increaseH, decreaseH;
	int lastWalkFrame, lastAttackFrame, tileIniFallY, numAnimations;
	bool dead, has_Sword, nextLevel, canJump, first_time;
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	glm::ivec2 BoxCollision, QuadSize;
	glm::vec2 posPlayer, exit;
	float *animDist;
	float jumpDist;
	int lvlGetSword;
};


#endif // _PLAYER_INCLUDE


