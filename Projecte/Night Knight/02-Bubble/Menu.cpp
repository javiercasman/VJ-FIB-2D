#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "Menu.h"

Menu::Menu()
{
	engine = Sound::instance().getSoundEngine();
	currentTime = 0.0f;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	prepareArrays(geom, texCoords, ShaderManager::instance().getShaderProgram());

	spritesheet.loadFromFile("images/select-menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	selection = Sprite::createSprite(glm::ivec2(252,37), glm::vec2(1, 1), &spritesheet, &ShaderManager::instance().getShaderProgram());
	selection->setNumberAnimations(1);

	selection->setAnimationSpeed(0, 6);
	selection->addKeyframe(0, glm::vec2(0, 0));

	selection->changeAnimation(0);
	selection->setPosition(glm::vec2(194.f, 249.f));

	main.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	instructions.loadFromFile("images/instr.png", TEXTURE_PIXEL_FORMAT_RGBA);
	credits.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	gameover.loadFromFile("images/gameover.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

Menu::~Menu()
{
	if (selection != NULL)
		delete selection;
}

void Menu::init(const int& idMenu)
{
	id = idMenu;
	if (id == 0) screen = main;
	else if (id == 1) screen = instructions;
	else if (id == 2) screen = credits;
	else if (id == 3) screen = gameover;
	pauseTime = 0.f;

	screen.setWrapS(GL_CLAMP_TO_EDGE);
	screen.setWrapT(GL_CLAMP_TO_EDGE);
	screen.setMinFilter(GL_NEAREST);
	screen.setMagFilter(GL_NEAREST);
}

void Menu::render()
{
	ShaderManager::instance().render();
	glEnable(GL_TEXTURE_2D);
	screen.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
	if (id == 0) selection->render();
}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
	if (id == 0) selection->update(deltaTime);
	if (id == 3) {
		if (pauseTime / 1000.f >= 4.f) Game::instance().init();
		else pauseTime += deltaTime;
	}
}

void Menu::setPosIndex(int pos)
{
	if(pos == 0) selection->setPosition(glm::vec2(194.f, 249.f));
	else if (pos == 1) selection->setPosition(glm::vec2(194.f,316.f));
	else if (pos == 2) selection->setPosition(glm::vec2(194.f, 383.f));
}

void Menu::prepareArrays(const glm::vec2 geom[2], const glm::vec2 texCoords[2], ShaderProgram& program)
{
	float vertices[24] = { geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
												geom[1].x, geom[0].y, texCoords[1].x, texCoords[0].y,
												geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
												geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
												geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
												geom[0].x, geom[1].y, texCoords[0].x, texCoords[1].y };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}
