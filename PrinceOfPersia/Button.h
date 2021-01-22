#ifndef _BUTTON_INCLUDE
#define _BUTTON_INCLUDE

#include <glm/glm.hpp>
#pragma once

#define NUM_PORTS 5;

class Button
{

public:
	Button();
	~Button();
	void freeButtons();

	void init(int level);
	bool update(glm::vec2 posPlayer);

	void presButton(int X, int Y);
	bool getType(int X, int Y);
	bool getOpener(int X, int Y);
	glm::vec2 getActivador(int X, int Y);

private:
	int getButton(int X, int Y);
	void addButton(int pos, bool opener, bool type, glm::vec2 CoordButton, glm::vec2 CoordActivador);

private:
	int numButtons;
	glm::vec2 *Botons;
	glm::vec2 *Activadores;
	bool *types;//true: portal | false: sprite
	bool *pressed,*opener;
};
#endif	// _BUTTON_INCLUDE

