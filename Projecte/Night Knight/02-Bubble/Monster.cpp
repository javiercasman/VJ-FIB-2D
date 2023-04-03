#include "Monster.h"

#define FALL_STEP 4

enum MonsterAnims
{
	MOVE_RIGHT, MOVE_LEFT, CAST_RIGHT, CAST_LEFT
	//las 2 ultimas son del mago
};

void Monster::init(string type, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	Monster::type = type;
	Monster::texProgram = shaderProgram;
	spritesheet.loadFromFile("images/monsters.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tileMapDispl = tileMapPos;
	if (type == "skeleton") {
		powerTime = -1;
		tileSize = glm::ivec2(16, 32);
		sprite = Sprite::createSprite(tileSize, glm::vec2(9.f / 64.f, 15.f/80.f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(MOVE_RIGHT, 6);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0 * 9.f / 64.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * 9.f / 64.f, 0.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 6);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 * 9.f / 64.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * 9.f / 64.f, 0.f));
	}
	else if (type == "vampire") {
		powerCooldown = 12.f;//12 segundos de enfriamiento para transformarse
		tileSize = glm::ivec2(16, 32);
		sprite = Sprite::createSprite(tileSize, glm::vec2(9.f / 64.f, 15.f / 80.f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(MOVE_RIGHT, 6);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0 * 9.f / 64.f, 1 * 15.f / 80.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * 9.f / 64.f, 1* 15.f / 80.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 6);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 * 9.f / 64.f, 1 * 15.f / 80.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * 9.f / 64.f, 1 * 15.f / 80.f));
	}
	else if (type == "bat") {//se crean cuando el vampiro se transforma
		powerCooldown = 6.f;
		up = true;
		tileSize = glm::ivec2(32,32);
		sprite = Sprite::createSprite(tileSize, glm::vec2(14.f / 64.f, 14.f/80.f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(MOVE_RIGHT, 6);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0 * 16.f / 64.f, 2 * 15.f / 80.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(1 * 16.f / 64.f, 2 * 15.f / 80.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * 16.f / 64.f, 2 * 15.f / 80.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 6);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0 * 16.f / 64.f, 2 * 15.f / 80.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 * 16.f / 64.f, 2 * 15.f / 80.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(2 * 16.f / 64.f, 2 * 15.f / 80.f));//es lo mismo pero servirá para indicar a donde se mueve

	}
	else if (type == "wizard") {//cambiar esto me voy a dormir
		powerCooldown = 5.0f;//5 segundos de enfriamiento para desactivar baldosas
		tileSize = glm::ivec2(32,32);
		sprite = Sprite::createSprite(tileSize, glm::vec2(16.f / 64.f, 16.f/80.f), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(4);

		sprite->setAnimationSpeed(MOVE_RIGHT, 5);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0 * 16.f / 64.f, 1 * 46.f / 80.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * 16.f / 64.f, 1 * 46.f / 80.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0 * 16.f / 64.f, 1 * 46.f / 80.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0 * 16.f / 64.f, 1 * 62.f / 80.f));

		sprite->setAnimationSpeed(MOVE_LEFT, 5);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 * 16.f / 64.f, 1 * 46.f / 80.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * 16.f / 64.f, 1 * 46.f / 80.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 * 16.f / 64.f, 1 * 46.f / 80.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 * 16.f / 64.f, 1 * 62.f / 80.f));

		sprite->setAnimationSpeed(CAST_RIGHT, 6);
		sprite->addKeyframe(CAST_RIGHT, glm::vec2(2 * 16.f / 64.f, 4 * 15.f / 80.f));

		sprite->setAnimationSpeed(CAST_LEFT, 6);
		sprite->addKeyframe(CAST_LEFT, glm::vec2(3 * 16.f / 64.f, 1 * 15.f / 80.f));
	}
	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y)));
}

void Monster::update(int deltaTime)
{
	if(type != "skeleton") powerTime += deltaTime;
	if (powerTime / 1000.f >= powerCooldown) {
		if (type == "vampire") {
			int aux = sprite->animation();
			delete sprite;
			init("bat", tileMapDispl, texProgram);
			powerTime = 0.f;
			sprite->changeAnimation(aux);
		}
		else if (type == "bat" && 
			map->collisionMoveDown(glm::ivec2(posMonster.x, posMonster.y+map->getTileSizey()), tileSize, &posMonster.y, false)) {
			int aux = sprite->animation();
			delete sprite;
			init("vampire", tileMapDispl, texProgram);
			powerTime = 0.f;
			sprite->changeAnimation(aux);
		}
		else if (type == "wizard") {
			if (map->deactivate(posMonster.x, posMonster.y + FALL_STEP)) {
				//tenemos que hacer que se quede congelado unos 10/11 frames
				powerTime = 0.f;
			}
		}
	}
	sprite->update(deltaTime);
	glm::ivec2 posMonsterLeft = glm::ivec2(posMonster.x - map->getTileSizex(), posMonster.y + FALL_STEP);
	glm::ivec2 posMonsterRight = glm::ivec2(posMonster.x + map->getTileSizex(), posMonster.y + FALL_STEP);
	if (sprite->animation() == MOVE_LEFT) {
		posMonster.x -= 1;
		if (map->collisionMoveLeft(posMonster, tileSize) || 
			(!map->collisionMoveDown(posMonsterLeft, tileSize, &posMonsterLeft.y, false) && type != "bat"))
		{
			posMonster.x += 1;
			sprite->changeAnimation(sprite->animation() - 1);
		}
	}
	else if (sprite->animation() == MOVE_RIGHT) {
		posMonster.x += 1;
		if (map->collisionMoveRight(posMonster, tileSize) ||
			(!map->collisionMoveDown(posMonsterRight, tileSize, &posMonsterRight.y, false) && type != "bat"))
		{
			posMonster.x -= 1;
			sprite->changeAnimation(sprite->animation() + 1);
		}
	}
	if (type == "bat") {
		if (up) {
			posMonster.y -= 1;
			if (map->collisionMoveUp(posMonster, tileSize)) {
 				posMonster.y += 1;
				up = false;
			}
		}
		else {
			posMonster.y += 1;
			glm::ivec2 pos = glm::ivec2(posMonster.x, posMonster.y + 1);//para hacer bien la colision con el suelo
			if (map->collisionMoveDown(pos, tileSize, &posMonster.y, false)) {
				posMonster.y -= 1;
				up = true;
			}
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y)));
}

void Monster::render()
{
	sprite->render();
}

void Monster::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Monster::setPosition(const glm::vec2& pos)
{
	posMonster = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posMonster.x), float(tileMapDispl.y + posMonster.y)));
}
