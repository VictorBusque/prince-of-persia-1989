#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Text.h"

#pragma comment (lib, "lib/freetype.lib") 


#define ATLAS_FONT_SIZE 64


using namespace std;


bool Text::bLibInit = false;
FT_Library Text::library;


Text::Text()
{
	quad = NULL;
}

Text::~Text()
{
	destroy();
	if(quad != NULL)
	{
		quad->free();
		delete quad;
	}
}


bool Text::init(const char *filename)
{
	FT_Error error;
	
	if(!bLibInit)
	{
		error = FT_Init_FreeType(&Text::library);
		if(error)
			return false;
		bLibInit = true;
	}
	error = FT_New_Face(Text::library, filename, 0, &face);
	if(error)
		return false;
	FT_Set_Pixel_Sizes(face, ATLAS_FONT_SIZE, ATLAS_FONT_SIZE);
	
	if(!extractCharSizes(&maxCharWidth, &maxCharHeight))
		return false;
	fontSize = maxCharHeight;
	textureSize = 512;
	if(floor(float(textureSize) / maxCharWidth) * floor(float(textureSize) / maxCharHeight) < (128 - 32))
		textureSize = 1024;
	if(floor(float(textureSize) / maxCharWidth) * floor(float(textureSize) / maxCharHeight) < (128 - 32))
		return false;
	createTextureAtlas();
	initShaders();
	
	glm::vec2 geom[2] = {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)};
	glm::vec2 texCoords[2] = {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)};
	
	quad = TexturedQuad::createTexturedQuad(geom, texCoords, program);
	
	return true;
}

void Text::destroy()
{
	FT_Done_Face(face);
}

ShaderProgram &Text::getProgram()
{
	return program;
}

int Text::getSize() const
{
	return fontSize;
}

void Text::render(char c, const glm::vec2 &pixel, int size, const glm::vec4 &color)
{
	int vp[4];
	glm::mat4 projection, modelview;
	glm::vec2 minTexCoord, maxTexCoord;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	program.use();
	glGetIntegerv(GL_VIEWPORT, vp);
	projection = glm::ortho(0.f, float(vp[2] - 1), float(vp[3] - 1), 0.f);
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", color.r, color.g, color.b, color.a);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(modelview, glm::vec3(pixel.x, pixel.y - size, 0.f));
	modelview = glm::scale(modelview, (float(size) / fontSize) * glm::vec3(chars[c-32].sx, chars[c-32].sy, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	minTexCoord = glm::vec2(float(chars[c-32].tx) / textureSize, float(chars[c-32].ty) / textureSize);
	maxTexCoord = glm::vec2(float(chars[c-32].tx + chars[c-32].sx) / textureSize, float(chars[c-32].ty + chars[c-32].sy) / textureSize);
	program.setUniform2f("minTexCoord", minTexCoord.s, minTexCoord.t);
	program.setUniform2f("maxTexCoord", maxTexCoord.s, maxTexCoord.t);
	quad->render(textureAtlas);
	glDisable(GL_BLEND);
}

void Text::render(const string &str, const glm::vec2 &pixel, int size, const glm::vec4 &color)
{
	int vp[4];
	glm::mat4 projection, modelview;
	glm::vec2 minTexCoord, maxTexCoord, pos = pixel;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	program.use();
	glGetIntegerv(GL_VIEWPORT, vp);
	projection = glm::ortho(0.f, float(vp[2] - 1), float(vp[3] - 1), 0.f);
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", color.r, color.g, color.b, color.a);

	for(unsigned int i=0; i<str.length(); i++)
	{
		modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3(pos.x + (float(size) / fontSize) * chars[str[i]-32].bl, pos.y - (float(size) / fontSize) * chars[str[i]-32].bt, 0.f));
		modelview = glm::scale(modelview, (float(size) / fontSize) * glm::vec3(chars[str[i]-32].sx, chars[str[i]-32].sy, 0.f));
		program.setUniformMatrix4f("modelview", modelview);
		minTexCoord = glm::vec2(float(chars[str[i]-32].tx) / textureSize, float(chars[str[i]-32].ty) / textureSize);
		maxTexCoord = glm::vec2(float(chars[str[i]-32].tx + chars[str[i]-32].sx) / textureSize, float(chars[str[i]-32].ty + chars[str[i]-32].sy) / textureSize);
		program.setUniform2f("minTexCoord", minTexCoord.s, minTexCoord.t);
		program.setUniform2f("maxTexCoord", maxTexCoord.s, maxTexCoord.t);
		quad->render(textureAtlas);
		pos.x += (float(size) / fontSize) * chars[str[i]-32].ax;
	}

	glDisable(GL_BLEND);
}

float Text::lengthPixel(string str, int size){
	float ret = 0.f;
	for (unsigned int i = 0; i<str.length(); i++) 
		ret += (float(size) / fontSize) * chars[str[i] - 32].ax;
	return ret;
}

void Text::initShaders()
{
	Shader vShader, fShader;

	vShader.free();
	fShader.free();
	vShader.initFromFile(VERTEX_SHADER, "shaders/text.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/text.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	program.init();
	program.addShader(vShader);
	program.addShader(fShader);
	program.link();
	if(!program.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << program.log() << endl << endl;
	}
	program.bindFragmentOutput("outColor");
}

bool Text::extractCharSizes(int *maxCharWidth, int *maxCharHeight)
{
	unsigned char c;

	*maxCharWidth = 0;
	*maxCharHeight = 0;
	for(c=32; c<128; c++)
	{
		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
			return false;
		*maxCharWidth = glm::max(*maxCharWidth, (int)face->glyph->bitmap.width);
		*maxCharHeight = glm::max(*maxCharHeight, (int)face->glyph->bitmap.rows);
	}
	
	return true;
}

void Text::createTextureAtlas()
{
	unsigned char c;
	int x = 0, y = 0;

	textureAtlas.createEmptyTexture(textureSize, textureSize);
	for(c=32; c<128; c++)
	{
		FT_Load_Char(face, c, FT_LOAD_RENDER);
		chars[c-32].tx = x;
		chars[c-32].ty = y;
		chars[c-32].sx = face->glyph->bitmap.width;
		chars[c-32].sy = face->glyph->bitmap.rows;
		chars[c-32].ax = face->glyph->advance.x >> 6;
		chars[c-32].ay = face->glyph->advance.y >> 6;
		chars[c-32].bl = face->glyph->bitmap_left;
		chars[c-32].bt = face->glyph->bitmap_top;
		textureAtlas.loadSubtextureFromGlyphBuffer(face->glyph->bitmap.buffer, x, y, face->glyph->bitmap.width, face->glyph->bitmap.rows);
		x += maxCharWidth;
		if((x + maxCharWidth) >= textureSize)
		{
			x = 0;
			y += maxCharHeight;
		}
	}
	textureAtlas.generateMipmap();
	textureAtlas.setWrapS(GL_CLAMP_TO_EDGE);
	textureAtlas.setWrapT(GL_CLAMP_TO_EDGE);
}



