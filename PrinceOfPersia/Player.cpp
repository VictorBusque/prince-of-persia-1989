#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define FALL_STEP 3  	  //'velocidad de caida'
#define DMG 1

void Player::setAnimationSprite(){
	numAnimations = 66;
	sprite->setNumberAnimations(numAnimations);

	//right
	sprite->setAnimationSpeed(BENDING_R, 8);
	for (int i = 1; i < 3; ++i) sprite->addKeyframe(BENDING_R, glm::vec2(i, 0));
	sprite->setAnimationSpeed(BENDED_R, 8);
	sprite->addKeyframe(BENDED_R, glm::vec2(3, 0));
	sprite->setAnimationSpeed(BEND_STEP_R, 8);
	for (int i = 4; i < 7; ++i) sprite->addKeyframe(BEND_STEP_R, glm::vec2(i, 0));
	sprite->setAnimationSpeed(WAKE_R, 8);
	for (int i = 4; i < 12; ++i) sprite->addKeyframe(WAKE_R, glm::vec2(i, 0));

	sprite->setAnimationSpeed(STAND_R, 8);
	sprite->addKeyframe(STAND_R, glm::vec2(0, 0));
	sprite->setAnimationSpeed(START_R, 8);
	for (int i = 0; i < 6; ++i) sprite->addKeyframe(START_R, glm::vec2(i, 1));
	sprite->setAnimationSpeed(MOVE_R, 8);
	for (int i = 6; i < 14; ++i) sprite->addKeyframe(MOVE_R, glm::vec2(i, 1));
	sprite->setAnimationSpeed(STOP_R, 8);
	for (int i = 0; i < 7; ++i) sprite->addKeyframe(STOP_R, glm::vec2(i, 9));

	sprite->setAnimationSpeed(SMALL_JUMP_INI_R, 8);
	for (int i = 0; i < 3; ++i) sprite->addKeyframe(SMALL_JUMP_INI_R, glm::vec2(i, 7));
	sprite->setAnimationSpeed(SMALL_JUMP_JUMP_R, 8);
	for (int i = 3; i < 9; ++i) sprite->addKeyframe(SMALL_JUMP_JUMP_R, glm::vec2(i, 7));
	sprite->setAnimationSpeed(SMALL_JUMP_LAND_R, 8);
	for (int i = 9; i < 14; ++i) sprite->addKeyframe(SMALL_JUMP_LAND_R, glm::vec2(i, 7));

	sprite->setAnimationSpeed(BIG_JUMP_R, 8);
	for (int i = 0; i < 11; ++i) sprite->addKeyframe(BIG_JUMP_R, glm::vec2(i, 10));
	sprite->setAnimationSpeed(SLOW_SPIN_R, 8);
	for (int i = 0; i < 6; ++i) sprite->addKeyframe(SLOW_SPIN_R, glm::vec2(i, 11));
	sprite->setAnimationSpeed(FAST_SPIN_R, 8);
	for (int i = 0; i < 11; ++i) sprite->addKeyframe(FAST_SPIN_R, glm::vec2(i, 8));

	sprite->setAnimationSpeed(SW_PAUSE_R, 8);
	sprite->addKeyframe(SW_PAUSE_R, glm::vec2(2, 3));
	sprite->setAnimationSpeed(ENFUNDAR_R, 12);
	for (int i = 0; i < 9; ++i) sprite->addKeyframe(ENFUNDAR_R, glm::vec2(i, 3));
	sprite->setAnimationSpeed(DESENFUNDAR_R, 12);
	for (int i = 0; i < 5; ++i) sprite->addKeyframe(DESENFUNDAR_R, glm::vec2(i, 4));
	sprite->setAnimationSpeed(ATACK_R, 13);
	for (int i = 0; i < 8; ++i) sprite->addKeyframe(ATACK_R, glm::vec2(i, 5));
	sprite->setAnimationSpeed(DEFEND_R, 4);
	for (int i = 0; i < 4; ++i) sprite->addKeyframe(DEFEND_R, glm::vec2(i, 6));

	sprite->setAnimationSpeed(STEP_R, 8);
	for (int i = 0; i < 8; ++i) sprite->addKeyframe(STEP_R, glm::vec2(i, 12));
	sprite->setAnimationSpeed(DRINK_R, 8);
	for (int i = 0; i < 12; ++i) sprite->addKeyframe(DRINK_R, glm::vec2(i, 13));

	sprite->setAnimationSpeed(VERTICAL_JUMP_UP_S_R, 8);
	for (int i = 0; i < 7; ++i) sprite->addKeyframe(VERTICAL_JUMP_UP_S_R, glm::vec2(i, 15));
	sprite->setAnimationSpeed(VERTICAL_JUMP_UP_J_R, 8);
	for (int i = 7; i < 11; ++i) sprite->addKeyframe(VERTICAL_JUMP_UP_J_R, glm::vec2(i, 15));
	sprite->setAnimationSpeed(VERTICAL_JUMP_DW_R, 8);
	for (int i = 11; i < 13; ++i) sprite->addKeyframe(VERTICAL_JUMP_DW_R, glm::vec2(i, 15));

	sprite->setAnimationSpeed(CLIMB_R, 8);
	for (int i = 0; i < 12; ++i) sprite->addKeyframe(CLIMB_R, glm::vec2(i, 17));
	sprite->setAnimationSpeed(DOOR_R, 8);
	for (int i = 0; i < 10; ++i) sprite->addKeyframe(DOOR_R, glm::vec2(i, 18));

	sprite->setAnimationSpeed(FALL_R, 8);
	sprite->addKeyframe(FALL_R, glm::vec2(0, 14));
	sprite->setAnimationSpeed(DIE_C_R, 8);
	sprite->addKeyframe(DIE_C_R, glm::vec2(1, 14));
	sprite->setAnimationSpeed(DIE_P_R, 8);
	sprite->addKeyframe(DIE_P_R, glm::vec2(2, 14));
	sprite->setAnimationSpeed(DIE_F_R, 8);
	for (int i = 5; i < 10; ++i) sprite->addKeyframe(DIE_F_R, glm::vec2(i, 14));
	sprite->setAnimationSpeed(DEAD_F_R, 6);
	sprite->addKeyframe(DEAD_F_R, glm::vec2(9, 14));

	sprite->setAnimationSpeed(WALK_SWORD_R, 4);
	sprite->addKeyframe(WALK_SWORD_R, glm::vec2(0, 5));
	sprite->addKeyframe(WALK_SWORD_R, glm::vec2(6, 5));
	sprite->addKeyframe(WALK_SWORD_R, glm::vec2(7, 5));

	sprite->setAnimationSpeed(RETREAT_SWORD_R, 4);
	sprite->addKeyframe(RETREAT_SWORD_R, glm::vec2(0, 5));
	sprite->addKeyframe(RETREAT_SWORD_R, glm::vec2(6, 5));
	sprite->addKeyframe(RETREAT_SWORD_R, glm::vec2(7, 5));


	//left
	sprite->setAnimationSpeed(BENDING_L, 8);
	for (int i = 12; 10 < i; --i) sprite->addKeyframe(BENDING_L, glm::vec2(i, 19));
	sprite->setAnimationSpeed(BENDED_L, 8);
	sprite->addKeyframe(BENDED_L, glm::vec2(10, 19));
	sprite->setAnimationSpeed(BEND_STEP_L, 8);
	for (int i = 9; 6 < i; --i) sprite->addKeyframe(BEND_STEP_L, glm::vec2(i, 19));
	sprite->setAnimationSpeed(WAKE_L, 8);
	for (int i = 9; 1 < i; --i) sprite->addKeyframe(WAKE_L, glm::vec2(i, 19));

	sprite->setAnimationSpeed(STAND_L, 8);
	sprite->addKeyframe(STAND_L, glm::vec2(13, 19));
	sprite->setAnimationSpeed(START_L, 8);
	for (int i = 13; 7 < i; --i) sprite->addKeyframe(START_L, glm::vec2(i, 20));
	sprite->setAnimationSpeed(MOVE_L, 8);
	for (int i = 7; -1 < i; --i) sprite->addKeyframe(MOVE_L, glm::vec2(i, 20));
	sprite->setAnimationSpeed(STOP_L, 8);
	for (int i = 13; 6 < i; --i) sprite->addKeyframe(STOP_L, glm::vec2(i, 28));

	sprite->setAnimationSpeed(SMALL_JUMP_INI_L, 8);
	for (int i = 13; 10 < i; i--) sprite->addKeyframe(SMALL_JUMP_INI_L, glm::vec2(i, 26));
	sprite->setAnimationSpeed(SMALL_JUMP_JUMP_L, 8);
	for (int i = 10; 4 < i; i--) sprite->addKeyframe(SMALL_JUMP_JUMP_L, glm::vec2(i, 26));
	sprite->setAnimationSpeed(SMALL_JUMP_LAND_L, 8);
	for (int i = 4; 0 <= i; i--) sprite->addKeyframe(SMALL_JUMP_LAND_L, glm::vec2(i, 26));

	sprite->setAnimationSpeed(BIG_JUMP_L, 8);
	for (int i = 13; 2 < i; --i) sprite->addKeyframe(BIG_JUMP_L, glm::vec2(i, 29));
	sprite->setAnimationSpeed(SLOW_SPIN_L, 8);
	for (int i = 13; 7 < i; --i) sprite->addKeyframe(SLOW_SPIN_L, glm::vec2(i, 30));
	sprite->setAnimationSpeed(FAST_SPIN_L, 8);
	for (int i = 13; 2 < i; --i) sprite->addKeyframe(FAST_SPIN_L, glm::vec2(i, 27));

	sprite->setAnimationSpeed(SW_PAUSE_L, 8);
	sprite->addKeyframe(SW_PAUSE_L, glm::vec2(11, 22));
	sprite->setAnimationSpeed(ENFUNDAR_L, 12);
	for (int i = 13; 4 < i; --i) sprite->addKeyframe(ENFUNDAR_L, glm::vec2(i, 22));
	sprite->setAnimationSpeed(DESENFUNDAR_L, 12);
	for (int i = 13; 8 < i; --i) sprite->addKeyframe(DESENFUNDAR_L, glm::vec2(i, 23));
	sprite->setAnimationSpeed(ATACK_L, 13);
	for (int i = 13; 5 < i; --i) sprite->addKeyframe(ATACK_L, glm::vec2(i, 24));
	sprite->setAnimationSpeed(DEFEND_L, 4);
	for (int i = 13; 9 < i; --i) sprite->addKeyframe(DEFEND_L, glm::vec2(i, 25));

	sprite->setAnimationSpeed(STEP_L, 8);
	for (int i = 13; 5 < i; --i) sprite->addKeyframe(STEP_L, glm::vec2(i, 31));
	sprite->setAnimationSpeed(DRINK_L, 8);
	for (int i = 13; 1 < i; --i) sprite->addKeyframe(DRINK_L, glm::vec2(i, 32));

	sprite->setAnimationSpeed(VERTICAL_JUMP_UP_S_L, 8);
	for (int i = 13; 6 < i; --i) sprite->addKeyframe(VERTICAL_JUMP_UP_S_L, glm::vec2(i, 34));
	sprite->setAnimationSpeed(VERTICAL_JUMP_UP_J_L, 8);
	for (int i = 6; 1 < i; --i) sprite->addKeyframe(VERTICAL_JUMP_UP_J_L, glm::vec2(i, 34));
	sprite->setAnimationSpeed(VERTICAL_JUMP_DW_L, 8);
	for (int i = 2; 0 < i; --i) sprite->addKeyframe(VERTICAL_JUMP_DW_L, glm::vec2(i, 34));

	sprite->setAnimationSpeed(CLIMB_L, 8);
	for (int i = 13; 1 < i; --i) sprite->addKeyframe(CLIMB_L, glm::vec2(i, 36));
	sprite->setAnimationSpeed(DOOR_L, 8);
	for (int i = 13; 3 < i; --i) sprite->addKeyframe(DOOR_L, glm::vec2(i, 37));

	sprite->setAnimationSpeed(FALL_L, 8);
	sprite->addKeyframe(FALL_L, glm::vec2(13, 33));
	sprite->setAnimationSpeed(DIE_C_L, 8);
	sprite->addKeyframe(DIE_C_L, glm::vec2(12, 33));
	sprite->setAnimationSpeed(DIE_P_L, 8);
	sprite->addKeyframe(DIE_P_L, glm::vec2(11, 33));
	sprite->setAnimationSpeed(DIE_F_L, 6);
	for (int i = 8; 3 < i; --i) sprite->addKeyframe(DIE_F_L, glm::vec2(i, 33));
	sprite->setAnimationSpeed(DEAD_F_L, 8);
	sprite->addKeyframe(DEAD_F_L, glm::vec2(4, 33));


	sprite->setAnimationSpeed(WALK_SWORD_L, 4);
	sprite->addKeyframe(WALK_SWORD_L, glm::vec2(13, 24));
	sprite->addKeyframe(WALK_SWORD_L, glm::vec2(7, 24));
	sprite->addKeyframe(WALK_SWORD_L, glm::vec2(6, 24));

	
	sprite->setAnimationSpeed(RETREAT_SWORD_L, 4);
	sprite->addKeyframe(RETREAT_SWORD_L, glm::vec2(13, 24));
	sprite->addKeyframe(RETREAT_SWORD_L, glm::vec2(7, 24));
	sprite->addKeyframe(RETREAT_SWORD_L, glm::vec2(6, 24));
}

