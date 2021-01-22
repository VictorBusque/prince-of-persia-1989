#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "StaticSprite.h"
#include "Interface.h"
#include <GL/glut.h>


#define SCREEN_X 0
#define SCREEN_Y 0

#define SIZE_BOX_LEVEL_X 10*32
#define SIZE_BOX_LEVEL_Y 3*64

Scene::Scene()
{
	mapBack = NULL;
	mapFront = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if (mapBack != NULL)
		delete mapBack; 
	if (mapFront != NULL)
		delete mapFront;
	if(player != NULL)
		delete player;
}

void Scene::changeBoxLevel(int X, int Y){
	BoxLevel = glm::ivec2(X, Y);
}

void Scene::init()
{
	initShaders();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	SpriteSheet.loadFromFile("images/EnvironSpriteOK.png", TEXTURE_PIXEL_FORMAT_RGBA);
	EnemySheet.loadFromFile("images/Enemy1SpriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	BossSheet.loadFromFile("images/Enemy2SpriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	player = new Player();
	portals = new Portal();
	buttons = new Button();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	interf = Interface::createInterface(simpleProgram, texProgram);
	x = y = 0;
	first_time_ini = true;
}

void Scene::start(int lvl){
	loadLevel(lvl);
	currentTime = 0.0f;
}

void Scene::loadLevel(int level){
	//solo 1 vez por nivel
	if (!first_time_ini){
		mapBack->freeVBO();
		mapFront->freeVBO();
		mapBack->freeMap();
		mapFront->freeMap();
	}
	if (level == 1){
		mapBack = TileMap::createTileMap(BACK_TILEMAP, "levels/level1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, TileSheetB);
		mapFront = TileMap::createTileMap(FRONT_TILEMAP, "levels/level1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, TileSheetF);

		changeBoxLevel(6, 0);
		ini_player_x = BoxLevel.x*SIZE_BOX_LEVEL_X + 1 * 32;
		ini_player_y = BoxLevel.y*SIZE_BOX_LEVEL_Y + 1 * 64;
	}
	else if (level == 2){
		mapBack = TileMap::createTileMap(BACK_TILEMAP, "levels/level2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, TileSheetB);
		mapFront = TileMap::createTileMap(FRONT_TILEMAP, "levels/level2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, TileSheetF);

		changeBoxLevel(0, 2);
		ini_player_x = BoxLevel.x*SIZE_BOX_LEVEL_X + 6 * 32;
		ini_player_y = BoxLevel.y*SIZE_BOX_LEVEL_Y + 1 * 64;
		
	}
	mapSize = mapBack->getMapSize(); //l1= 90,9 l2=90 15
	numRooms = glm::ivec2(mapSize.x / 10, mapSize.y / 3); //l1= 9,3 l2 = 90,5
	player->setTileMap(mapBack);

	restart(level);
}

void Scene::restart(int lvl){
	player->restart();
	player->setPosition(glm::vec2(ini_player_x, ini_player_y));

	for (StaticSprite *s : ssvector)s->free();
	ssvector.clear();
	free(cmap);
	cmap = new int[mapSize.x*mapSize.y];
	for (int i = 0; i < mapSize.x*mapSize.y; ++i) cmap[i] = -1;

	mapBack->loadSSprites();
	setStaticSprites(lvl);

	if (!first_time_ini){
		portals->freePortals();
		buttons->freeButtons();
		first_time_ini = false;
	}
	portals->init(lvl);
	buttons->init(lvl);

	for (Enemy *e : enemies)e->free();
	enemies.clear();
	initializeEnemies(lvl);

	pause = false;
	if (lvl == 1) ssvector[cmap[0 * mapSize.x + 59]]->start(CLOSING);
	else if (lvl == 2) ssvector[cmap[8 * mapSize.x + 64]]->start(CLOSING);
}

void Scene::updateSceneBox(){
	glm::vec2 posPlayer = player->getPosition();
	posPlayer.x += 5;
	posPlayer.y += 10;
	int playerX = int(posPlayer.x / (SIZE_BOX_LEVEL_X));
	int playerY = int(posPlayer.y / (SIZE_BOX_LEVEL_Y));
	if (playerX != BoxLevel.x) changeBoxLevel(playerX, BoxLevel.y);
	if (playerY != BoxLevel.y) changeBoxLevel(BoxLevel.x, playerY);
}

bool Scene::updatePlayer(){
	return !(Game::instance().getKey('a') || Game::instance().getKey('s') || Game::instance().getKey('d') || Game::instance().getKey('w'));
}

void Scene::updateSSprite(int deltaTime){
	for (StaticSprite *s : ssvector){
		bool colision_suelo = false;
		if (s->getType() == SUELO) {
			glm::vec2 pos = s->getPosition();
			pos.x += 10;//perespectiva
			colision_suelo = mapFront->collisionMoveDown(pos, glm::ivec2(32, 64));
			//si jugador saltando vertical si actualizar
			if ((player->collisionDown() && !player->jumping()) || s->isBrocken() || player->getAnimation() == VERTICAL_JUMP_UP_J_L || player->getAnimation() == VERTICAL_JUMP_UP_J_R)
				s->update(deltaTime, player->getPosition(), colision_suelo);
		}
		else s->update(deltaTime, player->getPosition(), colision_suelo);
	}
}

void Scene::update(int deltaTime)
{
	if (Game::instance().getKeyPressed('p')) {
		if (pause) write("");
		else write("PAUSED");
		pause = !pause;
	}
	interf->update(deltaTime, currentTime);
	if (!pause){
		if ((currentTime / 1000.f) < MAX_TIME_SEG){
			currentTime += deltaTime;
			if (updatePlayer()) player->update(deltaTime);
			updateSceneBox();
			updateSSprite(deltaTime);
			for (int i = 0; i < enemies.size(); i++) enemies[i]->update(deltaTime, player->getPosition(), player, player->getAnimation());
			checkButton();
		}
		else Game::instance().timeOut();
	}
}

void Scene::checkButton(){
	if (player->collisionDown()&& !player->jumping()){
		glm::ivec2 posP = player->getPosition();
		posP.x /= 32;
		posP.y /= 64;
		if (buttons->update(posP)){
			glm::vec2 ac = buttons->getActivador(posP.x, posP.y);
			if (buttons->getOpener(posP.x, posP.y)){//activar
				if (buttons->getType(posP.x, posP.y))//portal
					portals->activatePortal(ac.x, ac.y);
				else changeSSAnimation(true, ac.x, ac.y);
			}
			else{//desactivar
				if (buttons->getType(posP.x, posP.y))//portal
					portals->deactivatePortal(ac.x, ac.y);
				else changeSSAnimation(false, ac.x, ac.y);
			}
		}
	}
}

void Scene::cheat(){
	if (Game::instance().getKey('a')) { if (0 < x) x -= 0.01f; }
	else if (Game::instance().getKey('d')) { if (x < numRooms.x - 1) x += 0.01f; }
	else if (Game::instance().getKey('w')) { if (0 < y) y -= 0.01f; }
	else if (Game::instance().getKey('s')) { if (y < numRooms.y - 1) y += 0.01f; }
	else { x = float(BoxLevel.x); y = float(BoxLevel.y); }
}

void Scene::render()
{
	cheat();
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec2 translate = glm::vec2(SIZE_BOX_LEVEL_X*x, SIZE_BOX_LEVEL_Y*y);

	mapBack->render(texProgram, translate);
	for (StaticSprite *s : ssvector) s->render(translate);
	if (!Game::instance().getWin()) player->render(translate);
	for (int i = 0; i < enemies.size(); i++) enemies[i]->render(translate);
	mapFront->render(texProgram, translate);
	interf->render(projection, player->getMaxHealth(), player->getHealth(), ! player->inCombatMode());
	for (int i = 0; i < enemies.size(); i++) if (enemies[i]->sameRoom(player->getPosition())) interf->renderLifeEnemy(projection, enemies[i]->getMaxHealth(), enemies[i]->getHealth());
}

void Scene::write(string s){
	interf->write(s);
}

void Scene::writeWin(bool win){
	interf->win(win);
}

void Scene::writeLose(bool lose){
	interf->lose(lose);
}

void Scene::changeSSAnimation(bool open, int X, int Y) {
	int index = cmap[Y * mapSize.x + X];
	if (index != -1){
		StaticSprite *s = ssvector[index];
		if (open) s->open();
		else s->close();
	}
}

Enemy* Scene::getEnemy(int i) {
	return enemies[i];
}

void Scene::addStaticSprite(int X, int Y, SSType sst){
	glm::ivec2 tileSize = mapBack->getTileSize();
	int tile_x = X * tileSize.x;
	int tile_y = Y * tileSize.y;
	cmap[Y * mapSize.x + X] = ssvector.size();

	StaticSprite *s = new StaticSprite();
	switch (sst)
	{
	case FIRE:
		s->init(sst, glm::vec2(tile_x, tile_y), texProgram, SpriteSheet);
		s->setAnimLR(INI, 8, 0, 4, 0);
		s->start(INI);
		break;
	case POTI:
		s->init(sst, glm::vec2(tile_x + 32, tile_y - 6), texProgram, SpriteSheet);
		s->setAnimLR(INI, 8, 0, 10, 1);
		s->start(INI);
		break;
	case REJA:
		s->init(sst, glm::vec2(tile_x, tile_y), texProgram, SpriteSheet);
		s->setAnimLR(CLOSE, 8, 0, 1, 2);
		s->setAnimLR(OPENING, 8, 1, 15, 2);
		s->setAnimLR(OPEN, 8, 15, 16, 2);
		s->setAnimRL(CLOSING, 8, 14, 0, 2);
		s->start(CLOSE);
		break;
	case DOOR:
		s->init(sst, glm::vec2(tile_x + 32, tile_y - 16), texProgram, SpriteSheet);
		s->setAnimLR(CLOSE, 8, 0, 1, 3);
		s->setAnimLR(OPENING, 8, 1, 15, 3);
		s->setAnimLR(OPEN, 8, 15, 16, 3);
		s->setAnimRL(CLOSING, 8, 14, 0, 3);
		s->start(CLOSE);
		break;
	case PORTAL_B:
		s->init(sst, glm::vec2(tile_x, tile_y), texProgram, SpriteSheet);
		s->setAnimLR(CLOSE, 8, 5, 6, 7);
		s->setAnimLR(OPENING, 6, 0, 5, 6);
		s->setAnimLR(OPEN, 5, 0, 4, 7);
		s->setAnimRL(CLOSING, 6, 5, 0, 6);
		s->start(CLOSE);
		break;
	case PORTAL_R:
		s->init(sst, glm::vec2(tile_x, tile_y), texProgram, SpriteSheet);
		s->setAnimLR(CLOSE, 8, 5, 6, 9);
		s->setAnimLR(OPENING, 6, 0, 5, 8);
		s->setAnimLR(OPEN, 5, 0, 4, 9);
		s->setAnimRL(CLOSING, 6, 5, 0, 8);
		s->start(CLOSE);
		break;
	case PINCHO:
		s->init(sst, glm::vec2(tile_x, tile_y), texProgram, SpriteSheet);
		s->setAnimLR(CLOSE, 8, 0, 1, 4);
		s->setAnimLR(OPENING, 8, 1, 3, 4);
		s->setAnimLR(OPEN, 8, 3, 4, 4);
		s->setAnimRL(CLOSING, 16, 2, 0, 4);
		s->start(CLOSE);
		break;
	case SWORD:
		s->init(sst, glm::vec2(tile_x + 32, tile_y - 6), texProgram, SpriteSheet);
		s->setAnimLR(INI, 6, 0, 6, 5);
		s->start(INI);
		break;
	case SUELO:
		int line;
		if (Game::instance().getActualLevel() == 1) line = 10;
		else line = 11;
		s->init(sst, glm::vec2(tile_x, tile_y), texProgram, SpriteSheet);
		s->setAnimLR(INI, 8, 0, 1, line);
		s->setAnimLR(CLOSE, 8, 3, 4, line);
		s->start(INI);
		break;
	case BOTO:
		int lineb;
		if (Game::instance().getActualLevel() == 1) lineb = 10;
		else lineb = 11;
		s->init(sst, glm::vec2(tile_x, tile_y), texProgram, SpriteSheet);
		s->setAnimLR(CLOSE, 8, 0, 1, lineb);
		s->setAnimLR(CLOSING, 8, 1, 2, lineb);
		s->setAnimLR(OPENING, 8, 1, 2, lineb);
		s->setAnimLR(OPEN, 8, 2, 3, lineb);
		s->start(OPEN);
		break;
	default:
		break;
	}
	ssvector.push_back(s);
}

bool Scene::colisionStaticSprite(int x, int y, glm::ivec2 pos, glm::ivec2 size){
	int index = cmap[y * mapSize.x + x];
	if (index != -1) return ssvector[index]->colision(x, y, pos, size);
	else return false;
}

bool Scene::colisionEnemy(glm::ivec2 pos, glm::ivec2 size){
	for (int i = 0; i < enemies.size(); i++){
		if (enemies[i]->collisionable(pos, size)) return true;
	}
	return false;
}

bool Scene::isSSColisionable(int x, int y){
	int index = cmap[y * mapSize.x + x];
	if (index <= -1) return false;
	else return ssvector[index]->isSSColisionable();
}

bool Scene::hasSSprite(SSType type, int X, int Y){
	int index = cmap[Y * mapSize.x + X];
	if (index == -1) return false;
	else return (ssvector[index]->getType() == type);
}

bool Scene::getPoti(bool left){
	glm::ivec2 pos = player->getPosition();
	pos.x /= 32;
	pos.y /= 64;
	if (left) pos.x -= 1;
	int index = cmap[pos.y * mapSize.x + pos.x];
	if (index != -1){
		StaticSprite *s = ssvector[index];
		if (s->getType() == POTI){
			s->erase();
			cmap[pos.y * mapSize.x + pos.x] = -1;
			return true;
		}
		else return false;
	}
	else return false;
}

bool Scene::getSword(bool left){
	glm::ivec2 pos = player->getPosition();
	pos.x /= 32;
	pos.y /= 64;
	pos.x -= 1;
	int index = cmap[pos.y * mapSize.x + pos.x];
	if (index != -1){
		StaticSprite *s = ssvector[index];
		if (s->getType() == SWORD){
			s->erase();
			cmap[pos.y * mapSize.x + pos.x] = -1;
			return true;
		}
		else return false;
	}
	else return false;
}

bool Scene::openedDoor(bool left){
	glm::ivec2 pos = player->getPosition();
	pos.x /= 32;
	pos.y /= 64;
	pos.x -= 1;
	int index = cmap[pos.y * mapSize.x + pos.x];
	if (index != -1){
		StaticSprite *s = ssvector[index];
		if (s->getType() == DOOR){
			return s->animation() == OPEN;
		}
		else return false;
	}
	else return false;
}

glm::vec2 Scene::openedPortal(bool left){
	glm::ivec2 pos = player->getPosition();
	pos.x -= 24;
	pos.x /= 32;
	pos.y /= 64;
	if (portals->isActive(pos.x, pos.y)) 
		return portals->getExit(pos.x, pos.y);
	else return glm::vec2(-1);
}

void Scene::closePortal(int X, int Y){
	portals->deactivatePortal(X,Y);
}

//inisalizers
void Scene::setStaticSprites(int level) {
	if (level == 1){
		addStaticSprite(1, 5, SWORD);
		if (Game::instance().getLevel() == LEVEL_EASY) {
			addStaticSprite(6, 0, POTI);
			addStaticSprite(22, 4, POTI);
			addStaticSprite(33, 2, POTI);
			addStaticSprite(35, 0, POTI);
			addStaticSprite(35, 6, POTI);
			addStaticSprite(53, 0, POTI);
			addStaticSprite(53, 2, POTI);
			addStaticSprite(72, 4, POTI);
		}
		else if (Game::instance().getLevel() == LEVEL_NORMAL) {
			//[6,0],[33,2],[35,6],[53,2]
			addStaticSprite(6, 0, POTI);
			addStaticSprite(33, 2, POTI);
			addStaticSprite(35, 6, POTI);
			addStaticSprite(53, 2, POTI);
		}
		//suelos
		addStaticSprite(11, 3, SUELO);
		addStaticSprite(12, 3, SUELO);
		addStaticSprite(18, 2, SUELO);
		addStaticSprite(25, 4, SUELO);
		addStaticSprite(33, 5, SUELO);
		addStaticSprite(35, 1, SUELO);
		addStaticSprite(35, 3, SUELO);
		addStaticSprite(36, 1, SUELO);
		addStaticSprite(36, 3, SUELO);
		addStaticSprite(43, 4, SUELO);
		addStaticSprite(66, 2, SUELO);
		addStaticSprite(55, 2, SUELO);
	}
	else if (level == 2){
		addStaticSprite(35, 9, SWORD);
		if (Game::instance().getLevel() == LEVEL_EASY) {
			addStaticSprite(55, 2, POTI);
			addStaticSprite(56, 2, POTI);
			addStaticSprite(83, 3, POTI);
			addStaticSprite(67, 6, POTI);
			addStaticSprite(52, 4, POTI);
		}
		else if (Game::instance().getLevel() == LEVEL_NORMAL) {
			addStaticSprite(55, 2, POTI);
			addStaticSprite(83, 3, POTI);
			addStaticSprite(67, 6, POTI);
			addStaticSprite(52, 4, POTI);
		}
		addStaticSprite(27, 7, SUELO);
		addStaticSprite(51, 2, SUELO);
		addStaticSprite(53, 3, SUELO);
	}
}

void Scene::initializeEnemies(int level) {
	if (level == 1){
		switch (Game::instance().getLevel()) {
		case LEVEL_EASY:
			pushEnemy(false, 7, 8, 1, 1);
			break;
		case LEVEL_NORMAL:
			pushEnemy(false, 4, 8, 0, 0);
			pushEnemy(true, 7, 8, 1, 1);
			break;
		case LEVEL_HARD:
			pushEnemy(false, 4, 7, 0, 0);
			pushEnemy(false, 1, 4, 2, 1);
			pushEnemy(false, 2, 8, 1, 1);
			pushEnemy(true, 7, 8, 1, 1);
			break;
		}
	}
	else if (level == 2){
		switch (Game::instance().getLevel()) {
		case LEVEL_EASY:
			pushEnemy(false, 2, 2, 2, 1);
			break;
		case LEVEL_NORMAL:
			pushEnemy(false, 4, 1, 2, 1);
			pushEnemy(true, 2, 2, 2, 1);
			break;
		case LEVEL_HARD:
			pushEnemy(false, 2, 2, 2, 1);
			pushEnemy(false, 1, 4, 2, 1);
			pushEnemy(false, 4, 2, 2, 1);
			pushEnemy(true, 7, 8, 1, 1);
			break;
		}
	}
}

void Scene::pushEnemy(bool typeEnemy, int boxX, int desX, int boxY, int desY){
	Enemy *e;
	if (typeEnemy)e = new Enemy(typeEnemy,BossSheet);
	else e = new Enemy(typeEnemy,EnemySheet);
	e->setTileMap(mapFront);
	e->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::ivec2(boxX * SIZE_BOX_LEVEL_X + desX * mapBack->getTileSize().x, boxY*SIZE_BOX_LEVEL_Y + desY * mapBack->getTileSize().y));
	enemies.push_back(e);
}

void Scene::initShaders()
{
	Shader vShader, fShader;
	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if (!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}