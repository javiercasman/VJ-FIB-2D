#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	Sound::instance().init();
	engine = Sound::instance().getSoundEngine();
	ShaderManager::instance().init();
	idMenu = 0, idScene = 1, posIndex = 0, lifes = 0, points = 0;
	bPlay = true, start = false, start_level = false;
	pauseTime = 0.f;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	menu = new Menu();
	menu->init(idMenu);
	scene = new Scene();
	scene->init(idScene);
}


bool Game::update(int deltaTime)
{
	if (start) {
		if (pauseTime / 1000.f >= 3) scene->update(deltaTime);
		else {
			pauseTime += deltaTime;
			if (!start_level && pauseTime / 1000.f <= 1.2f) {
				engine->play2D("sounds/start_level.ogg");
				start_level = true;
			}
		}
	}
	else menu->update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (start) scene->render(); 
	else menu->render();
}

void Game::loseLife()
{
	engine->play2D("sounds/hit.ogg");
	--lifes;
}

void Game::incrPoints(int add) 
{
	points += add;
}

void Game::nextLevel() {
	scene = new Scene();
	if (idScene < 3) {
		engine->stopAllSounds();
		start_level = false;
		++idScene;
		scene->init(idScene);
		pauseTime = 0.f;
	}
	else {
		engine->play2D("sounds/win.ogg");
		menu->init(2);
		start = false;
	}
}

void Game::gameover() {
	engine->stopAllSounds();
	engine->play2D("sounds/gameover.ogg");
	start = false;
	scene = new Scene();
	idMenu = 3;
	menu->init(idMenu);
}

void Game::keyPressed(int key)
{
	if (key == 27) { // Escape code.
		if(idMenu == 0) bPlay = false;
		else if (idMenu != 3) {
			engine->stopAllSounds();
			init();
		}
	}
	if (key == 13 || key == 10) {//enter
		if (idMenu == 0) {
			if (posIndex == 0) {
				idMenu = -1;
				start = true;
				lifes = 3;
			}
			else {
				idMenu = posIndex;
				menu->init(posIndex);
			}
		}
	}
	if (key == 49 || key == 50 || key == 51) { //1, 2, 3
		if (start) {
			engine->stopAllSounds();
			scene = new Scene();
			start_level = false;
			idScene = key - 48;
			scene->init(idScene);
			scene->skipCheat = true;
			pauseTime = 0.f;
		}
	}
	if (key == 71 || key == 103) {//G g
		if (start) scene->godMode();
	}
	if (key == 75 || key == 107) {//K k
		if (start) scene->keyCheat = true;
	}
	//aqui deberiamos meter las teclas esas de god mode, etc.
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	if (idMenu == 0) {
		if (key == GLUT_KEY_DOWN) {
			++posIndex;
			posIndex %= 3;
			menu->setPosIndex(posIndex);
		}
		else if (key == GLUT_KEY_UP) {
			--posIndex;
			if (posIndex < 0) posIndex = -2 * posIndex;
			posIndex %= 3;
			menu->setPosIndex(posIndex);
		}
	}
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





