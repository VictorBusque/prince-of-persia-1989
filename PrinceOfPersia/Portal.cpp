#include "Portal.h"
#include"Game.h"

Portal::Portal()
{
}


Portal::~Portal()
{
}

void Portal::freePortals(){
	free(active);
	free(Port1Pos);
	free(Port2Pos);
}

void Portal::init(int level) {
	if (level == 1)		numPortals = 6;
	else if (level == 2)numPortals = 5;

	active = new bool[numPortals];
	Port1Pos = new glm::vec2[numPortals];
	Port2Pos = new glm::vec2[numPortals];

	if (level == 1){
		active[0] = false; // to get sword
		Port1Pos[0] = glm::vec2(65, 0);
		Port2Pos[0] = glm::vec2(1, 4);

		active[1] = false; // return to first Room
		Port1Pos[1] = glm::vec2(3, 6);
		Port2Pos[1] = glm::vec2(63, 0);

		active[2] = false; // go to final
		Port1Pos[2] = glm::vec2(35, 5);
		Port2Pos[2] = glm::vec2(83, 3);

		active[3] = false; // skip first Enemy
		Port1Pos[3] = glm::vec2(35, 2);
		Port2Pos[3] = glm::vec2(58, 0);

		active[4] = false; // go to poti
		Port1Pos[4] = glm::vec2(7, 3);
		Port2Pos[4] = glm::vec2(8, 0);

		active[5] = false; // fall die
		Port1Pos[5] = glm::vec2(87, 4);
		Port2Pos[5] = glm::vec2(3, 0);
	}
	else if (level == 2){
		active[0] = false; // go to sword
		Port1Pos[0] = glm::vec2(84, 4);
		Port2Pos[0] = glm::vec2(36, 9);

		active[1] = false; // return to sword
		Port1Pos[1] = glm::vec2(37, 9);
		Port2Pos[1] = glm::vec2(76, 3);

		active[2] = false; // go to skip boss
		Port1Pos[2] = glm::vec2(36, 7);
		Port2Pos[2] = glm::vec2(26, 10);

		active[3] = false; // return to skip boss
		Port1Pos[3] = glm::vec2(16, 10);
		Port2Pos[3] = glm::vec2(8, 6);

		active[4] = false; // fall die
		Port1Pos[4] = glm::vec2(57, 6);
		Port2Pos[4] = glm::vec2(23, 10);
	}
	for (int i = 0; i < numPortals; i++) {
		Game::instance().addStaticSprite(Port1Pos[i].x, Port1Pos[i].y, PORTAL_B);
		Game::instance().addStaticSprite(Port2Pos[i].x, Port2Pos[i].y, PORTAL_R);
	}
}

int Portal::getPortal(int X, int Y, bool entry) {
	if (entry) for (int i = 0; i < numPortals; i++) { if (Port1Pos[i].x == X && Port1Pos[i].y == Y) return i; }
	else for (int i = 0; i < 5; i++){ if (Port2Pos[i].x == X && Port2Pos[i].y == Y) return i; }
	return -1;
}

bool Portal::isActive(int X, int Y) {
	int x = getPortal(X, Y, true);
	if (x != -1) return active[x];
	return false;
}

void Portal::activatePortal(int X, int Y) {
	int i = getPortal(X, Y, true);
	if (active[i]) return;
	if (i != -1) {
		active[i] = true;
		glm::vec2 entry = Port1Pos[i];
		glm::vec2 exit = Port2Pos[i];
		Game::instance().changeSSAnimation(true, entry.x, entry.y);
		Game::instance().changeSSAnimation(true, exit.x, exit.y);
	}
}

void Portal::deactivatePortal(int X, int Y) {
	int i = getPortal(X, Y, false);
	if (!active[i]) return;
	if (i != -1) {
		active[i] = false;
		glm::vec2 entry = Port1Pos[i];
		glm::vec2 exit = Port2Pos[i];
		Game::instance().changeSSAnimation(false, entry.x, entry.y);
		Game::instance().changeSSAnimation(false, exit.x, exit.y);
	}
}

glm::vec2 Portal::getExit(int X, int Y) {
	int i = getPortal(X, Y, true);
	return Port2Pos[i];
}