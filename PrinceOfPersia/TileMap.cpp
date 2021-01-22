#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Game.h"
#include "Scene.h"


using namespace std;

TileMap *TileMap::createTileMap(const TileMapType type, const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Texture &tileSheet)
{
	TileMap *map = new TileMap(type, levelFile, minCoords, program, tileSheet);
	return map;
}


TileMap::TileMap(const TileMapType type, const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program, Texture &tileSheet)
{
	Perspective = glm::ivec2(10, -8);
	loadLevel(type, levelFile, tileSheet);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}

void TileMap::freeMap(){
	free(map);
}

void TileMap::freeVBO()
{
	glDeleteBuffers(1, &vbo);
}

void TileMap::render(ShaderProgram &program, glm::vec2 traslate) const
{
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(-traslate.x, -traslate.y, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	program.setUniform2f("texCoordDispl", 0.f, 0.f);
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

bool TileMap::loadLevel(const TileMapType type, const string &levelFile, Texture &tileSheet)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	string tile;
	Type = type;

	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;

	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize.x >> tileSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j<mapSize.y; j++)
	{
		getline(fin, line);
		int posDotAnt = 0;
		for (int i = 0; i<mapSize.x; i++)
		{
			int pos = line.find(",", posDotAnt);
			tile = line.substr(posDotAnt, pos - posDotAnt);
			posDotAnt = pos + 1;
			int tileTypeNum;
			if (tile.size() == 1) tileTypeNum = tile[0] - int('0');
			else tileTypeNum = (tile[0] - int('0')) * 10 + (tile[1] - int('0'));
			
			map[j*mapSize.x + i] = tileTypeNum;

		}
	}

	return true;
}

