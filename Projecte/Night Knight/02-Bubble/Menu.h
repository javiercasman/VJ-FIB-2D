#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <vector>
#include <glm/glm.hpp>
#include "Sprite.h"
#include "ShaderProgram.h"
#include "ShaderManager.h"
#include "Sound.h"

class Menu
{
public:
	Menu();
	~Menu();

	void init(const int& idMenu);
	void render();
	void update(int deltaTime);
	void setPosIndex(int pos);
	void prepareArrays(const glm::vec2 geom[2], const glm::vec2 texCoords[2], ShaderProgram& program);
	
private:
	int id;
	Texture screen, main, instructions, credits, gameover;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	Sprite* selection;
	float currentTime, pauseTime;
	Texture spritesheet;
	ISoundEngine* engine;
};
#endif // _MENU_INCLUDE