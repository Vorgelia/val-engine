/*
Val Engine
by Vorgelia
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
Libraries planned to be used:
IrrKlang

----Arbitrary to-do list----
-Engine Features
TODO: Find a way to multithread input so inputs are received and timed properly on sub-60FPS.
TODO: Add more customization over cleaning up framebuffers between frames.
TODO: Check if a framebuffer was used last frame before cleaning it up.
----
Important defines:
--Engine--
Resource.cpp:           VE_CREATE_DEFAULT_RESOURCES
InputDevice.cpp:        VE_INPUT_BUFFER_INIT
-                       VE_INPUT_BUFFER_MID
Rendering.cpp:          VE_AUX_BUFFER_AMOUNT
-                       VE_WORLD_SCALE
-                       VE_FONT_DEFAULT
Screen.h:               VE_USE_SINGLE_BUFFER
Time.h:                 VE_FRAME_TIME
-                       VE_FRAME_RATE
Debug.h:		        VE_DEBUG_ERRORTHROW
ScriptParsingUtils.cpp: VE_TAB_SPACE_AMOUNT

--Game--
PlayerManager.cpp:      VE_MAX_PLAYERS
-                       VE_MAX_SPECTATORS
*/

#include <clocale>
#include "GLIncludes.hpp"
#include "ServiceManager.h"
#include "GameSceneManager.h"
#include "Screen.h"

int main()
{
	//We're setting the numeric locale to default C. Different locales have different ways of signifying decimal points, so converting text into variables can be unpredictable.
	//This just makes it so that decimal point is always a dot, on both input and output.
	std::setlocale(LC_NUMERIC, "C");

	ServiceManager serviceManager;
	serviceManager.InitializeServices();

	Screen* screen = serviceManager.Screen();

	while(!glfwWindowShouldClose(screen->window))
	{
		serviceManager.UpdateServices();

		if(glfwGetKey(screen->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(screen->window, GLFW_TRUE);
		}

		if(glfwGetKey(screen->window, GLFW_KEY_F2))
		{
			serviceManager.GameSceneManager()->ReloadScene();
		}
	}

	serviceManager.CleanupServices();
}