#include "Button.h"
#include"Game.h"
#include <iostream>
using namespace std;
Button::Button()
{
}


Button::~Button()
{
}

void Button::freeButtons() {
	free(types);
	free(pressed);
	free(opener);
	free(Botons);
	free(Activadores);
}


void Button::init(int level) {
	if (level == 1)		numButtons = 17;
	else if (level == 2)numButtons = 21;

	types = new bool[numButtons];
	pressed = new bool[numButtons];
	opener = new bool[numButtons];
	Botons = new glm::vec2[numButtons];
	Activadores = new glm::vec2[numButtons];

	int i = 0;
	if (level == 1){
		addButton(i++, true, true, glm::vec2(68, 0), glm::vec2(65, 0));	//portal 0 to sword
		addButton(i++, true, true, glm::vec2(4, 6), glm::vec2(3, 6));	//portal 1 return to first Room
		addButton(i++, true, true, glm::vec2(39, 5), glm::vec2(35, 5));	//portal 2 go to final
		addButton(i++, true, true, glm::vec2(33, 1), glm::vec2(35, 2)); //portal 3 skip first Enemy
		addButton(i++, true, true, glm::vec2(6, 3), glm::vec2(7, 3));   //portal 4 go to poti
		
		addButton(i++, true, true, glm::vec2(86, 4), glm::vec2(87, 4)); //portal 5 fall die
		addButton(i++, true, false, glm::vec2(80, 3), glm::vec2(83, 4));//puerta final
		addButton(i++, true, false, glm::vec2(52, 3), glm::vec2(49, 3));//gate 1
		addButton(i++, true, false, glm::vec2(46, 3), glm::vec2(49, 3));//gate 1
		addButton(i++, true, false, glm::vec2(47, 4), glm::vec2(39, 3));//gate 2
		
		addButton(i++, true, false, glm::vec2(32, 3), glm::vec2(39, 3));//gate 2
		addButton(i++, true, false, glm::vec2(28, 4), glm::vec2(19, 3));//gate 3
		addButton(i++, true, false, glm::vec2(13, 3), glm::vec2(19, 3));//gate 3
		addButton(i++, false, false, glm::vec2(24, 4), glm::vec2(19, 3));//gate 3 close
		addButton(i++, true, false, glm::vec2(54, 0), glm::vec2(55, 0));//gate 4
		
		addButton(i++, true, false, glm::vec2(52, 0), glm::vec2(59, 0));//gate 5
		addButton(i++, true, false, glm::vec2(56, 0), glm::vec2(59, 0));//gate 5
	}
	else if (level == 2){
		addButton(i++, true, true, glm::vec2(86, 3), glm::vec2(84, 4)); //portal 0 go to sword
		addButton(i++, true, true, glm::vec2(38, 9), glm::vec2(37, 9)); //portal 1 return to sword
		addButton(i++, true, true, glm::vec2(34, 8), glm::vec2(36, 7)); //portal 2 go to skip boss
		addButton(i++, true, true, glm::vec2(19, 10), glm::vec2(16, 10));//portal 3 return to skip boss
		addButton(i++, true, true, glm::vec2(58, 6), glm::vec2(57, 6)); //portal 4 fall die
		
		addButton(i++, true, false, glm::vec2(9, 7), glm::vec2(5, 7));  //puerta final
		addButton(i++, true, false, glm::vec2(62, 8), glm::vec2(68, 8));//gate 1
		addButton(i++, true, false, glm::vec2(69, 8), glm::vec2(68, 8));//gate 1
		addButton(i++, true, false, glm::vec2(70, 6), glm::vec2(68, 6));//gate 2
		addButton(i++, true, false, glm::vec2(75, 5), glm::vec2(68, 4));//gate 3
		
		addButton(i++, true, false, glm::vec2(79, 4), glm::vec2(81, 5));//gate 4
		addButton(i++, true, false, glm::vec2(86, 4), glm::vec2(81, 3));//gate 5
		addButton(i++, true, false, glm::vec2(88, 4), glm::vec2(81, 3));//gate 5
		addButton(i++, true, false, glm::vec2(62, 4), glm::vec2(60, 4));//gate 6
		addButton(i++, true, false, glm::vec2(51, 3), glm::vec2(60, 6));//gate 7
		
		addButton(i++, true, false, glm::vec2(53, 2), glm::vec2(54, 5));//gate 8
		addButton(i++, true, false, glm::vec2(54, 7), glm::vec2(50, 7));//gate 9
		addButton(i++, true, false, glm::vec2(48, 7), glm::vec2(50, 7));//gate 9
		addButton(i++, true, false, glm::vec2(56, 8), glm::vec2(57, 8));//gate 10
		addButton(i++, true, false, glm::vec2(59, 8), glm::vec2(57, 8));//gate 10

		addButton(i++, true, false, glm::vec2(24, 7), glm::vec2(10, 7));//gate 11
	}

	for (int i = 0; i < numButtons; i++) {
		Game::instance().addStaticSprite(Botons[i].x, Botons[i].y, BOTO);
	}
}

void Button::addButton(int pos, bool actionOpen, bool type, glm::vec2 CoordButton, glm::vec2 CoordActivador){
	pressed[pos] = false;
	opener[pos] = actionOpen;
	types[pos] = type;
	Botons[pos] = CoordButton;
	Activadores[pos] = CoordActivador;
}

int Button::getButton(int X, int Y) {
	for (int i = 0; i < numButtons; i++) {
		if (Botons[i].x == X && Botons[i].y == Y) {
			return i;
		}
	}
	return -1;
}

glm::vec2 Button::getActivador(int buttonX, int buttonY) {
	int i = getButton(buttonX, buttonY);
	if (i != -1) return Activadores[i];
	return glm::vec2(-1);
}

bool Button::getType(int buttonX, int buttonY) {
	int i = getButton(buttonX, buttonY);
	if (i != -1) return types[i];
	else return false;//caso imposible??
}

bool Button::getOpener(int buttonX, int buttonY) {
	int i = getButton(buttonX, buttonY);
	if (i != -1) return opener[i];
	return false;//caso imposible??
}

bool Button::update(glm::vec2 posPlayer){//true si nuevo boton activado
	bool ret = false;
	for (int i = 0; i < numButtons; ++i){
		if (Botons[i].x == posPlayer.x && Botons[i].y == posPlayer.y) {
			Game::instance().changeSSAnimation(false, Botons[i].x, Botons[i].y);
			if (!pressed[i]) Game::instance().play(BUTTON_OPEN);
			pressed[i] = true;
			ret = true;
		}
		else {
			Game::instance().changeSSAnimation(true, Botons[i].x, Botons[i].y);
			if (pressed[i]) Game::instance().play(BUTTON_OPEN);
			pressed[i] = false;
		}
	}
	return ret;
}