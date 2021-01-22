#ifndef _PORTAL_INCLUDE
#define _PORTAL_INCLUDE

#include <glm/glm.hpp>
#pragma once

#define NUM_PORTS 5;

class Portal
{

public:
	Portal();
	~Portal();
	void freePortals();

	glm::vec2 getEntryPortalPosition(int i);
	glm::vec2 getExitPortalPosition(int i);
	void init(int level);

	void activatePortal(int X, int Y);
	void deactivatePortal(int X, int Y);
	bool isActive(int X, int Y);
	glm::vec2 getExit(int X, int Y);

private:
	int getPortal(int X, int Y, bool entry);

private:
	int numPortals;
	bool *active;
	glm::vec2 *Port1Pos;
	glm::vec2 *Port2Pos;
};
#endif	// _PORTAL_INCLUDE

