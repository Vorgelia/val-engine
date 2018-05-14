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
Type variableGetter/Setter()

----Arbitrary to-do list----
--Engine Features
TODO: Multithread rendering
TODO: Add more customization over cleaning up framebuffers between frames.
TODO: Check if a framebuffer was used last frame before cleaning it up.
TODO: Service for managing the type of the current build [debug, release]

--Code Cleanup
TODO: Replace some defines with service members [*].
TODO: ValScript is a horrible mess. Do something about it.

----Important defines----
--Engine--
Resource.cpp:           VE_CREATE_DEFAULT_RESOURCES
InputDevice.cpp:        VE_INPUT_BUFFER_INIT*
-                       VE_INPUT_BUFFER_MID*
Rendering.cpp:          VE_AUX_BUFFER_AMOUNT
-                       VE_WORLD_SCALE
-                       VE_FONT_DEFAULT
Screen.h:               VE_USE_SINGLE_BUFFER
Time.h:                 VE_FRAME_TIME*
-                       VE_FRAME_RATE*
Debug.h:		        VE_DEBUG_ERRORTHROW*
ScriptParsingUtils.cpp: VE_TAB_SPACE_AMOUNT

--Game--
PlayerManager.cpp:      VE_MAX_PLAYERS
-                       VE_MAX_SPECTATORS
*/

#include <clocale>
#include "ValEngine.h"
#include "GLIncludes.hpp"
#include "ServiceManager.h"
#include "Screen.h"

int main()
{
	//We're setting the numeric locale to default C. Different locales have different ways of signifying decimal points, so converting text into variables can be unpredictable.
	//This just makes it so that decimal point is always a dot, on both input and output.
	std::setlocale(LC_NUMERIC, "C");

	ServiceManager serviceManager;
	serviceManager.InitializeServices();

	Screen* screen = serviceManager.Screen();

	FixedPoint64 f64 = FixedPoint64::FromString("-5.142555");

	while(!glfwWindowShouldClose(screen->window))
	{
		serviceManager.UpdateServices();

		if(glfwGetKey(screen->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(screen->window, GLFW_TRUE);
		}
	}

	serviceManager.CleanupServices();
}