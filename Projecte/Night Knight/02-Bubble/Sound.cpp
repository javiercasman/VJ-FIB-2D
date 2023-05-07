#include "Sound.h"

Sound::Sound() {
	engine = NULL;
}

Sound::~Sound() {
	engine->drop();
}

void Sound::init() {
	engine = createIrrKlangDevice();
}

ISoundEngine* Sound::getSoundEngine() {
	return engine;
}