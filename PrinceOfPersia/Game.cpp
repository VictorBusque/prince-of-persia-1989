#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include <iostream>

//interacion game
void Game::init()
{
	restart = loadLv = player_Dead = win = false;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //color de fondo NEGRO
	
	state = INTRO;
	play(INTRO_THEME);
	intro.init();
	scene.init();
}

void Game::needRestartScene(){
	if (restart){
		stopThemeSong();
		scene.restart(lvl);
		sound.PlayThemeSong();
		restart = false;
		scene.write("");
		scene.writeWin(false);
		scene.writeLose(false);
	}
	else if (getSpecialKeyPressed(GLUT_KEY_F5)){
		restart = true;
		scene.write("RESTARTING...");
	}
	else if (player_Dead && getKeyPressed(' ')){
		player_Dead = false;
		restart = true;
		scene.writeLose(false);
		scene.write("RESTARTING...");
	}
}
void Game::needLoadLevelScene(){
	if (loadLv){
		stopThemeSong();
		scene.loadLevel(lvl);
		sound.PlayThemeSong();
		loadLv = false;
		scene.write("");
		scene.writeWin(false);
		scene.writeLose(false);
	}
	else if (getSpecialKeyPressed(GLUT_KEY_F1) && lvl != 1){
		loadLv = true;
		lvl = 1;
		scene.write("Loading Level 1 ...");
	}
	else if (getSpecialKeyPressed(GLUT_KEY_F2) && lvl != 2){
		loadLv = true;
		lvl = 2;
		scene.write("Loading Level 2 ...");
	}
}

bool Game::update(int deltaTime)
{
	if (state == INTRO){
		intro.update(deltaTime); 
		return !getKeyPressed(GL_KEY_ESC);//exit
	}
	else if (state == PLAYING){
		if (!win){
			needRestartScene();
			needLoadLevelScene();
			scene.update(deltaTime);
		}
		else if (getKeyPressed(' ')){
			win = false;
			goToMenu();
		}
		if (getKeyPressed(GL_KEY_ESC)) goToMenu();
	}
	return true;
}

void Game::timeOut(){ //rreaprofitar codi
	scene.write("Press 'SPACE' to go to Menu");
	win = true;
}
void Game::nextLevel(){
	if (lvl == 1){
		lvl = 2;
		loadLv = true;
		scene.write("Loading Level 2 ...");
	}
	else if (lvl == 2) {
		scene.write("Press 'SPACE' to go to Menu");
		scene.writeWin(true);
		sound.PlayWinSong();
		win = true;
	}
}
void Game::playerDead(){
	scene.write("Press 'SPACE' to start the level again");
	player_Dead = true;
	scene.writeLose(true);
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (state == INTRO) intro.render();
	else if (state == PLAYING) scene.render();
}

void Game::setLevel(int level) {
	this->level = level;
}
int Game::getLevel() {
	return level;
}

bool Game::getWin(){
	return win;
}
int Game::getActualLevel(){
	return lvl;
}

void Game::goToMenu(){
	stopThemeSong(); 
	sound.StopWinSong();
	scene.write("");
	scene.writeWin(false);
	scene.writeLose(false);
	state = INTRO;
	play(INTRO_THEME);
}
void Game::startGame(){
	state = PLAYING;
	lvl = 1;
	scene.start(lvl);//include load an reset of clock
	sound.PlayThemeSong(); //pause Intro theme??
}

//INPUT CALLBACK METHODS
//--Set Key
void Game::keyPressed(int key)
{
	keys[key] = true;
}
void Game::keyReleased(int key)
{
	keys[key] = false;
}
void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}
void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}
void Game::modifiersPressed(int mKey){
	modifKeys = mKey;
}
//--Get Key
bool Game::getKey(int key) const
{
	return keys[key];
}
bool Game::getKeyPressed(int key)
{
	if (keys[key]){
		if (!keysPressed[key]){
			keysPressed[key] = true;
			return true;
		}
		else return false;
	}
	else{
		keysPressed[key] = false;
		return false;
	}
}
bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}
bool Game::getSpecialKeyPressed(int key)
{
	if (specialKeys[key]){
		if (!specialKeysPressed[key]){
			specialKeysPressed[key] = true;
			return true;
		}
		else return false;
	}
	else{
		specialKeysPressed[key] = false;
		return false;
	}
}
bool Game::getModifierKey(int key) const
{
	return (modifKeys&key)!=0;
}
bool Game::getModifierKeyPressed(int key)
{
	if (getModifierKey(key)){
		if (!modifierKeysPressed[key]){
			modifierKeysPressed[key] = true;
			return true;
		}
		else return false;
	}
	else{
		modifierKeysPressed[key] = false;
		return false;
	}
}
//--Mouse
void Game::mouseMove(int x, int y)
{
}
void Game::mousePress(int button)
{
}
void Game::mouseRelease(int button)
{
}

//COMUNICATION WITH SCENE
//--collisions
bool Game::isSSColisionable(int x, int y){
	return scene.isSSColisionable(x, y);
}
bool Game::colisionStaticSprite(int x, int y, glm::ivec2 pos, glm::ivec2 size){
	return scene.colisionStaticSprite(x, y, pos, size);
}
bool Game::colisionEnemy(glm::ivec2 pos, glm::ivec2 size){
	return scene.colisionEnemy(pos, size);
}
//--modificar Static Sprite
void Game::addStaticSprite(int X, int Y, SSType sst){
	scene.addStaticSprite(X, Y, sst);
}
void Game::changeSSAnimation(bool open, int X, int Y) {
	scene.changeSSAnimation(open, X, Y);
}
void Game::closePortal(int X, int Y){
	scene.closePortal(X, Y);
}
//--consulta Static Sprite
bool Game::hasSSprite(SSType type, int X, int Y){
	return scene.hasSSprite(type, X, Y);
}
bool Game::getPoti(bool left){
	return scene.getPoti(left);
}
bool Game::getSword(bool left){
	return scene.getSword(left);
}
bool Game::openedDoor(bool left){
	return scene.openedDoor(left);
}
glm::vec2 Game::openedPortal(bool left){
	return scene.openedPortal(left);
}
//--enemys
int	 Game::getEnemyAtRange(glm::vec2 pPos){
	int numEnemies;
	switch (level) {
	case LEVEL_EASY: numEnemies = NUM_ENEMIES_EASY; break;
	case LEVEL_NORMAL: numEnemies = NUM_ENEMIES_NORMAL; break;
	case LEVEL_HARD: numEnemies = NUM_ENEMIES_HARD; break;
	}
	for (int i = 0; i < numEnemies; i++) {
		if (scene.getEnemy(i)->sameRoom(pPos)) return i;
	}
	return -1;
}
Enemy* Game::getEnemy(int i) {
	return scene.getEnemy(i);
}

//SOUND
void Game::play(FX song){
	sound.PlayFX(song);
}
void Game::stopThemeSong() {
	sound.StopThemeSong();
}
