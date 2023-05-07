#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 64
#define SCREEN_Y 80
#define FALL_STEP 4

Scene::Scene()
{
	map = NULL;
	player = NULL;
	map_background = NULL;
	key = NULL;
	gem = NULL;
	stopwatch = NULL;
	hourglass = NULL;
	engine = Sound::instance().getSoundEngine();
	hud = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (map_background != NULL)
		delete map_background;
	if (key != NULL)
		delete key;
	if (gem != NULL)
		delete gem;
	if (stopwatch != NULL)
		delete stopwatch;
	if (hourglass != NULL)
		delete hourglass;
	if (hud != NULL)
		delete hud;
	/*for (auto p : monsterList) delete p;
	monsterList.clear();*/
}

void Scene::initItems()
{
	std::map<string, glm::ivec2> itemList = map->getItemList();
	for (auto const& it : itemList) {
		string type = it.first;
		glm::ivec2 pos = it.second;
		int i = pos.x;
		int j = pos.y;
		if (type == "Key") {
			key = new Item();
			key->init(type, glm::ivec2(SCREEN_X, SCREEN_Y), ShaderManager::instance().getShaderProgram());
			key->setPosition(pos);
			key->setTileMap(map);
			key->timeOnScreen = 0.f;//no sirve pero bueno
		}
		else if (type == "Gem") {
			gem = new Item();
			gem->init(type, glm::ivec2(SCREEN_X, SCREEN_Y), ShaderManager::instance().getShaderProgram());
			gem->setPosition(pos);
			gem->setTileMap(map);
			gem->timeOnScreen = 0.f;
		}
		else if (type == "Stopwatch") {
			stopwatch = new Item();
			stopwatch->init(type, glm::ivec2(SCREEN_X, SCREEN_Y), ShaderManager::instance().getShaderProgram());
			stopwatch->setPosition(pos);
			stopwatch->setTileMap(map);
			stopwatch->timeOnScreen = 0.f;
		}
		else if (type == "Hourlgass") {
			hourglass = new Item();
			hourglass->init(type, glm::ivec2(SCREEN_X, SCREEN_Y), ShaderManager::instance().getShaderProgram());
			hourglass->setPosition(pos);
			hourglass->setTileMap(map);
			hourglass->timeOnScreen = 0.f;
		}
	}
}

void Scene::initMonsters() {
	vector<Enemy> enemyList = map->getEnemies();
	for (int i = 0; i < enemyList.size(); ++i) {
		Monster* m = new Monster();
		m->init(enemyList[i].type, glm::ivec2(SCREEN_X, SCREEN_Y), ShaderManager::instance().getShaderProgram());
		m->setPosition(enemyList[i].pos);
		m->setTileMap(map);
		monsterList.push_back(m);
	}
}

void Scene::init(int numLvl)
{
	map = TileMap::createTileMap("levels/level0"+to_string(numLvl), glm::vec2(SCREEN_X, SCREEN_Y), ShaderManager::instance().getShaderProgram());//tendremos que pasar el string del file por parametro a scene init()

	gemSpawnTime = map->getGemSpawnTime();
	stopwatchSpawnTime = map->getStopwatchSpawnTime();
	hourglassSpawnTime = map->getHourglassSpawnTime();
	totalPlates = map->getTotalPlates();

	map_background = TileMapBackground::createTileMapBackground("levels/level0" + to_string(numLvl)+"_bckgrnd.txt", glm::vec2(SCREEN_X, SCREEN_Y), ShaderManager::instance().getShaderProgram());//aqui igual
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), ShaderManager::instance().getShaderProgram());
	player->setPosition(glm::vec2(map->GetInitPlayerxTiles() * map->getTileSizex(), map->GetInitPlayeryTiles() * map->getTileSizey()));
	player->setTileMap(map);
	initItems();
	initMonsters();
	hud = new HUD();
	hud->init(numLvl);
	currentTime = 0.f, pauseTime = 0.f, freezeTime = 0.f;
	once = false;
	win = false, lose = false, next = false, end = false, keyCheat = false, start = false, freeze = false, god = false, skipCheat = false, playsong = false;
}

