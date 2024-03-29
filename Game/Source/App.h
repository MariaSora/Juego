#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "Fonts.h"
#include "EntityManager.h"

#include "PugiXml/src/pugixml.hpp"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Fonts;
class Audio;
class Scene;
class EntityManager;
class Map;
class Physics;
class GuiManager;
class FadeToBlack;
class SceneIntro;
class GameOver;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	bool LoadRequest();
	bool SaveRequest();
	bool Load();
	bool Save();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Fonts* fonts;
	Audio* audio;
	Scene* scene;
	EntityManager* entityManager;
	Map* map;
	Physics* physics;
	GuiManager* guiManager;
	FadeToBlack* fade;
	SceneIntro* sceneIntro;
	GameOver* gameover;

	int vida;
	bool statewalkingenemy = true;
	bool FlyingEnemyAlive = true;
	bool SecondFlyingEnemyAlive = true;
	bool WalkingEnemyAlive = true;
	bool WalkingEnemyAlive2 = true;

	bool saveRequest = false;
	bool loadRequest = false;

	bool attack = false;
	bool attack2 = false;

	iPoint positionportal2; 
	iPoint level2;

	bool godmode = false;
	bool WEDamaged = false;

	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;


private:

	int argc;
	char** args;
	SString gameTitle;
	SString organization;

	List<Module *> modules;


	uint frames;
	float dt;

    // required variables are provided:
	Timer startupTime;
	PerfTimer frameTime;
	PerfTimer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	uint32 secondsSinceStartup = 0;

	uint32 maxFrameDuration = 16;

};

extern App* app;

#endif	// __APP_H__