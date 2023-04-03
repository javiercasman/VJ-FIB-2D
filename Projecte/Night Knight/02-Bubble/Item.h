#ifndef ITEM_INCLUDE
#define ITEM_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Item
{
public:

	void init(string type, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);//es necesario?
	void render();
	void free();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void setGrab() { grabbed = true; }

	bool isOnScreen() { return onScreen; }
	bool isGrabbed() { return grabbed; }

	string getType() const { return type; }//necesario? creo q no
	glm::vec2 getPos() const { return posItem; }//necesario? creo q no
	glm::vec2 getTileSize() const { return tileSize; }
	float timeOnScreen;

private:

private:
	string type;//se puede quitar?
	glm::vec2 tileMapDispl, posItem, tileSize;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	bool onScreen;
	bool grabbed;
};

#endif // _ITEM_INCLUDE