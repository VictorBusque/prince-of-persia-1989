#include "Enemy.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#define SIZE_BOX_LEVEL_X 10*32
#define SIZE_BOX_LEVEL_Y 3*64

#define FALL_STEP 3

#define DMG_LVL_EASY 1
#define DMG_LVL_NORMAL 1
#define DMG_LVL_HARD 2

#define NUM_FRAMES_RETREATING 20

Enemy::Enemy(bool boss, Texture &SpriteSheet_Enemy)
{
	spritesheet = SpriteSheet_Enemy;
	if (!boss) {
		if (Game::instance().getLevel() == LEVEL_EASY) maxHealth = 2;
		else if (Game::instance().getLevel() == LEVEL_NORMAL) maxHealth = 3;
		else maxHealth = 4;
		health = maxHealth;
	}
	else {
		if (Game::instance().getLevel() == LEVEL_EASY) maxHealth = 3;
		else if (Game::instance().getLevel() == LEVEL_NORMAL) maxHealth = 5;
		else maxHealth = 7;
		health = maxHealth;
	}
}


Enemy::~Enemy()
{
}

void Enemy::free()
{
	sprite->free();
}
void Enemy::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::ivec2 iniPos)
{
	num_f_retreated = 0;
	retreating = false;
	lastAttackFrame = -1;
	glm::vec2 T = glm::vec2(1.f / 7.f, 1.f / 6.f);
	QuadSize = glm::ivec2(64, 64);
	BoxCollision = glm::ivec2(10, 40);
	sprite = Sprite::createSprite(QuadSize, T, &spritesheet, &shaderProgram);
	setAnimationSprite();
	sprite->changeAnimation(MOVE_LEFT);
	tileMapDispl = tileMapPos;//inicioScreen 0 0
	setPosition(iniPos);
}

void Enemy::setAnimationSprite() {
	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	for (int i = 0; i < 7; ++i) sprite->addKeyframe(MOVE_RIGHT, glm::vec2(i, 0));

	sprite->setAnimationSpeed(RETREAT_R, 8);
	for (int i = 0; i < 7; ++i) sprite->addKeyframe(RETREAT_R, glm::vec2(i, 0));

	sprite->setAnimationSpeed(ATTACK_RIGHT, 3);
	for (int i = 0; i < 4; ++i) sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(i, 1));

	sprite->setAnimationSpeed(DIE_RIGHT, 8);
	for (int i = 0; i < 5; ++i) sprite->addKeyframe(DIE_RIGHT, glm::vec2(i, 2));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	for (int i = 6; i >= 0; --i) sprite->addKeyframe(MOVE_LEFT, glm::vec2(i, 3));

	sprite->setAnimationSpeed(RETREAT_L, 8);
	for (int i = 6; i >= 0; --i) sprite->addKeyframe(RETREAT_L, glm::vec2(i, 3));

	sprite->setAnimationSpeed(ATTACK_LEFT, 3);
	for (int i = 6; i >= 3; --i) sprite->addKeyframe(ATTACK_LEFT, glm::vec2(i, 4));

	sprite->setAnimationSpeed(DIE_LEFT, 8);
	for (int i = 6; i >= 2; --i) sprite->addKeyframe(DIE_LEFT, glm::vec2(i, 5));
}

bool Enemy::sameRoom(glm::ivec2 playerPos) {
	int playerX = playerPos.x / (SIZE_BOX_LEVEL_X);
	int playerY = playerPos.y / (SIZE_BOX_LEVEL_Y);
	int EnemyX = getPosition().x / (SIZE_BOX_LEVEL_X);
	int EnemyY = getPosition().y / (SIZE_BOX_LEVEL_Y);
	return (playerX == EnemyX && playerY == EnemyY);
}

