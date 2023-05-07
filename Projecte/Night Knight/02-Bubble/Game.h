#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"
#include "ShaderManager.h"
#include "Sound.h"

// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	void loseLife();
	void incrPoints(int add);
	void nextLevel();
	void gameover();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	int getLifes() const { return lifes; }
	int getPoints() const { return points; }

private:
	bool bPlay, start, start_level;                // Continue to play game?
	Scene *scene;                     // Scene to render
	Menu *menu;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	int idMenu, idScene, posIndex, lifes, points;
	float pauseTime;
	ISoundEngine* engine;
};


#endif // _GAME_INCLUDE


