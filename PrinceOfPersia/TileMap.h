#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

enum TileMapType { FRONT_TILEMAP, BACK_TILEMAP };

enum TypeTile {
	NOTHING, BLACK, WINDOW, BRICK1, BRICK2, 
	BLOCK1, BLOCK2, BLOCK3, BLOCK4, 
	BLOCK5, FT_COLUM, FLOOR, FLOOR_BRICK,
	SKULL, PINCHOS, BROKEN_FLOOR, TORCH,
	BK_COLUM, CORNER, BK_REJA, FT_REJA,
	CATIFA, FT_DBCOL, FT_UBCOL, BK_UBCOL,
	BK_DBCOL
};

class TileMap
{
	

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const TileMapType type, const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Texture &tileSheet);

	TileMap(const TileMapType type, const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Texture &tileSheet);
	~TileMap();

	void render(ShaderProgram &program, glm::vec2 traslate) const;
	void freeVBO();
	void freeMap();
	
	glm::vec2 getTileSize() const { return tileSize; }
	glm::vec2 getMapSize() const { return mapSize; }

	bool collisionMoveLeft	(glm::ivec2 pos, const glm::ivec2 &size) const;
	bool collisionMoveRight	(glm::ivec2 pos, const glm::ivec2 &size) const;
	bool collisionMoveDown	(glm::ivec2 pos, const glm::ivec2 &size) const;
	bool collisionMoveUp	(glm::ivec2 pos, const glm::ivec2 &size) const;
	bool inJumpeableTile(glm::ivec2 pos, bool left);
	void loadSSprites();
	int getTileType(int X, int Y);

private:
	bool loadLevel(const TileMapType type, const string &levelFile, Texture &tileSheet);
	bool collisionInTileH(int x, int y, const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionInTileV(int x, int y, const glm::ivec2 &pos, const glm::ivec2 &size) const;
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, tilesheetSize;
	glm::ivec2 tileSize, mapSize;
	glm::ivec2  Perspective;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	int blockSize = 128;
	int Type;
};


#endif // _TILE_MAP_INCLUDE