void Player::updateAnimation(){
	switch (sprite->animation())
	{
		//RIGHT
	case STAND_R:
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			sprite->changeAnimation(SLOW_SPIN_L);
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && collisionDown()){
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(SMALL_JUMP_INI_R);
			else sprite->changeAnimation(VERTICAL_JUMP_UP_S_R);
		}
		else if (collisionDown() && Game::instance().getSpecialKey(GLUT_KEY_PAGE_UP))//Small jump with 9 in numpad
			sprite->changeAnimation(SMALL_JUMP_INI_R);
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)){
			if (Game::instance().getModifierKey(GLUT_ACTIVE_SHIFT)) sprite->changeAnimation(STEP_R);
			else sprite->changeAnimation(START_R);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			sprite->changeAnimation(BENDING_R);
		break;
		//moverse
	case START_R:
		if (sprite->isLastKeyframe()){
			if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
				sprite->changeAnimation(MOVE_R);
			else sprite->changeAnimation(STOP_R);
		}
		break;
	case MOVE_R:
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			sprite->changeAnimation(FAST_SPIN_R);
		else if (!Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			sprite->changeAnimation(STOP_R);
		else if (collisionDown() && Game::instance().getSpecialKeyPressed(GLUT_KEY_UP)){
			sprite->changeAnimation(BIG_JUMP_R);
		}
		break;
	case STOP_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_R);
		break;
		//Other moves
	case STEP_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_R);
		break;
	case SLOW_SPIN_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_L);
		break;
	case FAST_SPIN_R:
		if (sprite->isLastKeyframe()){
			sprite->changeAnimation(MOVE_L);
		}
		break;
		//saltos
	case SMALL_JUMP_INI_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(SMALL_JUMP_JUMP_R);
		break;
	case SMALL_JUMP_JUMP_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(SMALL_JUMP_LAND_R);
		break;
	case SMALL_JUMP_LAND_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_R);
		break;

	case BIG_JUMP_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(MOVE_R);
		break;
	case VERTICAL_JUMP_UP_S_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(VERTICAL_JUMP_UP_J_R);
		break;
	case VERTICAL_JUMP_UP_J_R:
		if (sprite->isLastKeyframe()){
			if (map->inJumpeableTile(getPosition(), facing_left()))
				sprite->changeAnimation(CLIMB_R);
			else sprite->changeAnimation(VERTICAL_JUMP_DW_R);
		}
		break;
	case VERTICAL_JUMP_DW_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_R);
		break;
		//agacharse
	case BENDING_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(BENDED_R);
		break;
	case BENDED_R:
		if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			sprite->changeAnimation(WAKE_R);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_RIGHT)){
			sprite->changeAnimation(BEND_STEP_R);
		}
		break;
	case WAKE_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_R);
		break;
	case BEND_STEP_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(BENDED_R);
		break;
	case FALL_R:
		if (collisionDown()){
			sprite->changeAnimation(BENDING_R);
			int tileFalled = posPlayer.y / 64 - tileIniFallY;

			if (tileFalled <= 1) Game::instance().play(LAND_SOFT);
			else if (tileFalled == 2) {
				Game::instance().play(LAND_HARM);
				decreaseHealth(decreaseH);
			}
			else if (tileFalled >= 3){
				Game::instance().play(LAND_FATAL);
				decreaseHealth(maxHealth);
			}
			if (dead){
				sprite->changeAnimation(DIE_C_R);
				Game::instance().playerDead();
			}
		}
		break;
	case DOOR_R:
		if (sprite->isLastKeyframe()){
			if (nextLevel) Game::instance().nextLevel();
			else teleport();
			sprite->changeAnimation(STAND_R);
		}
		break;
	//atackes 
	case RETREAT_SWORD_R:
		if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			sprite->changeAnimation(SW_PAUSE_R);
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			sprite->changeAnimation(WALK_SWORD_R);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP))
			sprite->changeAnimation(DEFEND_R);
		else if (Game::instance().getKeyPressed(' '))
			sprite->changeAnimation(ATACK_R);
		break;
	case WALK_SWORD_R:
		if (!Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			sprite->changeAnimation(SW_PAUSE_R);
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			sprite->changeAnimation(RETREAT_SWORD_R);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP))
			sprite->changeAnimation(DEFEND_R);
		else if (Game::instance().getKeyPressed(' '))
			sprite->changeAnimation(ATACK_R);
		break;
	case SW_PAUSE_R:
		if (Game::instance().getModifierKeyPressed(GLUT_ACTIVE_CTRL))
			sprite->changeAnimation(ENFUNDAR_R);
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			sprite->changeAnimation(WALK_SWORD_R);
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			sprite->changeAnimation(RETREAT_SWORD_R);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP))
			sprite->changeAnimation(DEFEND_R);
		else if (Game::instance().getKeyPressed(' '))
			sprite->changeAnimation(ATACK_R);
		break;
	case ATACK_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(SW_PAUSE_R);
		break;
	case DEFEND_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(SW_PAUSE_R);
		break;
	case DESENFUNDAR_R:
		if (sprite->isLastKeyframe()){
			if (has_Sword) sprite->changeAnimation(SW_PAUSE_R);
			else{
				has_Sword = true;
				sprite->changeAnimation(ENFUNDAR_R);
			}
		}
		break;
	case DIE_F_R:
		if (sprite->isLastKeyframe()){
			sprite->changeAnimation(DEAD_F_R);
			Game::instance().playerDead();
		}
		break;
	//acavar animacion:
	case ENFUNDAR_R:
	case DRINK_R:
	case CLIMB_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_R);
		break;

		//LEFT
	case STAND_L:
		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			sprite->changeAnimation(SLOW_SPIN_R);
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP) && collisionDown()){
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(SMALL_JUMP_INI_L);
			else sprite->changeAnimation(VERTICAL_JUMP_UP_S_L);
		}
		else if (collisionDown() && Game::instance().getSpecialKey(GLUT_KEY_HOME))//Small jump with 7 in numpad
			sprite->changeAnimation(SMALL_JUMP_INI_L);
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
			if (Game::instance().getModifierKey(GLUT_ACTIVE_SHIFT)) sprite->changeAnimation(STEP_L);
			else sprite->changeAnimation(START_L);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			sprite->changeAnimation(BENDING_L);
		break;
		//moverse
	case START_L:
		if (sprite->isLastKeyframe()){
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
				sprite->changeAnimation(MOVE_L);
			else sprite->changeAnimation(STOP_L);
		}
		break;
	case MOVE_L:
		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			sprite->changeAnimation(FAST_SPIN_L);
		else if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			sprite->changeAnimation(STOP_L);
		else if (collisionDown() && Game::instance().getSpecialKeyPressed(GLUT_KEY_UP)){
			sprite->changeAnimation(BIG_JUMP_L);
		}
		break;
	case STOP_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_L);
		break;
		//Other moves
	case STEP_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_L);
		break;
	case SLOW_SPIN_R:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_R);
		break;
	case FAST_SPIN_L:
		if (sprite->isLastKeyframe()){
			sprite->changeAnimation(MOVE_R);
		}
		break;
		//saltos
	case SMALL_JUMP_INI_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(SMALL_JUMP_JUMP_L);
		break;
	case SMALL_JUMP_JUMP_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(SMALL_JUMP_LAND_L);
		break;
	case SMALL_JUMP_LAND_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_L);
		break;
	case BIG_JUMP_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(MOVE_L);
		break;
	case VERTICAL_JUMP_UP_S_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(VERTICAL_JUMP_UP_J_L);
		break;
	case VERTICAL_JUMP_UP_J_L:
		if (sprite->isLastKeyframe()){
			if (map->inJumpeableTile(getPosition(), facing_left())) sprite->changeAnimation(CLIMB_L);
			else sprite->changeAnimation(VERTICAL_JUMP_DW_L);;
		}
		break;
	case VERTICAL_JUMP_DW_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_L);
		break;
		//agacharse
	case BENDING_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(BENDED_L);
		break;
	case BENDED_L:
		if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			sprite->changeAnimation(WAKE_L);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_LEFT)){
			sprite->changeAnimation(BEND_STEP_L);
		}
		break;
	case WAKE_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_L);
		break;
	case BEND_STEP_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(BENDED_L);
		break;
	case FALL_L:
		if (collisionDown()){
			sprite->changeAnimation(BENDING_L);
			int tileFalled = posPlayer.y / 64 - tileIniFallY;

			if (tileFalled <= 1) Game::instance().play(LAND_SOFT);
			else if (tileFalled == 2) {
				Game::instance().play(LAND_HARM);
				decreaseHealth(decreaseH);
			}
			else if (tileFalled >= 3){
				Game::instance().play(LAND_FATAL);
				decreaseHealth(maxHealth);
			}
			if (dead){
				sprite->changeAnimation(DIE_C_L);
				Game::instance().playerDead();
			}
		}
		break;
	case DOOR_L:
		if (sprite->isLastKeyframe()){
			if (nextLevel) Game::instance().nextLevel();
			else teleport();
			sprite->changeAnimation(STAND_L);
		}
		break;
	//atackes
	case RETREAT_SWORD_L:
		if (!Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			sprite->changeAnimation(SW_PAUSE_L);
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			sprite->changeAnimation(WALK_SWORD_L);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP))
			sprite->changeAnimation(DEFEND_L);
		else if (Game::instance().getKeyPressed(' '))
			sprite->changeAnimation(ATACK_L);
		break;
	case WALK_SWORD_L:
		if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			sprite->changeAnimation(SW_PAUSE_L);
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			sprite->changeAnimation(RETREAT_SWORD_L);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP))
			sprite->changeAnimation(DEFEND_L);
		else if (Game::instance().getKeyPressed(' '))
			sprite->changeAnimation(ATACK_L);
		break;
	case SW_PAUSE_L:
		if (Game::instance().getModifierKeyPressed(GLUT_ACTIVE_CTRL))
			sprite->changeAnimation(ENFUNDAR_L);
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			sprite->changeAnimation(WALK_SWORD_L);
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			sprite->changeAnimation(RETREAT_SWORD_L);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP))
			sprite->changeAnimation(DEFEND_L);
		else if (Game::instance().getKeyPressed(' '))
			sprite->changeAnimation(ATACK_L);
		break;
	case ATACK_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(SW_PAUSE_L);
		break;
	case DEFEND_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(SW_PAUSE_L);
		break;
	case DESENFUNDAR_L:
		if (sprite->isLastKeyframe()){
			if (has_Sword) sprite->changeAnimation(SW_PAUSE_L);
			else{
				has_Sword = true;
				sprite->changeAnimation(ENFUNDAR_L);
			}
		}
		break;
	case DIE_F_L:
		if (sprite->isLastKeyframe()){
			sprite->changeAnimation(DEAD_F_L);
			Game::instance().playerDead();
		}
		break;
		//acavar animacion
	case ENFUNDAR_L:
	case DRINK_L:
	case CLIMB_L:
		if (sprite->isLastKeyframe())
			sprite->changeAnimation(STAND_L);
		break;
	case DEAD_F_L: case DEAD_F_R:
	case DIE_C_L: case DIE_C_R:
	case DIE_P_L: case DIE_P_R:
		break;
	default:
		break;
	}

	
}

