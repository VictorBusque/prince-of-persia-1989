#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Intro.h"
#include "Game.h"
#include <GL/glut.h>

void Intro::init()
{
	initShaders();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	text.init("fonts/PoP.ttf");
	toPlay = false;
	changeTo(INTRO_S);
	currentTime = 0.0f;
}

void Intro::update(int deltaTime)
{
	currentTime += deltaTime;
	switch (state)
	{
	case INTRO_S:
		if (currentTime > 2 * 1000) changeTo(HISTO_S);
		break;
	case HISTO_S:
		if (currentTime > 20 * 1000) changeTo(JUGAR);
		break; 
	case JUGAR:
		if (toPlay){
			toPlay = false;
			Game::instance().startGame();
			str = "";
		}
		else if (Game::instance().getKeyPressed(GL_KEY_RETURN)) {
			toPlay = true;
			str = "Starting ...";
		}
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_DOWN)) changeTo(DIFICULTAD);
		break;
	case DIFICULTAD:
		if (Game::instance().getKeyPressed(GL_KEY_RETURN)){
			switch (Game::instance().getLevel()){
			case LEVEL_EASY: changeTo(FACIL); break;
			case LEVEL_NORMAL: changeTo(NORMAL); break;
			case LEVEL_HARD: changeTo(DIFICIL); break;
			default: break;
			}
		}
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP)) changeTo(JUGAR);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_DOWN)) changeTo(INSTRUCCIONES);
		break;
	case INSTRUCCIONES:
		if (Game::instance().getKeyPressed(GL_KEY_RETURN)) changeTo(INSTRUCCIONES2);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP)) changeTo(DIFICULTAD);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_DOWN)) changeTo(CREDITOS);
		break;
	case CREDITOS:
		if (Game::instance().getKeyPressed(GL_KEY_RETURN)) changeTo(CREDITOS2);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP)) changeTo(INSTRUCCIONES);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_DOWN)) changeTo(CREDITOS);
		break;
	case FACIL:
		if (Game::instance().getKeyPressed(GL_KEY_RETURN)){
			Game::instance().setLevel(LEVEL_EASY);
			changeTo(JUGAR);
		}
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_DOWN)) changeTo(NORMAL);
		break;
	case NORMAL:
		if (Game::instance().getKeyPressed(GL_KEY_RETURN)){
			Game::instance().setLevel(LEVEL_NORMAL);
			changeTo(JUGAR);
		}
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP)) changeTo(FACIL);
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_DOWN)) changeTo(DIFICIL);
		break;
	case DIFICIL:
		if (Game::instance().getKeyPressed(GL_KEY_RETURN)){
			Game::instance().setLevel(LEVEL_HARD);
			changeTo(JUGAR);
		}
		else if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP)) changeTo(NORMAL);
		break;
	case CREDITOS2:
		if ((glm::mod((currentTime / 500.f), 3.0f)>1.f)) str = "PRESS 'ENTER' TO RETURN TO MENU";
		else str = "";
		if (Game::instance().getKeyPressed(GL_KEY_RETURN)) changeTo(CREDITOS);
		break;

	case INSTRUCCIONES2: if (Game::instance().getKeyPressed(' ')) changeTo(INSTRUCCIONES3);
		break;
	case INSTRUCCIONES3: if (Game::instance().getKeyPressed(' ')) changeTo(INSTRUCCIONES);
		break;
	default: break;
	}

	if (state == INTRO_S || state == HISTO_S){
		if((glm::mod((currentTime/500.f),3.0f)>1.f)) str = "PRESS 'SPACE' TO SKIP";
		else str = "";
		if (Game::instance().getKey(' ')) changeTo(JUGAR);
	}
}

