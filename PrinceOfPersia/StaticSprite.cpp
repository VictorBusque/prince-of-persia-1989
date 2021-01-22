#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "StaticSprite.h"
#include "Game.h"

#define TIME_CLOSE_REJA  (30*1000)
#define TIME_BREAK_SUELO 550

void StaticSprite::free()
{
	sprite->free();
}

void StaticSprite::init(SSType sstype, const glm::ivec2 &posSp, ShaderProgram &shaderProgram, Texture &SpriteSheet)
{
	glm::vec2 T = glm::vec2(1.f / 16.f, 1.f / 12.f);
	QuadSize = glm::ivec2(64, 64);
	posSprite = iniPos = posSp;
	sst = sstype;
	Suelo_Roto = erased = false;
	t_acc = 0;
	sprite = Sprite::createSprite(QuadSize, T, &SpriteSheet, &shaderProgram);
	sprite->setPosition(posSp);
	sprite->setNumberAnimations(5);
}

void StaticSprite::setAnimLR(SSAnims A, int speed, int ini, int size, int line)
{
	sprite->setAnimationSpeed(A, speed);
	for (int i = ini; i < size; ++i) sprite->addKeyframe(A, glm::vec2(i, line));
}

void StaticSprite::setAnimRL(SSAnims A, int speed, int ini, int size, int line)
{
	sprite->setAnimationSpeed(A, speed);
	for (int i = ini; size < i; --i) sprite->addKeyframe(A, glm::vec2(i, line));
}

void StaticSprite::update(int deltaTime, glm::ivec2 posPlayer, bool colision_suelo)
{
	if (erased) return;
	sprite->update(deltaTime);
	//canvi de estat
	if (sprite->animation() == OPENING && sprite->isLastKeyframe()){
		sprite->changeAnimation(OPEN);
	}
	if (sprite->animation() == CLOSING && sprite->isLastKeyframe()){
		sprite->changeAnimation(CLOSE);
	}
	//si suelo desplazar hacia abajo???

	if (sst == REJA){
		t_acc += deltaTime;
		if (t_acc > TIME_CLOSE_REJA) close();
	}
	else if (sst == SUELO){
		if (sprite->animation() == INI){
			if (Suelo_Roto){
				if (!colision_suelo) {
					posSprite.y += 3;
					sprite->setPosition(posSprite);
				}
				else {
					sprite->changeAnimation(CLOSE);
					Game::instance().play(LOOSE_CRASH);
					posSprite.y = (posSprite.y / 64 + 1) * 64;
					posSprite.y -= 3;//calcular
					sprite->setPosition(posSprite);
				}
			}
			else if (posPlayer.x / 32 == posSprite.x / 32 && (posPlayer.y == posSprite.y + 60)){
				Suelo_Roto = true;
			}
			else if (posPlayer.x / 32 == posSprite.x / 32 && posPlayer.y / 64 == posSprite.y / 64){
				t_acc += deltaTime;
				if (t_acc > TIME_BREAK_SUELO) Suelo_Roto = true;
			}
		}
	}
	else if (sst == PINCHO){
		glm::vec2 dist = glm::vec2(posSprite.x / 32 - posPlayer.x / 32, posSprite.y / 64 - posPlayer.y / 64);
		dist.x = abs(dist.x);
		//dist.y = abs(dist.y);
		if (dist.x <= 1 && 0<=dist.y && dist.y <= 1) open();
		else close();
	}
	else if (sst == DOOR && sprite->animation() == OPENING) {
		Game::instance().play(GATE_OPEN);
	}
}

void StaticSprite::open()
{
	if (sprite->animation() == CLOSE){
		sprite->changeAnimation(OPENING);
		if (sst == PINCHO) Game::instance().play(SPIKES);
		//reproducir sonido??
	}
	if (sst == REJA)
		t_acc = 0;
}

void StaticSprite::close()
{
	if (sprite->animation() == OPEN)
		sprite->changeAnimation(CLOSING);
	if (sst == REJA)
		t_acc = 0;
}

void StaticSprite::render(glm::vec2 q)
{
	if (!erased) sprite->render(q);
}

void StaticSprite::start(SSAnims A)
{
	sprite->changeAnimation(A);
}

glm::vec2 StaticSprite::getPosition(){
	return posSprite;
}
SSType StaticSprite::getType(){
	return sst;
}

int StaticSprite::animation(){
	return sprite->animation();
}

void StaticSprite::erase(){
	erased = true;
}

bool StaticSprite::isBrocken(){
	return Suelo_Roto;
}

bool StaticSprite::isSSColisionable(){
	if (erased) return false;
	switch (sst)
	{
	case REJA:
		return sprite->animation() == CLOSE || sprite->animation() == OPENING;
		break;
	case SUELO:
		return  (sprite->animation() == INI);
		break;
	case PINCHO:
	case BOTO:
	case FIRE:
	case POTI:
	case DOOR:
	default:
		return false;
		break;
	}
}

bool StaticSprite::colision(int x, int y, glm::ivec2 pos, glm::ivec2 size){
	if (erased) return false;
	int posYBase = pos.y + size.y; //puede estar en otro tile
	int pixelTileBase = (y + 1) * 64;
	switch (sst)
	{
	case REJA:
		if (sprite->animation() == OPEN || sprite->animation() == CLOSING) return false;
		else {
			int posRej = posSprite.x + 32;
			return (pos.x <= posRej && (posRej - 8) <= (pos.x + size.x));
		}
		break;
	case SUELO:
		if (Suelo_Roto) return false;
		else{ //-8 es perespectiva.y
			if (pos.y < pixelTileBase  && pixelTileBase < posYBase) return true;
			if (pos.y < (pixelTileBase - 8) && (pixelTileBase - 8) < posYBase) return true;
			return false;
		}
		break;
	case PINCHO:
	case BOTO:
	case FIRE:
	case POTI:
	case DOOR:
	default:
		return false;
		break;
	}
}