#ifndef _INTERFFACE_INCLUDE
#define _INTERFFACE_INCLUDE


#include "ShaderProgram.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Texture.h"
#include "Player.h"
#include "Text.h"


#define MAX_TIME_SEG (15*60)

class Interface
{

public:
	static Interface *createInterface(ShaderProgram &simpleP, ShaderProgram &texP);

	Interface(ShaderProgram &simpleP, ShaderProgram &texP);
	void update(int deltaTime, float timePlaying);
	void render(glm::mat4 projection, int maxL, int actL, bool can_print_crono);
	void renderLifeEnemy(glm::mat4 projection, int maxL, int actL);
	void free();
	void write(string s);
	void win(bool win);
	void lose(bool lose);

private:
	void iniQuad();
	void iniTriangle();
	void renderText();
	void renderBigText();
	void renderQuad(glm::mat4 projection);
	void renderLife(glm::mat4 projection, int maxL, int actL);
	void renderTriangle(glm::mat4 projection, int x, bool player, bool full);
	void render_text(string s, glm::vec2 posC, int heightLetter, glm::vec4 color);
	void render_String(string s);
	void render_Time();

private:
	float varT;
	GLuint vaoQ, vaoT;
	GLuint vboQ, vboT;
	GLint posLocation, posLoc, texCoordLoc;
	ShaderProgram texProgram, simpleProgram;
	Texture texture;
	Text text;
	string str;
	bool writeWin, writeLose;
	float time_playing;
	
};


#endif // _INTERFFACE_INCLUDE

