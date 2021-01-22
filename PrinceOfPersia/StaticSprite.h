#ifndef _StaticSprite_INCLUDE
#define _StaticSprite_INCLUDE

#include "Sprite.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

enum SSAnims
{
	INI, OPENING, OPEN, CLOSING, CLOSE,
};
enum SSType
{
	FIRE, REJA, DOOR, SUELO, PINCHO, POTI, BOTO, SWORD, PORTAL_B, PORTAL_R
};
class StaticSprite
{

public:
	SSType getType();
	int animation();	
	bool isSSColisionable();
	bool colision(int x, int y, glm::ivec2 pos, glm::ivec2 size);
	void erase();
	bool isBrocken();

	void init(SSType sstype, const glm::ivec2 &posSp, ShaderProgram &shaderProgram, Texture &SpriteSheet);
	void update(int deltaTime, glm::ivec2 posPlayer, bool colision_suelo);
	void render(glm::vec2 q);
	void free();

	void setAnimLR(SSAnims A, int speed, int ini, int fi, int line);
	void setAnimRL(SSAnims A, int speed, int ini, int fi, int line);
	void open(); 
	void close();
	void start(SSAnims A);
	glm::vec2 getPosition();

private:
	Sprite *sprite;
	glm::ivec2 QuadSize, posSprite, iniPos;
	SSType sst;
	bool erased;
	bool Suelo_Roto;
	int t_acc;
};

#endif // _StaticSprite_INCLUDE


