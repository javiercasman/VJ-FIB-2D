#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 64
#define SCREEN_Y 80

#define INIT_PLAYER_X_TILES 3
#define INIT_PLAYER_Y_TILES 18


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
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
			key->init(type, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			key->setPosition(pos);
			key->setTileMap(map);
			key->timeOnScreen = 0.f;//no sirve pero bueno
		}
		else if (type == "Gem") {
			gem = new Item();
			gem->init(type, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			gem->setPosition(pos);
			gem->setTileMap(map);
			gem->timeOnScreen = 0.f;
		}
		else if (type == "Stopwatch") {
			stopwatch = new Item();
			stopwatch->init(type, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			stopwatch->setPosition(pos);
			stopwatch->setTileMap(map);
			stopwatch->timeOnScreen = 0.f;
		}
	}
}

void Scene::initMonsters() {
	vector<Enemy> enemyList = map->getEnemies();
	for (int i = 0; i < enemyList.size(); ++i) {
		Monster* m = new Monster();//esto se hace asi? no se
		m->init(enemyList[i].type, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		m->setPosition(enemyList[i].pos);
		m->setTileMap(map);
		monsterList.push_back(m);
	}
}

void Scene::init()
{
	initShaders();


	map = TileMap::createTileMap("levels/level01", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);//tendremos que pasar el string del file por parametro a scene init()

	gemSpawnTime = map->getGemSpawnTime();
	stopwatchSpawnTime = map->getStopwatchSpawnTime();
	//xxxxSpawnTime = map->getgetxxxxSpawnTime();
	totalPlates = map->getTotalPlates();

	map_background = TileMapBackground::createTileMapBackground("levels/level01_bckgrnd.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);//aqui igual
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSizex(), INIT_PLAYER_Y_TILES * map->getTileSizey()));
	player->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	initItems();
	initMonsters();
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	activatedPlates = map->getActivatedPlates();
	player->update(deltaTime);
	for (int i = 0; i < monsterList.size(); ++i) {
		monsterList[i]->update(deltaTime);
	}
	if (collideEntity(key->getPos(), key->getTileSize()) && key->isOnScreen() && !key->isGrabbed()) {
		key->free();
		key->setGrab();
		map->openDoor();
	}
	if (gem->isOnScreen() && !gem->isGrabbed()) {
		gem->timeOnScreen += deltaTime;
		if (collideEntity(gem->getPos(), gem->getTileSize())) {
			gem->setGrab();
			gem->free();
		}
		else if (gem->timeOnScreen / 1000.f >= 10) gem->free();
	}
}

bool Scene::collideEntity(glm::ivec2 entityPos, glm::ivec2 tileSize) {
	glm::ivec2 PosPlayer = player->getPosPlayer();
	int x0, x1, y0, y1;
	x0 = entityPos.x;
	x1 = x0 + tileSize.x;
	y0 = entityPos.y;
	y1 = y0 - tileSize.y;
	if (((PosPlayer.x >= x0 && PosPlayer.x <= x1) || (PosPlayer.x + 16 >=x0 && PosPlayer.x+16 <= x1)) && PosPlayer.y <= y0 && PosPlayer.y >= y1) return true;
	else return false;
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	map_background->render();
	player->render();
	for (int i = 0; i < monsterList.size(); ++i) {
		monsterList[i]->render();
	}
	if (currentTime / 1000.f >= gemSpawnTime && !gem->isGrabbed() && gem->timeOnScreen / 1000.f < 10) {
		gem->render();
	}
	if (activatedPlates == totalPlates && !key->isGrabbed()) {
		key->render();
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