void Player::signals(){
	if (sprite->animation() == STAND_L && Game::instance().getModifierKey(GLUT_ACTIVE_SHIFT)){
		exit = Game::instance().openedPortal(true);
		if (exit != glm::vec2(-1)){//entrar portal
			nextLevel = false;
			exit.x *= 32;
			exit.y *= 64;
			//apply offset
			sprite->changeAnimation(DOOR_L);
		}
		else if (Game::instance().openedDoor(true)){//entrar puerta
			nextLevel = true;
			//apply offset
			sprite->changeAnimation(DOOR_R);
		}
		else if (Game::instance().getPoti(true)){ //beber poti
			Game::instance().play(DRINK_POTION);
			sprite->changeAnimation(DRINK_L);
			increaseHealth(increaseH);
		}
		else if (!has_Sword && Game::instance().getSword(true)){ //cojer espada
			Game::instance().play(TAKE_SWORD_KILL_ENEMY);
			sprite->changeAnimation(DESENFUNDAR_L);
			lvlGetSword = Game::instance().getActualLevel();
		}
	}
	else if (sprite->animation() == STAND_R && Game::instance().getModifierKey(GLUT_ACTIVE_SHIFT)){
		exit = Game::instance().openedPortal(true);
		if (exit != glm::vec2(-1)){//entrar portal
			nextLevel = false;
			exit.x *= 32;
			exit.y *= 64;
			//apply offset
			sprite->changeAnimation(DOOR_R);
		}
		else if (Game::instance().openedDoor(false)){//entrar puerta
			nextLevel = true;
			//apply offset
			sprite->changeAnimation(DOOR_R);
		}
		else if (Game::instance().getPoti(false)){ //beber poti
			Game::instance().play(DRINK_POTION);
			sprite->changeAnimation(DRINK_R);
			increaseHealth(increaseH);
		}
		else if (!has_Sword && Game::instance().getSword(false)){//cojer espada
			Game::instance().play(TAKE_SWORD_KILL_ENEMY);
			sprite->changeAnimation(DESENFUNDAR_R);
			lvlGetSword = Game::instance().getActualLevel();
		}
	}

	if (has_Sword){
		if (sprite->animation() == STAND_L && Game::instance().getModifierKeyPressed(GLUT_ACTIVE_CTRL)){
			Game::instance().play(SWORD_DRAWN);
			sprite->changeAnimation(DESENFUNDAR_L);
		}
		else if (sprite->animation() == STAND_R && Game::instance().getModifierKeyPressed(GLUT_ACTIVE_CTRL)){
			Game::instance().play(SWORD_DRAWN);
			sprite->changeAnimation(DESENFUNDAR_R);
		}
	}

	//caer
	if ((sprite->animation() != FALL_L && sprite->animation() != FALL_R) &&
		(sprite->animation() != DIE_P_L && sprite->animation() != DIE_P_R)
		&&	!jumping() && !collisionDown()){
		tileIniFallY = posPlayer.y/64;
		if (facing_left()) sprite->changeAnimation(FALL_L);
		else  sprite->changeAnimation(FALL_R);
	}

	//morir pincho
	if (!dead && !jumping() && Game::instance().hasSSprite(PINCHO,posPlayer.x/32,posPlayer.y/64)){
		Game::instance().play(SPIKED);
		decreaseHealth(maxHealth);
		Game::instance().playerDead();
		if (facing_left()) sprite->changeAnimation(DIE_P_L);
		else sprite->changeAnimation(DIE_P_R);
	}
}

