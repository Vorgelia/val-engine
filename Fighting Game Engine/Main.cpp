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
Libraries planned to be used:
IrrKlang

Structured to depend on global variables and functions in namespaces.
Having global access to things like Input, Time, OpenGL states, etc was the most intuitive decision.
Somewhat error-prone for group projects, but more than manageable and convenient for a one-person project.
Also included are unnecessary attempts at memory management as personal exercises.

Variables and getters/setters begin with lowercase. Classes, namespaces, functions, etc begin with uppercase.

----Arbitrary to-do list----
-Engine Features
TODO: Find a way to multithread input so inputs are received and timed properly on sub-60FPS.
TODO: Add more customization over cleaning up framebuffers between frames.
TODO: Check if a framebuffer was used last frame before cleaning it up.
TODO: Add error checking to Network classes.
-Cleanup and Efficiency
TODO: Clean up includes. Change default function parameters to be defined in .h and not .cpp.
TODO: Change some class variables to be private with getters.
TODO: Replace some instances of map with unordered_map.
----
Important defines:
Resource.cpp:    VE_CREATE_DEFAULT_RESOURCES
InputDevice.cpp: VE_INPUT_BUFFER_INIT
VE_INPUT_BUFFER_MID
Rendering.cpp:   VE_AUX_BUFFER_AMOUNT
VE_WORLD_SCALE
VE_FONT_DEFAULT
Time.h:			 VE_FRAME_TIME
VE_FRAME_RATE
DebugLog.h:		 VE_DEBUG_ERRORTHROW
*/

#include "SystemIncludes.hpp"
#include "FGIncludes.hpp"

void GLInit();
void GLCleanup();
void EngineInit();
void EngineCleanup();
void UpdateComponents();
void BeginFrame();
void EndFrame();

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
		//Aaand this is the main game loop. 
		//There's two kinds of updates to keep track of. Loop updates, that just refer to a single iteration of the above loop running, and game updates, which refer to the fixed frame game state updates.

		//This is a fighting game, so we want a fixed game update rate of 60 per second. This isn't as simple as it might sound since a loop update might take much more or much less than 1/60th of a second.
		//The current approach is as follows: Check how long it's been since the last loop update, update enough times for that time to be less than 1/60th of a second.

		//It is crucial to note that every engine that isn't specialized on something as timing-precise as this one just updates as frequently as possible, while scaling timed operations
		//by the amount of time that passed between frames. For instance, on a frame that takes 2/60 seconds:
		//----OTHER ENGINE: Advance states like animations and movement at speed*2/60 -> render
		//----VAL ENGINE: Advance states like animations and movement at speed*1/60 -> repeat once -> render
		//And on a frame that takes 1/120 seconds.
		//----OTHER ENGINE: Advance states at speed*1/120
		//----VAL ENGINE: Do nothing until it's time to update, which is at another 1/120 seconds

		//This is because fighting games are generally perceived as working on frames, with no middle states in between. It's easier for a designer to specify a move lasting 4 frames
		//and have it concrete and easy to understand, rather than saying a move lasts 0.06666666 seconds and eeeeeh it depends on when we update.

		//It's also of note that physics engines also operate on fixed update rates just to maintain precision on lower framerates.

		UpdateComponents();//Some parts of the engine like timekeeping have to update as frequently as possible, regardless of whether it's game update time.

		bool updatedFrame = false;//This is a variable that keeps track of whether we've run a game update on this iteration. If we have, this will tell the engine to render at the end.
		if(!GameStateManager::isLoading)
		{
			while(Time::lastUpdateTime + VE_FRAME_TIME <= Time::time)
			{//Run updates until running one would put us ahead of our current time
				updatedFrame = true;
				Time::frameCount += 1;
				Time::lastUpdateTime += VE_FRAME_TIME;//This is important. We don't set last update time to current time, but we just advance it by 1/60
				//This means the while statement will run again if more than one frame was to be processed in between now and last loop update.
				InputManager::Update();
				GameStateManager::StateInit();
				GameStateManager::StateGameUpdate();//Send a game update callback

			}
			GameStateManager::StateUpdate();//Send a loop update callback regardless of whether the previous loop ran
		}

		GameStateManager::FrameEnd();//Checks if a level needs to be loaded and raises the necessary flags, as well as call the necessary resource management functions


		if(updatedFrame)
		{
			//Reset OpenGL rendering variables, clear buffers and prepare for rendering.
			BeginFrame();
			//Render all objects in the current scene
			GameStateManager::StateRenderObjects();
			//Apply post processing effects and render the result to the main buffer
			EndFrame();
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

//Reset OpenGL rendering variables, clear buffers and prepare for rendering.
void BeginFrame()
{
	//Not sure why this is here
	glfwSwapInterval(0);
	//Reset certain rendering parameters that might have been overriden in the last frame.
	//BlendFunc controls the way alpha blending happens
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Viewport controls the rendering size in pixels based on the actual window size.
	//We set it to the full window size here to perform no added transformation to the ones we do when rendering. Try changing Screen::size.y to Screen::size.y*0.5.
	//This will be used later to force the aspect ratio to 16/9
	glViewport(0, 0, Screen::size.x, Screen::size.y);

	//Bind and clear all framebuffers.
	for(unsigned int i = 0; i < Rendering::auxBuffers.size(); ++i)
	{
		Rendering::auxBuffers[i]->Clear();
	}
	Rendering::mainBuffer->Clear();//Also binds the main buffer	
}

//Apply post processing effects and render the result to the main buffer
void EndFrame()
{
	//Don't apply anything if the game state manager is loading.
	if(!GameStateManager::isLoading)
	{
		//Call the frame end callback on the current scene
		GameStateManager::states[GameStateManager::currentState]->FrameEnd();
		//Draw post effects specified in State/PostEffectsOrder.txt, in the order they were given
		for(unsigned int i = 0; i < GameStateManager::states[GameStateManager::currentState]->postEffectsOrder.size(); ++i)
		{
			Rendering::DrawPostEffect(Resource::postEffects[GameStateManager::states[GameStateManager::currentState]->postEffectsOrder[i]]);
		}
		//Tell the scene to draw its GUI now.
		GameStateManager::states[GameStateManager::currentState]->GUI();
	}
	//Rendering::DrawScreenText(glm::vec4(1920 - 64, 0, 64, 64), 64, std::to_string(GameStateManager::currentState), nullptr, TextAlignment::Right);

	//Finally, render the main buffer to the default buffer.
	//Set the viewport to what was calculated for a forced 16:9 aspect ratio
	glViewport(Screen::viewportSize.x, Screen::viewportSize.y, Screen::viewportSize.z, Screen::viewportSize.w);
	//Bind the default framebuffer, clear it and draw the main buffer directly.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	Rendering::DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), Resource::GetMesh("Meshes/Base/screenQuad.vm"), Rendering::mainBuffer, Resource::GetMaterial("Materials/Base/Screen_FB.vmat"));
	glfwSwapBuffers(Screen::window);
}
inline void EngineInit()
{
	DebugLog::Init();
	GLState::Init();
	Resource::Init();
	Rendering::Init();
	GameStateManager::Init();
	InputManager::Init();

	DebugLog::Push("Full Init");
}
inline void EngineCleanup()
{
	InputManager::Cleanup();
	GameStateManager::Cleanup();
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
	Rendering::Update();
	GameStateManager::Update();
}