#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "TileMapBackground.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "HUD.h"
#include "ShaderManager.h"
#include <irrKlang.h>
using namespace irrklang;


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void initItems();
	void initMonsters();
	void init(int numLvl);
	void update(int deltaTime);
	void render();
	bool collideEntity(glm::ivec2 entityPos, glm::ivec2 hitboxSize);
	void godMode() { player->godMode(); god = true; }
	bool keyCheat;
	bool skipCheat;
	//int setxxxxSpawnTime();

private:
	bool start, god;
	TileMap *map;
	TileMapBackground *map_background;
	Player *player;
	HUD* hud;
	float currentTime, pauseTime, freezeTime;
	glm::mat4 projection;
	Item *key;
	Item *gem;
	Item *stopwatch;
	Item* hourglass;
	//Item *loquesea;
	bool win, lose, next, end, playsong, once;
	int gemSpawnTime;
	int stopwatchSpawnTime;
	int hourglassSpawnTime;
	bool freeze;//para el stopwatch
	//int xxxxSpawnTime;
	int totalPlates, activatedPlates;
	vector<Monster*> monsterList;
	int remainingTime;
	ISoundEngine* engine;
};


#endif // _SCENE_INCLUDE