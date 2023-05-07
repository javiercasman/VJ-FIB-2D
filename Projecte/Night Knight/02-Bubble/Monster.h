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
	glm::ivec2 getTileSize() { return tileSize; }
	glm::ivec2 getHitboxSize() { return hitboxSize; }
private:
	string type;
	glm::ivec2 tileMapDispl, posMonster, tileSize, hitboxSize;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	float powerCooldown, powerTime, pauseTime;
	ShaderProgram texProgram;
	bool up, casting;
};

#endif // _MONSTER_INCLUDE