void Player::calcMoviment(int deltaTime){	
	int animacio = sprite->animation();
	if (facing_left()) animacio -= numAnimations / 2;

	float inc = animDist[animacio] / 6;

	if (sprite->animation() == VERTICAL_JUMP_UP_J_R || sprite->animation() == VERTICAL_JUMP_UP_J_L) {
		if (!facing_left() && (map->getTileType(posPlayer.x + 32, posPlayer.y) == CORNER ||
			(map->getTileType(posPlayer.x + 32, posPlayer.y) >= BLOCK1 && map->getTileType(posPlayer.x + 32, posPlayer.y) <= BLOCK5))) {
			posPlayer.y -= (jumpDist + 0.15f);
		}
		else if (!facing_left())posPlayer.y -= jumpDist;
		else posPlayer.y -= jumpDist * 4 / 5;
		int tilex = posPlayer.x / 32;
		if (!facing_left()) {
			if (map->inJumpeableTile(posPlayer, facing_left())) posPlayer.x = tilex * 32 + 20;
		}
		else {
			if (map->inJumpeableTile(posPlayer, facing_left())) posPlayer.x = tilex * 32 + 17;
		}
	}
	if (sprite->animation() == CLIMB_L || sprite->animation() == CLIMB_R) {
		posPlayer.y -= 0.5f;
		if (!facing_left() && sprite->getKeyframe() >= 7) posPlayer.x += 1.f;
		if (facing_left() && sprite->getKeyframe() >= 7)  posPlayer.x -= 1.f;
	}

	if (sprite->animation() == VERTICAL_JUMP_DW_R || sprite->animation() == VERTICAL_JUMP_DW_L)
		modifyPosY(jumpDist);
	
	if (sprite->animation() == DOOR_R || sprite->animation() == DOOR_L)
		modifyPosY(-0.20f);
	
	if (sprite->animation() == BIG_JUMP_L || sprite->animation() == BIG_JUMP_R){
		if (sprite->getKeyframe() <= 5 && sprite->getKeyframe() > 0) modifyPosY(-0.4f);
		else if (sprite->getKeyframe() != 0) modifyPosY(0.4f);
	}


	if (facing_left()){
		if (!modifyPosX(-inc))  {
			sprite->changeAnimation(STAND_L);
			Game::instance().play(BUMP);
		}
	}
	else {
		if (!modifyPosX(inc)) {
			sprite->changeAnimation(STAND_R);
			Game::instance().play(BUMP);
		}
	}
	if (!jumping()) modifyPosY(FALL_STEP);//apply gravity 

}

