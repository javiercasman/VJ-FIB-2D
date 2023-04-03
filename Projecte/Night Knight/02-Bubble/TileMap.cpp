#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"

using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile+".txt");
	prepareArrays(minCoords, program);
	minCoordsx = minCoords;
	programx = program;
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::activate(const int i, const int j)
{
	int coords = j * mapSize.x + i;
	map[coords] = 4;
	++activated;//podriamos ahcerlo desde scene, haciendo esto en  Scene::activate y el resto en esta de aquí
	prepareArrays(minCoordsx, programx);
}

bool TileMap::deactivate(const int i, const int j)
{
	int x0, x1, y;
	if (activated == nPlates) return false;
	x0 = i / tileSizex;
	x1 = (i + 32 - 1) / tileSizex;
	y = (j + 32 - 1) / tileSizey;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] == 4) {
			map[y * mapSize.x + x] = 3;
			--activated;
			prepareArrays(minCoordsx, programx);
			return true;
		}
	}
	return false;
}

void TileMap::openDoor() {
	int coords = doorCoord.y * mapSize.x + doorCoord.x;
	map[coords] = 6;
	prepareArrays(minCoordsx, programx);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;

	getline(fin, line);
	if (line.compare(0, 3, "gem") != 0)
		return false;
	else {
		string str = line.substr(4, 2);//siempre será un número de 2 digitos
		gemSpawnTime = stoi(str);
	}

	getline(fin, line);
	if (line.compare(0, 9, "stopwatch") != 0)
		return false;
	else {
		string str = line.substr(10, 2);//siempre será un número de 2 digitos
		if (str != "XX") stopwatchSpawnTime = stoi(str);//hay niveles donde no hay stopwatch
		else stopwatchSpawnTime = -1;
	}

	/*getline(fin, line);
	if (line.compare(0, 9, "xxxxitem") != 0)
		return false;
	else {
		string str = line.substr(10, 2);//siempre será un número de 2 digitos
		if (str != "XX") xxxxSpawnTime = stoi(str);
	}*/

	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSizex >> blockSize;
	tileSizey = tileSizex / 2;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	nPlates = 0;
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			else if (tile == '7') {
				map[j * mapSize.x + i] = 0;//esto se podria cambiar
				//createItem('Key', i * tileSizex, j * tileSizey);
				itemList.insert_or_assign("Key", glm::vec2(i * tileSizex, j * tileSizey));
			}
			else if (tile == '8') {//añadiremos más números según metamos más items
				map[j * mapSize.x + i] = 0;
				//createItem('Gem', i * tileSizex, j * tileSizey);
				itemList.insert_or_assign("Gem", glm::vec2(i * tileSizex, j * tileSizey));//comprobar si insert es suficiente CUANDO TENGAMOS MAS NIVELES
			}
			else if (tile == 'S') {//skeleton
				map[j * mapSize.x + i] = 0;
				Enemy en;
				en.type = "skeleton";
				en.pos[0] = i * tileSizex;
				en.pos[1] = j * tileSizey;
				enemyList.push_back(en);
			}
			else if (tile == 'V') {//vampire
				map[j * mapSize.x + i] = 0;
				Enemy en;
				en.type = "vampire";
				en.pos[0] = i * tileSizex;
				en.pos[1] = j * tileSizey;
				enemyList.push_back(en);
			}
			else if (tile == 'W') {//wizard
				map[j * mapSize.x + i] = 0;
				Enemy en;
				en.type = "wizard";
				en.pos[0] = i * tileSizex;
				en.pos[1] = j * tileSizey;
				enemyList.push_back(en);
			}
			else {
				if (tile == '3') ++nPlates;
				if (tile == '5') doorCoord = glm::ivec2(i, j);
				map[j * mapSize.x + i] = tile - int('0');
			}
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				//if (tile == 5 || tile == 6) tileSizey *= 2;
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSizex, minCoords.y + j * tileSizey);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
				//if (tile == 5 || tile == 6) tileSizey /= 2;
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSizex;
	y0 = pos.y / tileSizey;
	y1 = (pos.y + size.y - 1) / tileSizey;
	for(int y=y0; y<=y1; y++)
	{
		if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != 1 && map[y * mapSize.x + x] != 3 
			&& map[y * mapSize.x + x] != 4 && map[y * mapSize.x + x] != 5)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSizex;
	y0 = pos.y / tileSizey;
	y1 = (pos.y + size.y - 1) / tileSizey;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0 && map[y * mapSize.x + x] != 1 && map[y * mapSize.x + x] != 3 
			&& map[y * mapSize.x + x] != 4 && map[y * mapSize.x + x] != 5)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool player)
{
	int x0, x1, y;
	
	x0 = pos.x / tileSizex;
	x1 = (pos.x + size.x - 1) / tileSizex;
	y = (pos.y + size.y - 1) / tileSizey;//las piernas del jugador
	if (player) {
		for (int x = x0; x <= x1; x++)
		{
			if (map[y * mapSize.x + x] == 3) {
				activate(x, y);
			}
		}
	}
	for(int x=x0; x<=x1; x++)
	{
		if (map[y * mapSize.x + x] != 0 && ((map[y * mapSize.x + x] == 3 || map[y * mapSize.x + x] == 4)))
		{
			if(*posY - tileSizey * y + size.y <= 4)
			{
				*posY = tileSizey * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size)
{
	int x0, x1, y;

	x0 = pos.x / tileSizex;
	x1 = (pos.x + size.x - 1) / tileSizex;
	y = pos.y / tileSizey;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0 && ((map[y * mapSize.x + x] == 1 || map[y * mapSize.x + x] == 3 || map[y * mapSize.x + x] == 4)))
		{
			return true;
		}
	}
	return false;
}






























