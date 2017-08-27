#include "CommonUtilIncludes.hpp"
#include "ResourceLoader.h"
#include "resource1.h"
#include "ResourceInitializer.h"
void ResourceInitializer::Init()
{
	//Create some folders, paste some files from the resources in them.
	//Resources like textures and fonts don't work at the moment.
	FS::create_directory("Materials/");
	FS::create_directory("Materials/Base");
	FS::create_directory("Materials/Intro");
	ResourceLoader::SaveFile("Materials/Base/Object2D.vmat", ResourceLoader::LoadTextResource(Object2Dvmat), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Materials/Base/Screen_BasePostFX.vmat", ResourceLoader::LoadTextResource(Screen_BasePostFXvmat), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Materials/Base/Screen_FB.vmat", ResourceLoader::LoadTextResource(Screen_FBvmat), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Materials/Base/Screen_Text.vmat", ResourceLoader::LoadTextResource(Screen_Textvmat), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Materials/Base/Screen.vmat", ResourceLoader::LoadTextResource(Screenvmat), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Materials/Intro/Intro_Screen.vmat", ResourceLoader::LoadTextResource(Intro_Screenvmat), std::ios::out | std::ios::trunc);
	FS::create_directory("Fonts/");
	//ResourceLoader::SaveFile("Fonts/Amble.ttf", ResourceLoader::LoadTextResource(AmbleTTF,"FONT"), std::ios::out | std::ios::trunc);
	FS::create_directory("Meshes/");
	FS::create_directory("Meshes/Base");
	ResourceLoader::SaveFile("Meshes/Base/quad.vm", ResourceLoader::LoadTextResource(Quadvm), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Meshes/Base/screenQuad.vm", ResourceLoader::LoadTextResource(ScreenQuadvm), std::ios::out | std::ios::trunc);
	FS::create_directory("PostEffects/");
	ResourceLoader::SaveFile("PostEffects/BasePostFX.vfx", ResourceLoader::LoadTextResource(BasePostFXvfx), std::ios::out | std::ios::trunc);
	FS::create_directory("Settings/");
	FS::create_directory("Settings/Input");
	ResourceLoader::SaveFile("Settings/Input/Default.vi", ResourceLoader::LoadTextResource(Defaultvi), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Settings/Input/Keyboard.vi", ResourceLoader::LoadTextResource(Keyboardvi), std::ios::out | std::ios::trunc);
	FS::create_directory("Shaders/");
	FS::create_directory("Shaders/Base");
	ResourceLoader::SaveFile("Shaders/Base/2D.frag", ResourceLoader::LoadTextResource(S2Dfrag), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Shaders/Base/2D.vert", ResourceLoader::LoadTextResource(S2Dvert), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Shaders/Base/BasePostProcessing.frag", ResourceLoader::LoadTextResource(BasePostProcessingfrag), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Shaders/Base/BasePostProcessing.vert", ResourceLoader::LoadTextResource(BasePostProcessingvert), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Shaders/Base/Screen.frag", ResourceLoader::LoadTextResource(Screenfrag), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Shaders/Base/Screen.vert", ResourceLoader::LoadTextResource(Screenvert), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Shaders/Base/Screen_Text.frag", ResourceLoader::LoadTextResource(Screen_Textfrag), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Shaders/Base/Screen_Text.vert", ResourceLoader::LoadTextResource(Screen_Textvert), std::ios::out | std::ios::trunc);
	FS::create_directory("States/");
	FS::create_directory("States/Menu");
	FS::create_directory("States/Intro");
	FS::create_directory("Textures/");
	FS::create_directory("Textures/Intro");
	ResourceLoader::SaveFile("Textures/tex.png", ResourceLoader::LoadTextResource(texpng, "PNG"), std::ios::out | std::ios::trunc);
	ResourceLoader::SaveFile("Textures/Intro/Intro.png", ResourceLoader::LoadTextResource(Intropng, "PNG"), std::ios::out | std::ios::trunc);
}