bool Player::modifyPosX(float incX){ //true si no collision/ si ha podidio modificarlo
	int i = 1;
	if (incX < 0){
		while (i <= -incX && !map->collisionMoveLeft(glm::vec2(posPlayer.x - i, posPlayer.y ), BoxCollision)) i++;
		if (!(i <= -incX) && !map->collisionMoveLeft(glm::vec2(posPlayer.x + incX, posPlayer.y), BoxCollision)){
			posPlayer.x += incX;
			return true;
		}
		else {
			posPlayer.x -= (i - 1);
			return false;
		}
	}
	else{
		while (i <= incX && !map->collisionMoveRight(glm::vec2(posPlayer.x + i, posPlayer.y), BoxCollision)) i++;
		if (!(i <= incX) && !map->collisionMoveRight(glm::vec2(posPlayer.x + incX, posPlayer.y), BoxCollision)){
			posPlayer.x += incX;
			return true;
		}
		else {
			posPlayer.x += (i - 1);
			return false;
		}
	}
}

bool Player::modifyPosY(float incY){
	int i = 1;
	if (incY < 0){
		while (i <= -incY && !map->collisionMoveUp(glm::vec2(posPlayer.x, posPlayer.y - i), BoxCollision)) i++;
		if (!(i <= -incY) && !map->collisionMoveUp(glm::vec2(posPlayer.x, posPlayer.y + incY), BoxCollision)){
			posPlayer.y += incY;
			return true;
		}
		else {
			posPlayer.y -= (i - 1);
			return false;
		}
	}
	else{
		while (i <= incY && !map->collisionMoveDown(glm::vec2(posPlayer.x, posPlayer.y + i), BoxCollision)) i++;
		if (!(i <= incY) && !map->collisionMoveDown(glm::vec2(posPlayer.x, posPlayer.y + incY), BoxCollision)){
			posPlayer.y += incY;
			return true;
		}
		else {
			posPlayer.y += (i - 1);
			return false;
		}
	}
}

