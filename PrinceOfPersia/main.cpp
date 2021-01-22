#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

//Remove console (only works in Visual Studio)
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define TIME_PER_FRAME (1000.f / 60.f)// Approx. 60 fps
#define SCREEN_SIZE_FACT 2.5f

static int prevTime;
static Game game; // This object represents our whole game

// If a key is pressed this callback is called
static void keyboardDownCallback(unsigned char key, int x, int y)
{
	Game::instance().keyPressed(key);
}

// If a key is released this callback is called
static void keyboardUpCallback(unsigned char key, int x, int y)
{
	Game::instance().keyReleased(key);
}

// If a special key is pressed this callback is called
static void specialDownCallback(int key, int x, int y)
{
	Game::instance().specialKeyPressed(key);
	Game::instance().modifiersPressed(glutGetModifiers());
}

// If a special key is released this callback is called
static void specialUpCallback(int key, int x, int y)
{
	Game::instance().specialKeyReleased(key);
	Game::instance().modifiersPressed(glutGetModifiers());
}

// Same for changes in mouse cursor position
static void motionCallback(int x, int y)
{
	Game::instance().mouseMove(x, y);
}

// Same for mouse button presses or releases
static void mouseCallback(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
		Game::instance().mousePress(button);
	else if(state == GLUT_UP)
		Game::instance().mouseRelease(button);
}
static void drawCallback()
{
	Game::instance().render();
	glutSwapBuffers();
}

static void idleCallback()
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = currentTime - prevTime;
	
	if(deltaTime > TIME_PER_FRAME)
	{
		// Every time we enter here is equivalent to a game loop execution
		if(!Game::instance().update(deltaTime)) exit(0);
		prevTime = currentTime;
		glutPostRedisplay();
	}
}

static void size(int width, int height){
	glutReshapeWindow(SCREEN_WIDTH * SCREEN_SIZE_FACT, SCREEN_HEIGHT * SCREEN_SIZE_FACT);
}

int main(int argc, char **argv)
{
	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(SCREEN_WIDTH * SCREEN_SIZE_FACT, SCREEN_HEIGHT * SCREEN_SIZE_FACT);

	glutCreateWindow(argv[0]);
	glutDisplayFunc(drawCallback);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(keyboardDownCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialFunc(specialDownCallback);
	glutSpecialUpFunc(specialUpCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	glutReshapeFunc(size);

	// GLEW will take care of OpenGL extension functions
	glewExperimental = GL_TRUE;
	glewInit();
	
	// Game instance initialization
	Game::instance().init();
	prevTime = glutGet(GLUT_ELAPSED_TIME);
	// GLUT gains control of the application
	glutMainLoop();

	return 0;
}