void TileMap::loadSSprites(){
	int tileTypeNum = 0;
	if (Type == BACK_TILEMAP){
		for (int j = 0; j<mapSize.y; j++) {
			for (int i = 0; i<mapSize.x; i++) {
				tileTypeNum = map[j * mapSize.x + i];
				if		(tileTypeNum == PINCHOS) Game::instance().addStaticSprite(i, j, PINCHO);
				else if (tileTypeNum == TORCH)	 Game::instance().addStaticSprite(i, j, FIRE);
				else if (tileTypeNum == BK_REJA) Game::instance().addStaticSprite(i, j, REJA);
				else if (tileTypeNum == FT_REJA) Game::instance().addStaticSprite(i, j, DOOR);
				//...
			}
		}
	}
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	//halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=mapSize.y-1; 0<=j; j--)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			switch (Type)
			{
			case BACK_TILEMAP:
				if (BLOCK1 <= tile && tile <= BLOCK5) tile = NOTHING;
				else if (tile == TORCH || tile == FT_REJA) tile = FLOOR;
				break;
			case FRONT_TILEMAP:
				if (tile == BK_COLUM) tile = FT_COLUM;
				else if (tile == BK_REJA) tile = FT_REJA;
				else if (tile == BK_DBCOL) tile = FT_DBCOL;
				else if (tile == BK_UBCOL) tile = FT_UBCOL;
				else if (tile == CORNER) tile = BLOCK2;
				else if (!(BLOCK1 <= tile && tile <= BLOCK5)) tile = NOTHING;
				break;
			}
			tile--;
			if(tile > 0)
			{ 
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize.x, minCoords.y + (j-1) * tileSize.y);
				texCoordTile[0] = glm::vec2(float(tile % tilesheetSize.x) / tilesheetSize.x, float(tile / tilesheetSize.y) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;

				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionInTileH(int x, int y, const glm::ivec2 &pos, const glm::ivec2 &size) const{
	int tileTypeNum = map[y*mapSize.x + x];
	if (tileTypeNum <= BRICK2) return false; // vacio
	else if ((BLOCK1 <= tileTypeNum && tileTypeNum <= BLOCK5) || tileTypeNum == CORNER || tileTypeNum == CATIFA) return true; //solido
	else{
		if (tileTypeNum == BK_REJA){
			return Game::instance().colisionStaticSprite(x, y, pos, size); //para la reja
		}
		else if (Type == BACK_TILEMAP) return Game::instance().colisionEnemy(pos, size);//para el enemigo
		else return false;
	}
}

bool TileMap::collisionInTileV(int x, int y, const glm::ivec2 &pos, const glm::ivec2 &size) const{
	int tileTypeNum = map[y*mapSize.x + x];
	int posYBase = pos.y + size.y; //puede estar en otro tile
	if (tileTypeNum <= BRICK2 || tileTypeNum == BK_UBCOL) {// vacio
		return Game::instance().colisionStaticSprite(x, y, pos, size); // caso suelo roto
	}
	else if ((BLOCK1 <= tileTypeNum && tileTypeNum <= BLOCK5) || tileTypeNum == CORNER) return true; //solido
	else {//suelo???
		int pixelTileBase = (y + 1)*tileSize.y;
		if (pos.y < pixelTileBase  && pixelTileBase < posYBase) return true;
		if (posYBase >(pixelTileBase + Perspective.y) && (pixelTileBase + Perspective.y)> pos.y) return true;
		else return false;
	}
}

bool TileMap::collisionMoveLeft(glm::ivec2 pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	int posPX = pos.x - Perspective.x;
	x = posPX / tileSize.x;
	y0 = pos.y / tileSize.y;
	y1 = (pos.y + size.y - 1) / tileSize.y;
	for(int y=y0; y<=y1; y++)
	{
		if (collisionInTileH(x, y, pos, size)) return true;
	}
	return false;
}

bool TileMap::collisionMoveRight(glm::ivec2 pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	int posPX = pos.x - Perspective.x;
	x = (posPX + size.x - 1) / tileSize.x;
	y0 = pos.y / tileSize.y;
	y1 = (pos.y + size.y - 1) / tileSize.y;
	for(int y=y0; y<=y1; y++)
	{
		if (collisionInTileH(x, y, pos, size)) return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(glm::ivec2 pos, const glm::ivec2 &size) const
{
	int x0, x1, y;
	pos.x -= Perspective.x;
	x0 = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y = (pos.y + size.y - 1) / tileSize.y;
	for(int x=x0; x<=x1; x++)
	{
		if (collisionInTileV(x, y, pos, size)) return true;
	}
	
	return false;
}

bool TileMap::collisionMoveUp(glm::ivec2 pos, const glm::ivec2 &size) const
{
	int x0, x1, y;
	pos.x -= Perspective.x;
	x0 = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y = pos.y / tileSize.y;
	for (int x = x0; x <= x1; x++)
	{
		if (collisionInTileV(x, y, pos, size)) return true;
	}

	return false;
}

bool TileMap::inJumpeableTile(glm::ivec2 pos, bool left) {
	int x = int(pos.x / 32);
	int y = int(pos.y / 64) + 1;

	int above = map[(y - 1) * mapSize.x + x];
	int above_right = map[(y - 1) * mapSize.x + x + 1];
	int above_left = map[(y - 1) * mapSize.x + x - 1];

	bool ab = above <= BRICK2;
	if(ab)  ab = !Game::instance().isSSColisionable(x, y - 1);

	if (left) { //suelo es 11->17 y "19"
		if (ab && ((FLOOR <= above_left && above_left <= BK_COLUM) || above_left == BK_DBCOL)) return true;
		if (ab && above_left == BK_REJA) return !Game::instance().isSSColisionable(x - 1, y - 1);
	}
	else {
		if (ab && ((FLOOR <= above_right && above_right <= BK_COLUM) || above_right == BK_DBCOL)) return true;
		if (ab && above_right == BK_REJA) return !Game::instance().isSSColisionable(x + 1, y - 1);
	}
	return false;
}

int TileMap::getTileType(int X, int Y) {
	int XT = X / 32;
	int YT = Y / 64;
	return map[YT * mapSize.x + XT];
}