void Enemy::update(int deltaTime, glm::ivec2 playerPos, Player *p, int PlayerAnimation)
{
	if (health > 0) {
		if (!p->isDead()) {
			sprite->update(deltaTime);
			if (!retreating && !p->isDead() && lastAttackFrame != sprite->getKeyframe() && ((sprite->animation() == ATTACK_LEFT) || (sprite->animation() == ATTACK_RIGHT))) {
				PlayFX();
				if (sprite->getKeyframe() == 2) {
					int dmg;
					switch (Game::instance().getLevel()) {
					case LEVEL_EASY:
						dmg = DMG_LVL_EASY;
						break;
					case LEVEL_NORMAL:
						dmg = DMG_LVL_NORMAL;
						break;
					case LEVEL_HARD:
						dmg = DMG_LVL_HARD;
						break;
					}
					if ((sprite->animation() == ATTACK_RIGHT && PlayerAnimation != DEFEND_L) || (sprite->animation() == ATTACK_LEFT && PlayerAnimation != DEFEND_R)) {
						Game::instance().play(HARM);
						if (!p->decreaseHealth(dmg)) {//returns false if death ends up being 0
							p->die_fighting((p->facing_left()));
						}
					}
					else if ((sprite->animation() == ATTACK_RIGHT && PlayerAnimation == DEFEND_L) || (sprite->animation() == ATTACK_LEFT && PlayerAnimation == DEFEND_R)) {
						Game::instance().play(SWORD_DEFENSE);
						retreating = true;
					}
				}
				lastAttackFrame = sprite->getKeyframe();
			}
			if (sameRoom(playerPos)) calcMoviment(playerPos, PlayerAnimation, p);
			modifyPosY(FALL_STEP);
			setPositionToRender();
		}
		else {
			if (sprite->animation() == ATTACK_LEFT || sprite->animation() == MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
			else sprite->changeAnimation(MOVE_RIGHT);
		}
	}
	else{
		if (!sprite->isLastKeyframe()){
			sprite->update(deltaTime);
		}
	}
}

void Enemy::calcMoviment(glm::ivec2 playerPos, int PlayerAnimation, Player *p) {
	int ATTACK_RANGE = 1 * 32;
	float diffTile_x = (getPosition().x) - (playerPos.x);
	float diffTile_y = int((getPosition().y / 64) - (playerPos.y / 64));

	if (retreating) {
		if (sprite->animation() % 2 == 0) {//left
			modifyPosX(1);
			if (sprite->animation() != RETREAT_L) 
				sprite->changeAnimation(RETREAT_L);
		}
		else {//right
			modifyPosX(-1);
			if (sprite->animation() != RETREAT_R) 
				sprite->changeAnimation(RETREAT_R);
		}

		num_f_retreated++;
		if (num_f_retreated == NUM_FRAMES_RETREATING) {
			retreating = false;
			num_f_retreated = 0;
		}
	}
	else if (diffTile_x > ATTACK_RANGE) {
		modifyPosX(-1);
		if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
	}
	else if (diffTile_x >= 0 && diffTile_x <= ATTACK_RANGE && diffTile_y == 0) {
		if (sprite->animation() != ATTACK_LEFT) sprite->changeAnimation(ATTACK_LEFT);
	}
	else if (diffTile_x < 0 && diffTile_x >= -ATTACK_RANGE && diffTile_y == 0) {
		if (sprite->animation() != ATTACK_RIGHT) sprite->changeAnimation(ATTACK_RIGHT);
	}
	else if (diffTile_x < -ATTACK_RANGE) {
		modifyPosX(1);
		if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
	}
}

void Enemy::render(glm::vec2 cuadranteTraslate)
{
	sprite->render(cuadranteTraslate);
}

void Enemy::setTileMap(TileMap *tileMap){ map = tileMap; }

glm::ivec2 Enemy::getPosition() { return posEnemy; };

void Enemy::setPosition(const glm::vec2 &pos)
{	//li pasa en cap, es gurada en cap
	//posicion global
	//pixel superior izquierdo
	posEnemy = pos;
	setPositionToRender();
}

void Enemy::setPositionToRender(){
	glm::ivec2 posRender = posEnemy - glm::ivec2((QuadSize.x - BoxCollision.x) / 2, QuadSize.y - BoxCollision.y);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRender.x), float(tileMapDispl.y + posRender.y)));
}

int Enemy::getHealth() {
	return health;
}

int Enemy::getMaxHealth() {
	return maxHealth;
}

bool Enemy::decreaseHealth(int amount) {
	if (health - amount <= 0) {
		health = 0;
		return false;
	}
	health -= amount;
	return true;
}

void Enemy::PlayFX() {
	switch (sprite->animation()) {
	case ATTACK_RIGHT:
	case ATTACK_LEFT:
		if (sprite->getKeyframe() == 2) {
			Game::instance().play(GUARD_HIT);
		}
	}
}


void Enemy::die_fighting(bool left) {
	Game::instance().play(TAKE_SWORD_KILL_ENEMY);
	if (left) sprite->changeAnimation(DIE_LEFT);
	else sprite->changeAnimation(DIE_RIGHT);
}

void Enemy::modifyPosX(float incX){ //true si no collision/ si ha podidio modificarlo
	int i = 1;
	if (incX < 0){
		while (i <= -incX && !map->collisionMoveLeft(glm::vec2(posEnemy.x - i, posEnemy.y), BoxCollision)) i++;
		if (!(i <= -incX) && !map->collisionMoveLeft(glm::vec2(posEnemy.x + incX, posEnemy.y), BoxCollision)) posEnemy.x += incX;
		else posEnemy.x -= (i - 1);
	}
	else{
		while (i <= incX && !map->collisionMoveRight(glm::vec2(posEnemy.x + i, posEnemy.y), BoxCollision)) i++;
		if (!(i <= incX) && !map->collisionMoveRight(glm::vec2(posEnemy.x + incX, posEnemy.y), BoxCollision)) posEnemy.x += incX;
		else posEnemy.x += (i - 1);
	}
}

void Enemy::modifyPosY(float incY){
	int i = 1;
	while (i <= incY && !map->collisionMoveDown(glm::vec2(posEnemy.x, posEnemy.y + i), BoxCollision)) i++;
	if (!(i <= incY) && !map->collisionMoveDown(glm::vec2(posEnemy.x, posEnemy.y + incY), BoxCollision)) posEnemy.y += incY;
	else posEnemy.y += (i - 1);
}

bool Enemy::collisionable(glm::ivec2 pos, glm::ivec2 size){
	if (sameRoom(pos) && !((sprite->animation() == DIE_LEFT || sprite->animation() == DIE_RIGHT))){
		glm::ivec2 pSize = pos + size;
		bool colX, colY;
		colX = colY = false;
		if (pos.x <= posEnemy.x  && posEnemy.x <= pSize.x) colX = true;
		if (pos.x <= posEnemy.x + BoxCollision.x && posEnemy.x + BoxCollision.x <= pSize.x) colX = true;
		if (pos.y <= posEnemy.y  && posEnemy.y <= pSize.y) colY = true;
		if (pos.y <= posEnemy.y + BoxCollision.y && posEnemy.y + BoxCollision.y <= pSize.y) colY = true;
		return colX&&colY;
	}
	else return false;
}