bool Player::collisionDown(){//toco suelo
	glm::vec2 posConsulta = glm::vec2(posPlayer.x, posPlayer.y + 1);//suelo inmediato
	return map->collisionMoveDown(posConsulta, BoxCollision);
}

bool Player::jumping(){
	switch (sprite->animation())
	{
	case SMALL_JUMP_INI_R:		case SMALL_JUMP_INI_L:
	case SMALL_JUMP_JUMP_R:		case SMALL_JUMP_JUMP_L:
	case SMALL_JUMP_LAND_R:		case SMALL_JUMP_LAND_L:
	case BIG_JUMP_R:			case BIG_JUMP_L:
	case VERTICAL_JUMP_UP_S_R:	case VERTICAL_JUMP_UP_S_L:
	case VERTICAL_JUMP_UP_J_R:	case VERTICAL_JUMP_UP_J_L:
	case VERTICAL_JUMP_DW_R:	case VERTICAL_JUMP_DW_L:
	case CLIMB_R:				case CLIMB_L:
	case DOOR_R:				case DOOR_L:
		return true; break;
	default: return false; break;
	}
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	dead = has_Sword = nextLevel = false;
	lastWalkFrame = lastAttackFrame = lvlGetSword = -1;	
	glm::vec2 T = glm::vec2(1.f / 14.f, 1.f / 38.f);
	QuadSize = glm::ivec2(64, 64);
	BoxCollision = glm::ivec2(10, 40);
	spritesheet.loadFromFile("images/Prince_Spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(QuadSize, T, &spritesheet, &shaderProgram);
	setAnimationSprite();
	sprite->changeAnimation(STAND_R);
	tileMapDispl = tileMapPos;//inicioScreen 0 0
	initVector();
	decreaseH = 1;
}

void Player::restart(){
	switch (Game::instance().getLevel())
	{
	case LEVEL_EASY:
		setMaxHealth(5);
		increaseH = 3;
		break;
	case LEVEL_NORMAL:
		setMaxHealth(3);
		increaseH = 2;
		break;
	case LEVEL_HARD:
		setMaxHealth(2);
		increaseH = 1;
		break;
	default:
		break;
	}
	setHealth(maxHealth);
	dead = nextLevel = false;
	lastWalkFrame = lastAttackFrame = -1;
	sprite->changeAnimation(STAND_R);
	setHealth(maxHealth);
	if (lvlGetSword == -1) has_Sword = false;
	else if (Game::instance().getActualLevel() == lvlGetSword) has_Sword = false;	
}

void Player::PlayFX() {
	switch (sprite->animation()) {
	case MOVE_R:	case MOVE_L:
		if (sprite->getKeyframe() == 1 || sprite->getKeyframe() == 5) Game::instance().play(STEP);
		break;
	}
}

void Player::attackCalc(){
	if ((sprite->animation() == ATACK_L || sprite->animation() == ATACK_R) && lastAttackFrame != sprite->getKeyframe()) {
		int enem = Game::instance().getEnemyAtRange(getPosition());
		if (enem != -1) { //there's some enemy in the room
			Enemy *cEnem = Game::instance().getEnemy(enem);
			glm::vec2 ePos = cEnem->getPosition();
			float diff = getPosition().x - ePos.x;
			if (cEnem->getHealth() > 0 && (int(ePos.y / 64)) == (int(getPosition().y / 64)) && (diff >= -32 && diff <= 32) && sprite->getKeyframe() == 5) {
				Game::instance().play(SWORD_ATTACK);
				bool death = (!cEnem->decreaseHealth(DMG));
				if (death) cEnem->die_fighting(!facing_left());
			}
		}
		lastAttackFrame = sprite->getKeyframe();
	}
}

void Player::update(int deltaTime)
{	
	sprite->update(deltaTime);
	if (lastWalkFrame != sprite->getKeyframe()) {//canvi de frame
		PlayFX();
		lastWalkFrame = sprite->getKeyframe();
	}
	attackCalc();
	updateAnimation();
	signals();
	calcMoviment(deltaTime);
	setPositionToRender();
}

void Player::render(glm::vec2 cuadranteTraslate)
{
	sprite->render(cuadranteTraslate);
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

bool Player::facing_left(){
	return sprite->animation() >= numAnimations / 2;
}

void Player::setPosition(const glm::vec2 &pos)
{	//li pasa en cap, es gurada en cap
	//posicion global
	//pixel superior izquierdo
	posPlayer = pos;
	setPositionToRender();
}

void Player::setPositionToRender(){
	glm::ivec2 posRender = posPlayer - glm::vec2((QuadSize.x - BoxCollision.x) / 2, QuadSize.y - BoxCollision.y);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRender.x), float(tileMapDispl.y + posRender.y)));
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
};

