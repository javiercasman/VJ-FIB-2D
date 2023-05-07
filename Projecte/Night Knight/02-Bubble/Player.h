#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Sound.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setDead(bool dead) { isDead = dead; }
	void godMode() { invulnerable = true, god = true; }

	glm::ivec2 getPosPlayer() { return posPlayer; }
	bool isInvulerable() { return invulnerable; }
	
private:
	bool bJumping;
	bool falling;
	bool isDead, invulnerable, god;
	glm::ivec2 tileMapDispl, posPlayer;
	int startY;
	int jumpAngle;
	float deadTime, invulTime;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	ISoundEngine* engine;
	bool jumpSound;
};


#endif // _PLAYER_INCLUDE


