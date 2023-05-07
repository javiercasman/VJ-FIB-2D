#ifndef _SHADER_MANAGER_INCLUDE
#define _SHADER_MANAGER_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//ShaderManager is a singleton to init and render the Shader Program

class ShaderManager
{
public:
	static ShaderManager& instance() {
		static ShaderManager SM;
		return SM;
	}
	void init();
	ShaderProgram& getShaderProgram() { return texProgram; }
	void render();

private:
	ShaderProgram texProgram;
	glm::mat4 projection;
};

#endif //_SHADER_MANAGER_INCLUDE