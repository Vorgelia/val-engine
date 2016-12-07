/*
Val Engine
	by Vorgelia
	-@Vorgelia

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

TODO: Clean up includes. Change default function parameters to be defined in .h and not .cpp.
TODO: Change some class variables to be private with getters.
TODO: Replace some instances of map with unordered_map.
TODO: Rewrite input polling for proper handling of unrendered frames and multithreading.
----
Important defines:
	Resource.cpp: VE_CREATE_DEFAULT_RESOURCES
	InputDevice.cpp: INPUT_BUFFER_INIT
					 INPUT_BUFFER_MID
*/

#include "SystemIncludes.hpp"
#include "FGIncludes.hpp"

#include "resource1.h"
void GLInit();
void GLCleanup();
void EngineInit();
void EngineCleanup();
void UpdateComponents();
void BeginFrame();
void EndFrame();

int main(){
	setlocale(LC_NUMERIC, "C");
	GLInit();
	EngineInit();


	while (!glfwWindowShouldClose(Screen::window)){


		UpdateComponents();

		bool updatedFrame = false;
		if (!GameStateManager::isLoading){
			while (Time::lastUpdateTime + VE_FRAME_TIME <= Time::time){
				updatedFrame = true;
				Time::frameCount += 1;
				Time::lastUpdateTime += VE_FRAME_TIME;
				InputManager::Update();
				if (!GameStateManager::isLoading){
					if (!GameStateManager::states[GameStateManager::currentState]->initialized())
						GameStateManager::states[GameStateManager::currentState]->Init();
					GameStateManager::states[GameStateManager::currentState]->GameUpdate();
				}
			}
			if (GameStateManager::states[GameStateManager::currentState]->initialized())
				GameStateManager::states[GameStateManager::currentState]->Update();
		}

		GameStateManager::FrameEnd();


		if(updatedFrame){
			BeginFrame();
			if (!GameStateManager::isLoading)
				GameStateManager::states[GameStateManager::currentState]->RenderObjects();
			EndFrame();

			/*system("cls");
			std::cout << "State:" << GameStateManager::currentState << std::endl;
			std::cout << "Loading:" << GameStateManager::isLoading << std::endl;
			std::cout << "Time:" << Time::time << std::endl;
			std::cout << "Last Update Time:" << Time::lastUpdateTime << std::endl;
			std::cout << "Load Time:" << Time::timeSinceLoad << std::endl;*/
		}

	}

	EngineCleanup();
	GLCleanup();
}

//--
//GL Handling
//--
inline void GLInit(){
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
	//Init window
	Screen::window = glfwCreateWindow(Screen::size.x,Screen::size.y,"Videogame",nullptr,nullptr);
	glfwMakeContextCurrent(Screen::window);
	//Callbacks
	glfwSetWindowPos(Screen::window, (int)(Screen::mode->width*0.5 - Screen::size.x*0.5), (int)(Screen::mode->height*0.5 - Screen::size.y*0.5));
	glfwSetKeyCallback(Screen::window,InputManager::KeyCallback);
	glfwSetWindowSizeCallback(Screen::window,Screen::OnResize);
	glfwSwapInterval(0);
	glewExperimental = true;
	glewInit();
}
inline void GLCleanup(){
	//Cleanup GLFW
	glfwTerminate();
}
//--
//Engine Handling
//--
void BeginFrame(){
	
	glfwSwapInterval(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, Screen::size.x, Screen::size.y);
	for (unsigned int i = 0; i <Rendering::auxBuffers.size(); ++i){
		Rendering::auxBuffers[i]->Clear();
	}
	Rendering::mainBuffer->Clear();//Also binds the main buffer	
}
void EndFrame(){
	if (!GameStateManager::isLoading){
		GameStateManager::states[GameStateManager::currentState]->FrameEnd();

		for (unsigned int i = 0; i<GameStateManager::states[GameStateManager::currentState]->postEffectsOrder.size(); ++i){
			Rendering::DrawPostEffect(Resource::postEffects[GameStateManager::states[GameStateManager::currentState]->postEffectsOrder[i]]);
		}

		GameStateManager::states[GameStateManager::currentState]->GUI();
	}
	//Rendering::DrawScreenText(glm::vec4(1920 - 64, 0, 64, 64), 64, std::to_string(GameStateManager::currentState), nullptr, TextAlignment::Right);
	//Finally, render the main buffer to the default buffer
	glViewport(Screen::viewportSize.x, Screen::viewportSize.y, Screen::viewportSize.z, Screen::viewportSize.w);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	Rendering::DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), Resource::GetMesh("Meshes/Base/screenQuad.vm"), Rendering::mainBuffer, Resource::GetMaterial("Materials/Base/Screen_FB.vmat"));
	glfwSwapBuffers(Screen::window);
	Screen::isDirty = false;	
}
inline void EngineInit(){
	GLState::Init();
	Resource::Init();
	Rendering::Init();
	GameStateManager::Init();
	InputManager::Init();
}
inline void EngineCleanup(){
	GameStateManager::Cleanup();
	Rendering::Cleanup();
	Resource::Cleanup();
	GLState::Cleanup();
}
//--
//Component Handling
//--
inline void UpdateComponents(){
	Profiler::Clear();
	Time::Update();
	Screen::Update();
	Rendering::Update();
	GameStateManager::Update();
}