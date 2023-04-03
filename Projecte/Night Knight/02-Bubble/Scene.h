#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "TileMapBackground.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void initItems();
	void initMonsters();
	void init();
	void update(int deltaTime);
	void render();
	bool collideEntity(glm::ivec2 entityPos, glm::ivec2 tileSize);
	//int setxxxxSpawnTime();

private:
	void initShaders();

private:
	TileMap *map;
	TileMapBackground *map_background;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Item *key;
	Item *gem;
	Item *stopwatch;
	//Item *loquesea;

	int gemSpawnTime;
	int stopwatchSpawnTime;
	//int xxxxSpawnTime;
	int totalPlates, activatedPlates;
	std::vector<Monster*> monsterList;

};


#endif // _SCENE_INCLUDE

