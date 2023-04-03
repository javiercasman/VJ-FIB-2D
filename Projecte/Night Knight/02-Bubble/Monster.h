#ifndef _MONSTER_INCLUDE
#define _MONSTER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Monster
{

public:
	void init(string type, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	glm::ivec2 getPosMonster() { return posMonster; }
private:
	string type;
	glm::ivec2 tileMapDispl, posMonster, tileSize;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	float powerCooldown, powerTime;
	ShaderProgram texProgram;
	bool up;
};

#endif // _MONSTER_INCLUDE