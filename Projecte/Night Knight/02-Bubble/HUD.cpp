#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "HUD.h"

HUD::HUD()
{
	currentTime = 0.f;

	glm::vec2 geom[2] = { glm::vec2(63.f, 35.f), glm::vec2(95.f, 51.f) };//32x16
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	prepareArrays(geom, texCoords, vao1, vbo, posLocation1, texCoordLocation1);

	geom[0] = glm::vec2(97.f, 35.f);
	geom[1] = glm::vec2(113.f, 51.f);
	prepareArrays(geom, texCoords, vao2, vbo, posLocation2, texCoordLocation2);

	geom[0] = glm::vec2(129.f, 35.f);
	geom[1] = glm::vec2(145.f, 51.f);
	prepareArrays(geom, texCoords, vao3, vbo, posLocation3, texCoordLocation3);

	geom[0] = glm::vec2(146.f, 35.f);
	geom[1] = glm::vec2(162.f, 51.f);
	prepareArrays(geom, texCoords, vao4, vbo, posLocation4, texCoordLocation4);

	geom[0] = glm::vec2(163.f, 35.f);
	geom[1] = glm::vec2(179.f, 51.f);
	prepareArrays(geom, texCoords, vao5, vbo, posLocation5, texCoordLocation5);

	geom[0] = glm::vec2(180.f, 35.f);
	geom[1] = glm::vec2(196.f, 51.f);
	prepareArrays(geom, texCoords, vao6, vbo, posLocation6, texCoordLocation6);

	geom[0] = glm::vec2(197.f, 35.f);
	geom[1] = glm::vec2(213.f, 51.f);
	prepareArrays(geom, texCoords, vao7, vbo, posLocation7, texCoordLocation7);

	geom[0] = glm::vec2(214.f, 35.f);
	geom[1] = glm::vec2(230.f, 51.f);
	prepareArrays(geom, texCoords, vao8, vbo, posLocation8, texCoordLocation8);

	geom[0] = glm::vec2(303.f, 35.f);
	geom[1] = glm::vec2(319.f, 51.f);
	prepareArrays(geom, texCoords, vao9, vbo, posLocation9, texCoordLocation9);

	geom[0] = glm::vec2(320.f, 35.f);
	geom[1] = glm::vec2(336.f, 51.f);
	prepareArrays(geom, texCoords, vao10, vbo, posLocation10, texCoordLocation10);

	geom[0] = glm::vec2(448.f, 35.f);
	geom[1] = glm::vec2(544.f, 51.f);
	prepareArrays(geom, texCoords, vao11, vbo, posLocation11, texCoordLocation11);

	geom[0] = glm::vec2(544.f, 35.f);
	geom[1] = glm::vec2(560.f, 51.f);
	prepareArrays(geom, texCoords, vao12, vbo, posLocation12, texCoordLocation12);

	geom[0] = glm::vec2(561.f, 35.f);
	geom[1] = glm::vec2(576.f, 51.f);
	prepareArrays(geom, texCoords, vao13, vbo, posLocation13, texCoordLocation13);

	geom[0] = glm::vec2(230.f, 240.f);
	geom[1] = glm::vec2(326.f, 256.f);
	prepareArrays(geom, texCoords, vao14, vbo, posLocation14, texCoordLocation14);

	geom[0] = glm::vec2(326.f, 240.f);
	geom[1] = glm::vec2(408.f, 256.f);
	prepareArrays(geom, texCoords, vao15, vbo, posLocation15, texCoordLocation15);

	geom[0] = glm::vec2(270.f, 240.f);
	geom[1] = glm::vec2(366.f, 256.f);
	prepareArrays(geom, texCoords, vao16, vbo, posLocation16, texCoordLocation16);

	geom[0] = glm::vec2(65.f, 436.f);
	geom[1] = glm::vec2(209.f, 452.f);
	prepareArrays(geom, texCoords, vao17, vbo, posLocation17, texCoordLocation17);

	geom[0] = glm::vec2(255.f, 436.f);
	geom[1] = glm::vec2(382.f, 452.f);
	prepareArrays(geom, texCoords, vao18, vbo, posLocation18, texCoordLocation18);

	geom[0] = glm::vec2(480.f, 436.f);
	geom[1] = glm::vec2(543.f, 452.f);
	prepareArrays(geom, texCoords, vao19, vbo, posLocation19, texCoordLocation19);

	geom[0] = glm::vec2(560.f, 436.f);
	geom[1] = glm::vec2(576.f, 452.f);
	prepareArrays(geom, texCoords, vao20, vbo, posLocation20, texCoordLocation20);

	zero.loadFromFile("images/tiles/0.png", TEXTURE_PIXEL_FORMAT_RGBA);
	one.loadFromFile("images/tiles/1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	two.loadFromFile("images/tiles/2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	three.loadFromFile("images/tiles/3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	four.loadFromFile("images/tiles/4.png", TEXTURE_PIXEL_FORMAT_RGBA);
	five.loadFromFile("images/tiles/5.png", TEXTURE_PIXEL_FORMAT_RGBA);
	six.loadFromFile("images/tiles/6.png", TEXTURE_PIXEL_FORMAT_RGBA);
	seven.loadFromFile("images/tiles/7.png", TEXTURE_PIXEL_FORMAT_RGBA);
	eight.loadFromFile("images/tiles/8.png", TEXTURE_PIXEL_FORMAT_RGBA);
	nine.loadFromFile("images/tiles/9.png", TEXTURE_PIXEL_FORMAT_RGBA);
	lifes.loadFromFile("images/tiles/lifes.png", TEXTURE_PIXEL_FORMAT_RGBA);
	stage.loadFromFile("images/tiles/stage.png", TEXTURE_PIXEL_FORMAT_RGBA);
	clear1.loadFromFile("images/tiles/stage.png", TEXTURE_PIXEL_FORMAT_RGBA);
	clear2.loadFromFile("images/tiles/clear.png", TEXTURE_PIXEL_FORMAT_RGBA);
	ready.loadFromFile("images/tiles/ready.png", TEXTURE_PIXEL_FORMAT_RGBA);
	keycheat.loadFromFile("images/tiles/keycheat.png", TEXTURE_PIXEL_FORMAT_RGBA);
	godcheat.loadFromFile("images/tiles/godmode.png", TEXTURE_PIXEL_FORMAT_RGBA);
	skipcheat.loadFromFile("images/tiles/skip.png", TEXTURE_PIXEL_FORMAT_RGBA);
}

HUD::~HUD()
{
}

void HUD::init(int numLvl)
{
	lifesNum = three;
	points1 = zero;
	points2 = zero;
	points3 = zero;
	points4 = zero;
	points5 = zero;
	points6 = zero;
	time1 = six;
	time2 = zero;
	stageNum1 = zero;
	if (numLvl == 1) {
		stageNum2 = one;
		skipnum = one;
	}
	else if (numLvl == 2) {
		stageNum2 = two;
		skipnum = two;
	}
	else if (numLvl == 3) {
		stageNum2 = three;
		skipnum = three;
	}
}

void HUD::render(bool cheatKey, bool cheatGod, bool cheatSkip)
{
	renderTexture(lifes, vao1, posLocation1, texCoordLocation1);
	renderTexture(lifesNum, vao2, posLocation2, texCoordLocation2);
	renderTexture(points1, vao3, posLocation3, texCoordLocation3);
	renderTexture(points2, vao4, posLocation4, texCoordLocation4);
	renderTexture(points3, vao5, posLocation5, texCoordLocation5);
	renderTexture(points4, vao6, posLocation6, texCoordLocation6);
	renderTexture(points5, vao7, posLocation7, texCoordLocation7);
	renderTexture(points6, vao8, posLocation8, texCoordLocation8);
	renderTexture(time1, vao9, posLocation9, texCoordLocation9);
	renderTexture(time2, vao10, posLocation10, texCoordLocation10);
	renderTexture(stage, vao11, posLocation11, texCoordLocation11);
	renderTexture(stageNum1, vao12, posLocation12, texCoordLocation12);
	renderTexture(stageNum2, vao13, posLocation13, texCoordLocation13);
	if(cheatKey) renderTexture(keycheat, vao17, posLocation17, texCoordLocation17);
	if(cheatGod) renderTexture(godcheat, vao18, posLocation18, texCoordLocation18);
	if (cheatSkip) {
		renderTexture(skipcheat, vao19, posLocation19, texCoordLocation19);
		renderTexture(skipnum, vao20, posLocation20, texCoordLocation20);
	}
}

void HUD::renderStageClear()
{
	renderTexture(clear1, vao14, posLocation14, texCoordLocation14);
	renderTexture(clear2, vao15, posLocation15, texCoordLocation15);
}

void HUD::renderReady() {
	renderTexture(ready, vao16, posLocation16, texCoordLocation16);
}

void HUD::update(int deltaTime, int lifes, int remainingTime, int points)
{
	currentTime += deltaTime;
	updateTex(lifesNum, lifes);
	int point1 = points / 100000;
	int point2 = (points%100000) / 10000;
	int point3 = (points%10000) / 1000;
	int point4 = (points%1000) / 100;
	int point5 = (points%100) / 10;
	int point6 = points % 10;
	updateTex(points1, point1);
	updateTex(points2, point2);
	updateTex(points3, point3);
	updateTex(points4, point4);
	updateTex(points5, point5);
	updateTex(points6, point6);
	int t1 = remainingTime / 10;
	int t2 = remainingTime % 10;
	updateTex(time1, t1);
	updateTex(time2, t2);
}

void HUD::updateTex(Texture& tex, int num)
{
	if (num == 0) tex = zero;
	else if (num == 1) tex = one;
	else if (num == 2) tex = two;
	else if (num == 3) tex = three;
	else if (num == 4) tex = four;
	else if (num == 5) tex = five;
	else if (num == 6) tex = six;
	else if (num == 7) tex = seven;
	else if (num == 8) tex = eight;
	else if (num == 9) tex = nine;
}

void HUD::prepareArrays(const glm::vec2 geom[2], const glm::vec2 texCoords[2], 
	GLuint &vao, GLuint &vbo, GLint &posLocation, GLint &texCoordLocation)
{
	vector<float> vertices;
	vertices.push_back(geom[0].x); vertices.push_back(geom[0].y);
	vertices.push_back(texCoords[0].x); vertices.push_back(texCoords[0].y);
	vertices.push_back(geom[1].x); vertices.push_back(geom[0].y);
	vertices.push_back(texCoords[1].x); vertices.push_back(texCoords[0].y);
	vertices.push_back(geom[1].x); vertices.push_back(geom[1].y);
	vertices.push_back(texCoords[1].x); vertices.push_back(texCoords[1].y);

	vertices.push_back(geom[0].x); vertices.push_back(geom[0].y);
	vertices.push_back(texCoords[0].x); vertices.push_back(texCoords[0].y);
	vertices.push_back(geom[1].x); vertices.push_back(geom[1].y);
	vertices.push_back(texCoords[1].x); vertices.push_back(texCoords[1].y);
	vertices.push_back(geom[0].x); vertices.push_back(geom[1].y);
	vertices.push_back(texCoords[0].x); vertices.push_back(texCoords[1].y);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = ShaderManager::instance().getShaderProgram().bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = ShaderManager::instance().getShaderProgram().bindVertexAttribute("texCoord", 2, 4 * sizeof(float),
		(void*)(2 * sizeof(float)));
}

void HUD::renderTexture(Texture tex, GLuint &vao, GLint& posLocation, GLint& texCoordLocation) {
	tex.setWrapS(GL_CLAMP_TO_EDGE);
	tex.setWrapT(GL_CLAMP_TO_EDGE);
	tex.setMinFilter(GL_NEAREST);
	tex.setMagFilter(GL_NEAREST);
	glEnable(GL_TEXTURE_2D);
	tex.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}