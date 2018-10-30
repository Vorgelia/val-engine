/*
Val Engine

-@Vorgelia
https://valerie-laine-dev.com
Specialized for Fighting Games

Libraries used:
GLM http://glm.g-truc.net
GLEW http://glew.sourceforge.net/
GLFW http://www.glfw.org/
SOIL http://www.lonesock.net/soil.html
Boost http://www.boost.org/
Freetype 2 https://www.freetype.org/
FMTlib http://fmtlib.net
Nlohmann JSON https://github.com/nlohmann/json
FixPointCS https://github.com/XMunkki/FixPointCS

----Conventions----
#define DEFINE_NAME
class ClassName
private/protected: Type _variableName
public: Type variableName
Type FunctionName
Type variableGetter()

----Arbitrary to-do list----
--Engine Features
TODO: Multithread rendering
TODO: Add more customization over cleaning up framebuffers between frames.
TODO: Check if a framebuffer was used last frame before cleaning it up.
TODO: Service for managing the type of the current build [debug, release]

--Code Cleanup
TODO: ValScript is a horrible mess. Do something about it.
	---For starters, remove the std::shared_ptr madness.
*/


#include <clocale>
#include "ValEngine.h"
#include "GLIncludes.hpp"
#include "GameInstance.h"
#include "Screen.h"

int main()
{
	std::setlocale(LC_NUMERIC, "C");

	ve::unique_object_ptr<GameInstance> gameInstance = ve::unique_object_ptr<GameInstance>(new GameInstance());
	ObjectFactory::InitializeObject(gameInstance.get(), nullptr);

	Screen* screen = gameInstance->Screen();

	while(!glfwWindowShouldClose(screen->window))
	{
		gameInstance->UpdateServices();

		if(glfwGetKey(screen->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(screen->window, GLFW_TRUE);
		}
	}
}