void Scene::update(int deltaTime)
{
	if (!playsong) {
		irrklang::ISound* sound = engine->play2D("sounds/level.ogg", true, false, true);
		sound->setVolume(0.2f);
		playsong = true;
	}
	start = true;
	hud->update(deltaTime, Game::instance().getLifes(), remainingTime, Game::instance().getPoints());
	if(!freeze) currentTime += deltaTime;
	if (currentTime / 1000.f >= 60.f) lose = true;
	if (!win && !lose) {
		remainingTime = 60 - (currentTime / 1000.f);
		activatedPlates = map->getActivatedPlates();
		player->update(deltaTime);
		if (key->isGrabbed() && collideEntity(glm::ivec2(map->getDoorCoords().x * map->getTileSizex(), map->getDoorCoords().y * map->getTileSizey()), glm::ivec2(32, 16)))
			win = true;
		for (int i = 0; i < monsterList.size(); ++i) {
			if(!freeze) monsterList[i]->update(deltaTime);
			if (collideEntity(monsterList[i]->getPosMonster(), monsterList[i]->getHitboxSize()) && !player->isInvulerable()) {
				player->setDead(true);
				Game::instance().loseLife();
				if (Game::instance().getLifes() == 0) lose = true;
			}
		}
		glm::ivec2 posplayer = player->getPosPlayer();
		posplayer.y += FALL_STEP;
		if (map->collisionSpikes(posplayer, glm::ivec2(16, 32), &posplayer.y) && !player->isInvulerable()) {
			player->setDead(true);
			Game::instance().loseLife();
			if (Game::instance().getLifes() == 0) lose = true;
		}
		posplayer.y -= FALL_STEP;
		if (collideEntity(key->getPos(), key->getTileSize()) && key->isOnScreen() && !key->isGrabbed()) {
			key->free();
			key->setGrab();
			engine->play2D("sounds/key.ogg");
			map->openDoor();
		}
		if (gem->isOnScreen() && !gem->isGrabbed()) {
			if(!freeze) gem->timeOnScreen += deltaTime;
			if (collideEntity(gem->getPos(), gem->getTileSize())) {
				gem->setGrab();
				engine->play2D("sounds/gem.ogg");
				gem->free();
				Game::instance().incrPoints(500);
			}
			else if (gem->timeOnScreen / 1000.f >= 10) gem->free();
		}
		if (stopwatch != NULL && stopwatch->isOnScreen() && !stopwatch->isGrabbed()) {
			if (!freeze) stopwatch->timeOnScreen += deltaTime;
			if (collideEntity(stopwatch->getPos(), stopwatch->getTileSize())) {
				stopwatch->setGrab();
				engine->play2D("sounds/stopwatch.ogg");
				stopwatch->free();
				freeze = true;
			}
			else if (stopwatch->timeOnScreen / 1000.f >= 10) stopwatch->free();
		}
		if (hourglass != NULL && hourglass->isOnScreen() && !hourglass->isGrabbed()) {
			if (!freeze) hourglass->timeOnScreen += deltaTime;
			if (collideEntity(hourglass->getPos(), hourglass->getTileSize())) {
				hourglass->setGrab();
				engine->play2D("sounds/hourglass.ogg");
				hourglass->free();
				currentTime = 0;
			}
			else if (hourglass->timeOnScreen / 1000.f >= 10) hourglass->free();
		}
	}
	else if (win && !next) {
		if (pauseTime / 1000.f >= 1.5f) {
			if (int(currentTime) % 4 == 0 && remainingTime > 0) {
				--remainingTime;
				Game::instance().incrPoints(10);
				engine->play2D("sounds/beep.ogg");
			}
			if (remainingTime == 0) {
				if (pauseTime / 1000.f > 2.5f) next = true;
				else pauseTime += deltaTime;
			}
		}
		else pauseTime += deltaTime;
	}
	else if (lose && !end) {
		if (pauseTime / 1000.f >= 2.f) {
			end = true;
		}
		else pauseTime += deltaTime;
	}
	if (freeze) {
		if (freezeTime / 1000.f >= 4.5f) {
			freeze = false;
			freezeTime = 0;
		}
		else freezeTime += deltaTime;
	}
}

bool Scene::collideEntity(glm::ivec2 entityPos, glm::ivec2 hitboxSize) {
	glm::ivec2 PosPlayer = player->getPosPlayer();
	int x0, x1, y0, y1;
	x0 = entityPos.x;
	x1 = x0 + hitboxSize.x;
	y0 = entityPos.y;
	y1 = y0 - hitboxSize.y;
	if (((PosPlayer.x >= x0 && PosPlayer.x <= x1) || (PosPlayer.x + 16 >=x0 && PosPlayer.x+16 <= x1)) &&
		((PosPlayer.y <= y0 && PosPlayer.y >= y1) || (PosPlayer.y - 32 <= y0 && PosPlayer.y - 32 >= y1))) return true;
	else return false;
}

void Scene::render()
{
	ShaderManager::instance().render();
	map->render();
	map_background->render();
	hud->render(keyCheat, god, skipCheat);
	if (!start) hud->renderReady();
	if (win) {
		if (!once) {
			engine->stopAllSounds();
			once = true;
		}
		hud->renderStageClear();
	}
	if(start && !win && !lose) player->render();
	for (int i = 0; i < monsterList.size(); ++i) {
		monsterList[i]->render();
	}
	if (currentTime / 1000.f >= gemSpawnTime && !gem->isGrabbed() && gem->timeOnScreen / 1000.f < 10 && start && !win && !lose) {
		gem->render();
	}
	if (stopwatch != NULL && currentTime / 1000.f >= stopwatchSpawnTime && !stopwatch->isGrabbed() && stopwatch->timeOnScreen / 1000.f < 10 && start && !win && !lose) {
		stopwatch->render();
	}
	if (hourglass != NULL && currentTime / 1000.f >= hourglassSpawnTime && !hourglass->isGrabbed() && hourglass->timeOnScreen / 1000.f < 10 && start && !win && !lose) {
		hourglass->render();
	}
	if ((activatedPlates == totalPlates || keyCheat) && !key->isGrabbed() && start && !win && !lose) {
		key->render();
	}
	if (next) {
		Game::instance().nextLevel();
		return;
	}
	if (end) {
		Game::instance().gameover();
		return;
	}
}