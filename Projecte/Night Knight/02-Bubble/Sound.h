#ifndef _SOUND_INCLUDE
#define _SOUND_INCLUDE

#include <irrKlang.h>
using namespace irrklang;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class Sound
{
public:
	Sound();
	~Sound();
	static Sound& instance() {
		static Sound S;
		return S;
	}

	void init();
	ISoundEngine* getSoundEngine();

private:
	ISoundEngine* engine;
};

#endif // _SOUND_INCLUDE