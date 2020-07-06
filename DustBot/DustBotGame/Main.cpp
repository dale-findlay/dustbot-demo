#include <game\Game.h>
#include <game\GameCreateInfo.h>
#include <subsystems\VirtualFileSystem.h>
#include <utilities\Logger.h>

#include <exception>
#include <iostream>

#ifndef DEBUG
#include <windows.h>
#endif

std::vector<std::pair<std::string, std::string>> GetMountedGameDirectories()
{
	std::vector<std::pair<std::string, std::string>> mountedDirectories;
	std::string applicationDirectory = Horizon::VirtualFileSystem::GetApplicationDirectory();

	std::pair<std::string, std::string> meshesPair("~/meshes", applicationDirectory + "\\GameAssets\\meshes");
	std::pair<std::string, std::string> texturesPair("~/textures", applicationDirectory + "\\GameAssets\\textures");
	std::pair<std::string, std::string> assembliesPair("~/assemblies", applicationDirectory + "\\GameAssets\assemblies");

	mountedDirectories.push_back(meshesPair);
	mountedDirectories.push_back(texturesPair);

	return mountedDirectories;
}

#ifdef DEBUG
int main()
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
#endif
{
	int returnCode = -1;
	
	Horizon::Game game;
	Horizon::GameCreateInfo gameCreateInfo;
	gameCreateInfo.name = "Dust Bot";
	gameCreateInfo.windowWidth = 1366;
	gameCreateInfo.windowHeight = 768;
	gameCreateInfo.mountedDirectories = GetMountedGameDirectories();
	gameCreateInfo.rootNamespace = "";

	gameCreateInfo.dependentAssembliesDefinitions =
	{ 
		{"~/assemblies/HorizonEngineScripting.dll", {"HorizonEngine", "HorizonEngine.Core", "HorizonEngine.Structures", "HorizonEngine.Subsystems", "HorizonEngine.Utilities"}, false },
		{"~/assemblies/GlmNet.dll", {"GlmNet"}, false },
		{"~/assemblies/DustBotScript.dll", {"DustBotScript"}, true },
		{"~/assemblies/Aim.dll", {"Aim"}, false },
		{"~/assemblies/ObjParser.dll", {"ObjParser"}, false }
	};

	gameCreateInfo.startScene = "01_test_scene";

	if (!game.Init(gameCreateInfo))
	{
		game.Shutdown();
		system("PAUSE");
		return -1;
	}

	returnCode = game.Run();

	game.Shutdown();

	return returnCode;
}