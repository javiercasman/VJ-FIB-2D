#ifndef _HUD_INCLUDE
#define _HUD_INCLUDE

#include <glm/glm.hpp>
#include "Sprite.h"
#include "ShaderProgram.h"
#include "ShaderManager.h"

class HUD
{
public:
	HUD();
	~HUD();

	void init(int numLvl);
	void render(bool cheatKey, bool cheatGod, bool cheatSkip);
	void renderStageClear();
	void renderReady();
	void update(int deltaTime, int lifes, int remainingTime, int points);
	void updateTex(Texture& tex, int num);
	void prepareArrays(const glm::vec2 geom[2], const glm::vec2 texCoords[2],
		GLuint &vao, GLuint &vbo, GLint &posLocation, GLint &texCoordLocation);
	void renderTexture(Texture tex, GLuint& vao, GLint& posLocation, GLint& texCoordLocation);

private:
	Texture zero, one, two, three, four, five, six, seven, eight, nine;
	float currentTime;
	GLuint vbo;
	Texture lifes; GLuint vao1; GLint posLocation1, texCoordLocation1;
	Texture lifesNum; GLuint vao2; GLint posLocation2, texCoordLocation2;
	Texture points1; GLuint vao3; GLint posLocation3, texCoordLocation3;
	Texture points2; GLuint vao4; GLint posLocation4, texCoordLocation4;
	Texture points3; GLuint vao5; GLint posLocation5, texCoordLocation5;
	Texture points4; GLuint vao6; GLint posLocation6, texCoordLocation6;
	Texture points5; GLuint vao7; GLint posLocation7, texCoordLocation7;
	Texture points6; GLuint vao8; GLint posLocation8, texCoordLocation8;
	Texture time1; GLuint vao9; GLint posLocation9, texCoordLocation9;
	Texture time2; GLuint vao10; GLint posLocation10, texCoordLocation10;
	Texture stage; GLuint vao11; GLint posLocation11, texCoordLocation11;
	Texture stageNum1; GLuint vao12; GLint posLocation12, texCoordLocation12;
	Texture stageNum2; GLuint vao13; GLint posLocation13, texCoordLocation13;
	Texture clear1; GLuint vao14; GLint posLocation14, texCoordLocation14;
	Texture clear2; GLuint vao15; GLint posLocation15, texCoordLocation15;
	Texture ready; GLuint vao16; GLint posLocation16, texCoordLocation16;
	Texture keycheat; GLuint vao17; GLint posLocation17, texCoordLocation17;
	Texture godcheat; GLuint vao18; GLint posLocation18, texCoordLocation18;
	Texture skipcheat; GLuint vao19; GLint posLocation19, texCoordLocation19;
	Texture skipnum; GLuint vao20; GLint posLocation20, texCoordLocation20;
};
#endif // _HUD_INCLUDE