void Intro::changeTo(IntroStates nS){ 
	switch (nS)
	{
	case INTRO_S:
		initF();
		break;
	case HISTO_S:
		textureF.loadFromFile("images/Fondo_Intro.png", TEXTURE_PIXEL_FORMAT_RGBA);
		initD();
		break; 
	case JUGAR:	textureF.loadFromFile("images/MenuBackground_Jugar.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case DIFICULTAD: textureF.loadFromFile("images/MenuBackground_Dificultad.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case INSTRUCCIONES: textureF.loadFromFile("images/MenuBackground_Instrucciones.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case CREDITOS: textureF.loadFromFile("images/MenuBackground_Credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case FACIL: textureF.loadFromFile("images/MenuDificultad_Facil.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case NORMAL: textureF.loadFromFile("images/MenuDificultad_Normal.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case DIFICIL: textureF.loadFromFile("images/MenuDificultad_Dificil.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case CREDITOS2: textureF.loadFromFile("images/Credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case INSTRUCCIONES2: textureF.loadFromFile("images/instruccions_1.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case INSTRUCCIONES3: textureF.loadFromFile("images/instruccions_2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	default: break;
	}
	state = nS;
	str = "";
}

void Intro::render(){
	renderF();
	if (state==HISTO_S) renderD();
	if(str!="")renderText();
}

void Intro::initF(){
	textureF.loadFromFile("images/Titulo_Intro.png", TEXTURE_PIXEL_FORMAT_RGBA);
	glm::vec2 quadSize = glm::vec2(320, 200);
	glm::vec2 sizeInSpritesheet = glm::vec2(1.f, 1.f);

	float vertices[24] = {
		0.f, 0.f, 0.f, 0.f,
		quadSize.x, 0.f, sizeInSpritesheet.x, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, 0.f, 0.f, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, quadSize.y, 0.f, sizeInSpritesheet.y
	};

	glGenVertexArrays(1, &vaoF);
	glBindVertexArray(vaoF);
	glGenBuffers(1, &vboF);
	glBindBuffer(GL_ARRAY_BUFFER, vboF);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocationF = texProgram.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocationF = texProgram.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

void Intro::initD(){
	textureD.loadFromFile("images/text_1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	glm::vec2 quadSize = glm::vec2(290,159);
	glm::vec2 sizeInSpritesheet = glm::vec2(1.f, 1.f);

	float vertices[24] = {
		0.f, 0.f, 0.f, 0.f,
		quadSize.x, 0.f, sizeInSpritesheet.x, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, 0.f, 0.f, 0.f,
		quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y,
		0.f, quadSize.y, 0.f, sizeInSpritesheet.y
	};

	glGenVertexArrays(1, &vaoD);
	glBindVertexArray(vaoD);
	glGenBuffers(1, &vboD);
	glBindBuffer(GL_ARRAY_BUFFER, vboD);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocationD = texProgram.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocationD = texProgram.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

void Intro::renderF()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	texProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	glEnable(GL_TEXTURE_2D);
	textureF.use();
	glBindVertexArray(vaoF);
	glEnableVertexAttribArray(posLocationF);
	glEnableVertexAttribArray(texCoordLocationF);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Intro::renderD()
{
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(15.f, 15.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	glEnable(GL_TEXTURE_2D);
	textureD.use();
	glBindVertexArray(vaoD);
	glEnableVertexAttribArray(posLocationD);
	glEnableVertexAttribArray(texCoordLocationD);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Intro::render_text(string s, glm::vec2 posC, int heightLetter, glm::vec4 color){
	float factor_scale = 2.5f;
	int length = text.lengthPixel(s, heightLetter * factor_scale) / factor_scale;
	glm::vec2 posLU = glm::vec2((posC.x - (length / 2)), posC.y + (heightLetter/2));
	text.render(s, posLU * factor_scale, heightLetter * factor_scale, color);
}

void Intro::renderText(){
	int heightLetter = 25;
	if (state == CREDITOS2) heightLetter = 16;
	glm::vec2 posC = glm::vec2(320 / 2, 200 - (26 / 2)-3);
	glm::vec4 color = glm::vec4(1, 1, 1, 1);
	render_text(str, posC, heightLetter, color);
}

void Intro::initShaders()
{
	Shader vShader, fShader;

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