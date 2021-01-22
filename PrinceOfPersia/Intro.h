#ifndef _INTRO_INCLUDE
#define _INTRO_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "text.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Intro
{
	enum IntroStates
	{
		INTRO_S, HISTO_S, MENU_S,
		JUGAR, INSTRUCCIONES, DIFICULTAD, CREDITOS,
		FACIL, NORMAL, DIFICIL,
		CREDITOS2, INSTRUCCIONES2, INSTRUCCIONES3
	};

public:

	void init();
	void update(int deltaTime);
	void render();

private:
	void changeTo(IntroStates nS);
	void initF();
	void initD();
	void initS();
	void renderF();
	void renderD();
	void renderS();
	void initShaders();
	void renderText();
	void render_text(string s, glm::vec2 posC, int heightLetter, glm::vec4 color);
	void write(string s);
	void update_In_Meun();
	void render_In_Meun();

private:
	ShaderProgram texProgram;
	glm::mat4 projection;
	//fondo
	GLuint vaoF, vboF;
	GLint posLocationF, texCoordLocationF;
	Texture textureF;
	//dialodo
	GLuint vaoD, vboD;
	GLint posLocationD, texCoordLocationD;
	Texture textureD;

	float currentTime;
	Text text;
	string str;
	IntroStates state;

	bool toPlay;
	
};


#endif // _INTRO_INCLUDE