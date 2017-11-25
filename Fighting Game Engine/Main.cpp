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

Structured to depend on global variables and functions in namespaces.
Having global access to things like Input, Time, OpenGL states, etc was the most intuitive decision.
Somewhat error-prone for group projects, but more than manageable and convenient for a one-person project.
Also included are unnecessary attempts at memory management as personal exercises.

----Arbitrary to-do list----
-Engine Features
TODO: Find a way to multithread input so inputs are received and timed properly on sub-60FPS.
TODO: Add more customization over cleaning up framebuffers between frames.
TODO: Check if a framebuffer was used last frame before cleaning it up.
-Cleanup and Efficiency
TODO: Cleanup main.cpp. Move functionality to proper management classes/namespaces
----
Important defines:
--Engine--
Resource.cpp:           VE_CREATE_DEFAULT_RESOURCES
InputDevice.cpp:        VE_INPUT_BUFFER_INIT
-                       VE_INPUT_BUFFER_MID
Rendering.cpp:          VE_AUX_BUFFER_AMOUNT
-                       VE_WORLD_SCALE
-                       VE_FONT_DEFAULT
Rendering.h:            VE_USE_SINGLE_BUFFER
Time.h:                 VE_FRAME_TIME
-                       VE_FRAME_RATE
DebugLog.h:		        VE_DEBUG_ERRORTHROW
ScriptParsingUtils.cpp: VE_TAB_SPACE_AMOUNT

--Game--
PlayerManager.cpp:      VE_MAX_PLAYERS
-                       VE_MAX_SPECTATORS
*/

#include "SystemIncludes.hpp"
#include "FGIncludes.hpp"

void GLInit();
void GLCleanup();
void EngineInit();
void EngineCleanup();
void UpdateComponents();

int main()
{
	//We're setting the numeric locale to default C. Different locales have different ways of signifying decimal points, so converting text into variables can be unpredictable.
	//This just makes it so that decimal point is always a dot, on both input and output.
	setlocale(LC_NUMERIC, "C");

	//Initialize OpenGL parameters and engine components.
	GLInit();
	EngineInit();

	while(!glfwWindowShouldClose(Screen::window))
	{
		UpdateComponents();

		if(glfwGetKey(Screen::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(Screen::window, GLFW_TRUE);
		}

		if(glfwGetKey(Screen::window, GLFW_KEY_F2))
		{
			GameSceneManager::ReloadScene();
		}
	}

	EngineCleanup();
	GLCleanup();
}

//Initialize OpenGL related parameters.
inline void GLInit()
{
	//Init GLFW
	glfwInit();
	//Window settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef VE_USE_SINGLE_BUFFER
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
#endif

	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	//Initialize screen variables
	Screen::Init();

	//Initialize and store window
	Screen::window = glfwCreateWindow(Screen::size.x, Screen::size.y, "Videogame", nullptr, nullptr);
	glfwMakeContextCurrent(Screen::window);
	//Callbacks
	glfwSetWindowPos(Screen::window, (int)(Screen::mode->width*0.5 - Screen::size.x*0.5), (int)(Screen::mode->height*0.5 - Screen::size.y*0.5));
	glfwSetKeyCallback(Screen::window, InputManager::KeyCallback);
	glfwSetWindowSizeCallback(Screen::window, Screen::OnResize);
	//Vsync
	glfwSwapInterval(0);

	glewExperimental = true;
	glewInit();
}

inline void GLCleanup()
{
	//Cleanup GLFW
	glfwTerminate();
}

//--
//Engine Handling
//--

inline void EngineInit()
{
	DebugLog::Init();
	GLState::Init();
	Resource::Init();
	Rendering::Init();
	GameSceneManager::Init();
	InputManager::Init();
	ScriptManager::Init();
	PlayerManager::Init();

	VE_DEBUG_LOG("Full Init");
}

inline void EngineCleanup()
{
	ScriptManager::Cleanup();
	InputManager::Cleanup();
	GameSceneManager::Cleanup();
	Rendering::Cleanup();
	Resource::Cleanup();
	GLState::Cleanup();
	DebugLog::Cleanup();
}

//--
//Component Handling
//--

inline void UpdateComponents()
{
	Profiler::Clear();
	Time::Update();
	Screen::Update();
	PlayerManager::Update();
	GameSceneManager::Update();
}