int Player::getHealth() {
	return health;
}

int Player::getMaxHealth() {
	return maxHealth;
}

bool Player::increaseHealth(int amount) {
	if (health + amount > maxHealth) {
		health = maxHealth;
		return false;
	}
	health += amount;
	return true;
}
bool Player::decreaseHealth(int amount) {
	if (health - amount <= 0) {
		health = 0;
		dead = true;
		return false;
	}
	health -= amount;
	return true;
}
bool Player::setHealth(int amount) {
	if (amount > 0 && amount <= maxHealth) {
		health = amount;
		return true;
	}
	else return false;
}
bool Player::setMaxHealth(int amount) {
	if (amount > 0) {
		maxHealth = amount;
		return true;
	}
	else return false;
}

void Player::die_fighting(bool left) {
	Game::instance().stopThemeSong();
	Game::instance().play(FIGHT_DEATH);
	if (left) sprite->changeAnimation(DIE_F_L);
	else sprite->changeAnimation(DIE_F_R);
}

int Player::getAnimation() {
	return sprite->animation();
}

bool Player::isDead() {
	return dead;
}

bool Player::hasSword() {
	return has_Sword;
}

bool Player::inCombatMode(){
	int anim = sprite->animation();
	return (
		anim == DESENFUNDAR_R||
		anim == SW_PAUSE_R ||
		anim == ATACK_R ||
		anim == DEFEND_R ||
		anim == WALK_SWORD_R ||
		anim == RETREAT_SWORD_R ||
		anim == ENFUNDAR_R ||
		anim == DESENFUNDAR_L ||
		anim == SW_PAUSE_L ||
		anim == ATACK_L ||
		anim == DEFEND_L ||
		anim == WALK_SWORD_L ||
		anim == RETREAT_SWORD_L ||
		anim == ENFUNDAR_L
		);
}

