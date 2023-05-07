#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 56
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, DEAD_LEFT, DEAD_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false, isDead = false, invulnerable = false, god = false, jumpSound = false;
	deadTime = 0.f, invulTime = 0.f;
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	engine = Sound::instance().getSoundEngine();
	sprite = Sprite::createSprite(glm::ivec2(16,32), glm::vec2(1.f/8.f, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);
	
		sprite->setAnimationSpeed(STAND_RIGHT, 6);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0 * 1.f/8.f, 0.f));

		sprite->setAnimationSpeed(STAND_LEFT, 6);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(1 * 1.f / 8.f, 0));

		sprite->setAnimationSpeed(MOVE_RIGHT, 6);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(2 * 1.f / 8.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0 * 1.f / 8.f, 0.f));
	
		sprite->setAnimationSpeed(MOVE_LEFT, 6);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(3 * 1.f / 8.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(1 * 1.f / 8.f, 0.f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 6);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(4 * 1.f / 8.f, 0.f));

		sprite->setAnimationSpeed(JUMP_LEFT, 6);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(5 * 1.f / 8.f, 0.f));

		sprite->setAnimationSpeed(DEAD_RIGHT, 6);
		sprite->addKeyframe(DEAD_RIGHT, glm::vec2(6 * 1.f / 8.f, 0.f));

		sprite->setAnimationSpeed(DEAD_LEFT, 6);
		sprite->addKeyframe(DEAD_LEFT, glm::vec2(7 * 1.f / 8.f, 0.f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (isDead) {
		posPlayer.y += FALL_STEP;
		while (!map->collisionMoveDown(posPlayer, glm::ivec2(16, 32), &posPlayer.y, true)) {
			++posPlayer.y;
		}
		//posPlayer.y -= FALL_STEP;
		invulnerable = true;
		invulTime += deltaTime;//
		if (sprite->animation() == STAND_LEFT ||
			sprite->animation() == MOVE_LEFT ||
			sprite->animation() == JUMP_LEFT)
			sprite->changeAnimation(DEAD_LEFT);
		else if (sprite->animation() == STAND_RIGHT ||
				 sprite->animation() == MOVE_RIGHT ||
				 sprite->animation() == JUMP_RIGHT)
			sprite->changeAnimation(DEAD_RIGHT);
		if (invulTime / 1000.f >= 2.f) {//estas 2 segundos muerto y respawneas
			isDead = false;//esto se haria en scene
			//creamos una funcion spawn? para las estrellitas y todo eso
			posPlayer.x = map->GetInitPlayerxTiles() * map->getTileSizex();
			posPlayer.y = map->GetInitPlayeryTiles() * map->getTileSizey();
			bJumping = false;
			sprite->changeAnimation(STAND_RIGHT);
			invulTime = 0.f;
		}
	}
	else {
		if (invulnerable && !god) {
			invulTime += deltaTime;
			if (invulTime / 1000.f >= 2.f) {
				invulTime = 0.f;
				invulnerable = false;
			}
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (bJumping || falling) {
				if (sprite->animation() != JUMP_LEFT)
					sprite->changeAnimation(JUMP_LEFT);
			}
			else {
				if (sprite->animation() != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
			}
			posPlayer.x -= 2;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(16, 32)))
			{
				posPlayer.x += 2;
				if (!bJumping && !falling) sprite->changeAnimation(STAND_LEFT);
			}

		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if (bJumping || falling) {
				if (sprite->animation() != JUMP_RIGHT)
					sprite->changeAnimation(JUMP_RIGHT);
			}
			else {
				if (sprite->animation() != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
			}
			posPlayer.x += 2;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(16, 32)))
			{
				posPlayer.x -= 2;
				if (!bJumping && !falling) sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else
		{
			if (!bJumping && !falling) {
				if (sprite->animation() == MOVE_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == MOVE_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
			}
		}

		if (bJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 132)//cuando vuelves a la y desde donde has empezado el salto (puede ser suelo o no)
			{
				posPlayer.y += FALL_STEP;
				if (map->collisionMoveDown(posPlayer, glm::ivec2(16, 32), &posPlayer.y, true))
				{
					jumpSound = false;
					if (sprite->animation() == JUMP_LEFT)
						sprite->changeAnimation(STAND_LEFT);
					else if (sprite->animation() == JUMP_RIGHT)
						sprite->changeAnimation(STAND_RIGHT);
					posPlayer.y = startY;
				}
				posPlayer.y -= FALL_STEP;
				bJumping = false;
			}
			else //cuando estas en pleno salto
			{
				posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 135.f));//el numero es 115 pero se bugea :(
				if (jumpAngle > 76) //cuando estas cayendo
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(16, 32), &posPlayer.y, true);//el salto se cancela si, mientras caes, colisionas con suelo
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(16, 32), &posPlayer.y, true)) {
				jumpSound = false;
				if (falling) falling = false;
				if (sprite->animation() == JUMP_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == JUMP_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					if (!jumpSound) {
						engine->play2D("sounds/jump.ogg");
						jumpSound = true;
					}
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
					if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT)
						sprite->changeAnimation(JUMP_LEFT);
					else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
						sprite->changeAnimation(JUMP_RIGHT);
				}
			}
			else { //cuando está cayendo sin saltar
				falling = true;
			}
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	if(int(invulTime) % 6 == 0 || !invulnerable) sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




