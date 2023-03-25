#ifndef _TILE_MAP_BACKGROUND_INCLUDE
#define _TILE_MAP_BACKGROUND_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

class TileMapBackground
{
private:
	TileMapBackground(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

public:
	static TileMapBackground* createTileMapBackground(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	~TileMapBackground();

	void render() const;
	void free();

private:
	bool loadLevel(const string& levelFile);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSizex, tileSizey, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int* map;
};

#endif // _TILE_MAP_INCLUDE