void Player::teleport(){
	posPlayer = exit;
	posPlayer.x += 32;
	Game::instance().closePortal(exit.x/32,exit.y/64);
}

void Player::initVector() {
	animDist = new float[numAnimations / 2];
	animDist[BENDING_R] = 0;
	animDist[BENDED_R] = 0;
	animDist[BEND_STEP_R] = 0.25 * 32.0f;
	animDist[WAKE_R] = 0;
	animDist[STAND_R] = 0;
	animDist[START_R] = 0.2f * 32;
	animDist[MOVE_R] = 6;
	animDist[STOP_R] = (1.0 / 6.0) * 32;
	animDist[SMALL_JUMP_INI_R] = 0;
	animDist[SMALL_JUMP_JUMP_R] = (2.5f * 32.0f) / 6.0f;
	animDist[SMALL_JUMP_LAND_R] = 0;
	animDist[BIG_JUMP_R] = (3.0 / 10.0f) * 32.0f;
	animDist[SLOW_SPIN_R] = 0;
	animDist[FAST_SPIN_R] = 0.2f;
	animDist[ENFUNDAR_R] = 0;
	animDist[DESENFUNDAR_R] = 0;
	animDist[ATACK_R] = 0;
	animDist[DEFEND_R] = 0;
	animDist[STEP_R] = 24 / 7;
	animDist[DRINK_R] = 0;
	animDist[CLIMB_R] = 0;
	animDist[DOOR_R] = 0;
	animDist[VERTICAL_JUMP_UP_S_R] = 0;
	animDist[VERTICAL_JUMP_UP_J_R] = 0;
	animDist[VERTICAL_JUMP_DW_R] = 0;
	animDist[DIE_F_R] = 0;
	animDist[DIE_P_R] = 0;
	animDist[DIE_C_R] = 0;
	animDist[FALL_R] = 0;

	animDist[ENFUNDAR_R] = 0;
	animDist[DESENFUNDAR_R] = 0;
	animDist[ATACK_R] = 0;
	animDist[DEFEND_R] = 0;
	animDist[SW_PAUSE_R] = 0;
	animDist[WALK_SWORD_R] = 4;
	animDist[RETREAT_SWORD_R] = -4;
	animDist[DEAD_F_R] = 0;

	jumpDist = 7 / 4;
}

