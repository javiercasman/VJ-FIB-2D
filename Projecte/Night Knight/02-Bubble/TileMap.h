#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <map>
#include <vector>


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

struct Enemy {
	string type;
	glm::ivec2 pos;
};


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void activate(const int i, const int j);
	bool deactivate(const int i, const int j);
	void free();
	
	int getTileSizex() const { return tileSizex; }
	int getTileSizey() const { return tileSizey; }
	int getTotalPlates() const { return nPlates; } //nos servir� para determinar cuando spawnear la llave
	std::vector<Enemy> TileMap::getEnemies() const { return enemyList; }

	int getGemSpawnTime() const { return gemSpawnTime; }
	int getStopwatchSpawnTime() const { return stopwatchSpawnTime; }
	//int getxxxxSpawnTime() const { return xxxxSpawnTime; }

	int getActivatedPlates() const { return activated; }

	std::map<string, glm::ivec2> getItemList() const { return itemList; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool player);
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size); //para el wizard

	void openDoor();
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	//GLuint vao2;
	//GLuint vbo2;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize, doorCoord;
	int tileSizex, tileSizey, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int* map;
	//int *mapItems;
	int nPlates, activated = 0;
	glm::vec2 &minCoordsx = glm::vec2(0,0);
	ShaderProgram& programx = ShaderProgram();//habra que cambiarlo
	std::map<string, glm::ivec2> itemList;
	vector<Enemy> enemyList;

	int gemSpawnTime;
	int stopwatchSpawnTime;
	//int xxxxSpawnTime;

};


#endif // _TILE_MAP_INCLUDE


