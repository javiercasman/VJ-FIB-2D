#include <iostream>
#include "Item.h"

enum ItemAnims
{
	IDLE //un item solo tiene la animación idle
};

void Item::init(string type, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tileMapDispl = tileMapPos;
	onScreen = false; grabbed = false;
	if (type == "Key") {
		tileSize = glm::ivec2(12,24);
		sprite = Sprite::createSprite(tileSize, glm::vec2(5.f/26.f, 1), &spritesheet, &shaderProgram);//son 5 de anchura
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(IDLE, 6);
		sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
		sprite->changeAnimation(0);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
	}
	else if (type == "Gem") {
		tileSize = glm::ivec2(12,24);
		sprite = Sprite::createSprite(tileSize, glm::vec2(5.f / 26.f, 1), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(IDLE, 6);
		sprite->addKeyframe(IDLE, glm::vec2(1 * 5.f/26.f, 0.f));
		sprite->changeAnimation(0);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
	}
	else if (type == "Stopwatch") {
		tileSize = glm::ivec2(16, 32);
		sprite = Sprite::createSprite(tileSize, glm::vec2(8.f / 26.f, 1), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(IDLE, 6);
		sprite->addKeyframe(IDLE, glm::vec2(2 * 5.f / 26.f, 0.f));
		sprite->changeAnimation(0);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
	}
	else if (type == "Hourglass") {
		tileSize = glm::ivec2(16, 32);
		sprite = Sprite::createSprite(tileSize, glm::vec2(8.f / 26.f, 1), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(1);
		sprite->setAnimationSpeed(IDLE, 6);
		sprite->addKeyframe(IDLE, glm::vec2(1 * 18.f / 26.f, 0.f));
		sprite->changeAnimation(0);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
	}
}

void Item::render()
{
	sprite->render();
	onScreen = true;
}

void Item::free()
{
	sprite->free();
	onScreen = false;
}

void Item::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Item::setPosition(const glm::vec2& pos)
{
	posItem = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posItem.x), float(tileMapDispl.y + posItem.y)));
}