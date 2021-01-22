#include <GL/glew.h>
#include <GL/gl.h>
#include "Interface.h"
#include "Game.h"
#include <iostream>
#include <sstream>
using namespace std;

string IntToString(int a)
{
	ostringstream temp;
	temp << a;
	return temp.str();
}

Interface *Interface::createInterface(ShaderProgram &simpleP, ShaderProgram &texP)
{
	Interface *interf = new Interface(simpleP,texP);
	return interf;
}

void Interface::free()
{
	glDeleteBuffers(1, &vboQ);
	glDeleteBuffers(1, &vboT);
}

Interface::Interface(ShaderProgram &simpleP, ShaderProgram &texP)
{
	//init
	simpleProgram = simpleP;
	texProgram = texP;
	varT = 0.f;
	if (!text.init("fonts/PoP.ttf")) cout << "error load font" << endl;
	str = "";
	writeWin = writeLose = false;
	iniQuad();
	iniTriangle();
}

void Interface::render(glm::mat4 projection, int maxL, int actL, bool can_print_crono)
{
	renderQuad(projection);
	renderLife(projection, maxL, actL);
	if (str != "") renderText();
	else if (can_print_crono)render_Time();
	renderBigText();
}

void Interface::iniQuad(){
	float x = 0.f;
	float width = 32 * 10;
	float y = 64 * 3;
	float height = 8.f;
	float vertices[12] = {
		x, y, x + width, y, x + width, y + height,
		x, y, x + width, y + height, x, y + height
	};

	glGenVertexArrays(1, &vaoQ);
	glBindVertexArray(vaoQ);
	glGenBuffers(1, &vboQ);
	glBindBuffer(GL_ARRAY_BUFFER, vboQ);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = simpleProgram.bindVertexAttribute("position", 2, 0, 0);
}

void Interface::renderQuad(glm::mat4 projection){
	simpleProgram.use();
	simpleProgram.setUniformMatrix4f("projection", projection);
	simpleProgram.setUniform4f("color", 0.0f, 0.0f, 0.0f, 1.0f);
	simpleProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));

	glBindVertexArray(vaoQ);
	glEnableVertexAttribArray(posLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Interface::iniTriangle(){
	texture.loadFromFile("images/LifeSpriteSheet.png", TEXTURE_PIXEL_FORMAT_RGBA); //24*8
	glm::vec2 quadSize = glm::vec2(6, 8);//6*8pixels
	glm::vec2 sizeInSpritesheet = glm::vec2(1.f / 4.f, 1.f / 1.f);// 4 col / 1 fila

	float vertices[24] = {
		0.f, 0.f, 0.f, 0.f,
		quadSize.x, 0.f, sizeInSpritesheet.x, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, 0.f, 0.f, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, quadSize.y, 0.f, sizeInSpritesheet.y
	};

	glGenVertexArrays(1, &vaoT);
	glBindVertexArray(vaoT);
	glGenBuffers(1, &vboT);
	glBindBuffer(GL_ARRAY_BUFFER, vboT);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLoc = texProgram.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLoc = texProgram.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

void Interface::renderTriangle(glm::mat4 projection, int x, bool player, bool full){
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);//tocar canviar color corazones enemy,no olblidar player
	int a;
	if (player && full)			a = 0;
	else if (player && !full)	a = 1;
	else if (!player && full)	a = 2;
	else if (!player && !full)	a = 3;
	glm::vec2 texCoordDispl = glm::vec2(a*1.f / 4.f, 0);

	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(x, 64 * 3 - 1, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture.use();
	glBindVertexArray(vaoT);
	glEnableVertexAttribArray(posLoc);
	glEnableVertexAttribArray(texCoordLoc);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Interface::renderLife(glm::mat4 projection, int maxL, int actL){
	if (actL == 1){
		renderTriangle(projection, 0 * 8 + 2, true, int(varT) % 256 > 128);
		for (int i = 1; i < maxL; i++) renderTriangle(projection, i * 8 + 2, true, i < actL);
	}
	else {
		varT = 0.f;
		for (int i = 0; i < maxL; i++) renderTriangle(projection, i * 8 + 2, true, i < actL);
	}
}

void Interface::renderLifeEnemy(glm::mat4 projection, int maxL, int actL){
	for (int i = 0; i < maxL; i++) renderTriangle(projection, 32 * 10 - (i + 1) * 8 - 1, false, i < actL);
}

void Interface::update(int deltaTime, float timePlaying){
	varT += deltaTime;
	time_playing = timePlaying;
}

void Interface::render_text(string s, glm::vec2 posC, int heightLetter, glm::vec4 color){
	float factor_scale = 2.5f;
	int length = text.lengthPixel(s, heightLetter * factor_scale) / factor_scale;
	glm::vec2 posLD = glm::vec2((posC.x - length / 2), (posC.y + heightLetter / 2));
	text.render(s, posLD * factor_scale, heightLetter * factor_scale, color);
}

void Interface::render_String(string s){
	int heightLetter = 7;
	glm::vec2 posC = glm::vec2(320 / 2, 200 - (8 / 2) - 1);
	glm::vec4 color = glm::vec4(1, 1, 1, 1);
	render_text(s, posC, heightLetter, color);
}

void Interface::renderText(){ //render write en la interficie
	render_String(str);
}

void Interface::render_Time(){
	int seg = time_playing / 1000.f;
	int seg_print = MAX_TIME_SEG - seg;
	if (Game::instance().getKey(' ') || seg_print<10){
		int actMin = seg_print / 60;
		int actSeg = seg_print % 60;
		string time = IntToString(actMin) + " minutes and " + IntToString(actSeg) + " seconds";
		render_String(time);
	}
}

void Interface::renderBigText(){ //render write en la interficie
	int heightLetter = 70;
	glm::vec2 posC = glm::vec2(320 / 2, (200 - 8) / 2 - 10);
	if (writeWin){
		glm::vec4 color = glm::vec4(0, 1, 0, 1);
		render_text("YOU WON", posC, heightLetter, color);
	}
	else if (writeLose){
		glm::vec4 color = glm::vec4(1, 0, 0, 1);
		render_text("DEAD", posC, heightLetter, color);
	}
	if ((time_playing/1000.f) > MAX_TIME_SEG){
		heightLetter = 65;
		glm::vec4 color = glm::vec4(1, 1, 0, 1);
		render_text("TIME OUT", posC, heightLetter, color);
	}
}

void Interface::write(string s){
	str = s; 
}

void Interface::win(bool win){
	writeWin = win;
}

void Interface::lose(bool lose){
	writeLose = lose;
}