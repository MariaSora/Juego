#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Fonts.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "movingPlatform.h"
#include "transparentWall.h"
#include "Portal.h"
#include "Particles.h"
#include "SceneIntro.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access

	for (pugi::xml_node healItemNode = config.child("healItem"); healItemNode; healItemNode = healItemNode.next_sibling("healItem"))
	{
		HealItem* healItem = (HealItem*)app->entityManager->CreateEntity(EntityType::HEALITEM);
		healItem->parameters = healItemNode;
	}
	
	for (pugi::xml_node candyItemNode = config.child("candyItem"); candyItemNode; candyItemNode = candyItemNode.next_sibling("candyItem"))
	{
		CandyItem* candyItem = (CandyItem*)app->entityManager->CreateEntity(EntityType::CANDYITEM);
		candyItem->parameters = candyItemNode;
	}

	for (pugi::xml_node checkpointNode = config.child("checkpoint"); checkpointNode; checkpointNode = checkpointNode.next_sibling("checkpoint"))
	{
		Checkpoints* checkpoint = (Checkpoints*)app->entityManager->CreateEntity(EntityType::CHECKPOINT);
		checkpoint->parameters = checkpointNode;
	}

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	for (pugi::xml_node ParticlesNode = config.child("particles"); ParticlesNode; ParticlesNode = ParticlesNode.next_sibling("particles")) {
		Particles* particles = (Particles*)app->entityManager->CreateEntity(EntityType::PARTICLES);
		particles->parameters = ParticlesNode;
	}

	for (pugi::xml_node FlyingEnemyNode = config.child("flyingEnemy"); FlyingEnemyNode; FlyingEnemyNode = FlyingEnemyNode.next_sibling("flyingEnemy")) {
		FlyingEnemy* flyingenemy = (FlyingEnemy*)app->entityManager->CreateEntity(EntityType::FLYINGENEMY); 
		flyingenemy->parameters = FlyingEnemyNode;
//		flyingEnemy = flyingenemy;
	}

	for (pugi::xml_node WalkingEnemyNode = config.child("walkingEnemy"); WalkingEnemyNode; WalkingEnemyNode = WalkingEnemyNode.next_sibling("walkingEnemy")) {
		WalkingEnemy* walkingenemy = (WalkingEnemy*)app->entityManager->CreateEntity(EntityType::WALKINGENEMY);
		walkingenemy->parameters = WalkingEnemyNode;
	}

	for (pugi::xml_node platformNode = config.child("movingplatform"); platformNode; platformNode = platformNode.next_sibling("movingplatform")) {
		MovingPlatform* movingplatform = (MovingPlatform*)app->entityManager->CreateEntity(EntityType::MOVINGPLATFORM);
		movingplatform->parameters = platformNode;
	}

	for (pugi::xml_node platformNode = config.child("transparentWall"); platformNode; platformNode = platformNode.next_sibling("transparentWall")) {
		transparentWall* transparentwall = (transparentWall*)app->entityManager->CreateEntity(EntityType::WALL);
		transparentwall->parameters = platformNode;
	}

	for (pugi::xml_node platformNode = config.child("portal"); platformNode; platformNode = platformNode.next_sibling("portal")) {
		Portal* portal = (Portal*)app->entityManager->CreateEntity(EntityType::PORTAL);
		portal->parameters = platformNode;
	}
		
	if (config.child("Boss")) {
		boss = (Boss*)app->entityManager->CreateEntity(EntityType::BOSS);
		boss->parameters = config.child("Boss");
	}

	if (config.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->name = config.child("map").attribute("name").as_string();
		app->map->path = config.child("map").attribute("path").as_string();
	}
	
	return ret;
}

	
// Called before the first frame
bool Scene::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	app->audio->PlayMusic("Assets/Audio/Music/backgroundMusic.ogg");

	//Get the size of the window
	//app->win->GetWindowSize(windowW, windowH);

	////Get the size of the texture
	//app->tex->GetSize(img, texW, texH);

	//textPosX = (float)windowW / 2 - (float)texW / 2;
	//textPosY = (float)windowH / 2 - (float)texH / 2;

	//SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
	//	app->map->mapData.width,
	//	app->map->mapData.height,
	//	app->map->mapData.tileWidth,
	//	app->map->mapData.tileHeight,
	//	app->map->mapData.tilesets.Count());

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	//if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
	//	if (popUpPause == nullptr) {
	//		popUpPause = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 1, "", { 0,0,0,0 }, this);
	//		SDL_Rect btPos7 = { 720, 110, 30,30 };
	//		crossPButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "X", btPos7, this);
	//	}
	//	if (crossPButton != nullptr) {
	//		if (crossPButton->isPressed) {
	//			app->guiManager->RemoveGuiControl(popUpPause);
	//			popUpPause = nullptr;
	//			app->guiManager->RemoveGuiControl(crossPButton);
	//			crossPButton = nullptr;
	//		}
	//	}
	//}
	if (app->godmode) {
		pugi::xml_node parameters;

		//Si estoy en godmode puedo restaurar la vida del player y enemigos
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) { 
			app->vida = 5;//parameters.attribute("vida").as_int(); 
			app->WalkingEnemyAlive = true;
			app->FlyingEnemyAlive = true;
			app->SecondFlyingEnemyAlive = true;
		}	
	}

	if (app->render->camera.x != player->position.x * app->win->GetScale()) {
		app->render->camera.x = -player->position.x * app->win->GetScale() + 200;
	}
	if (app->render->camera.y != player->position.y * app->win->GetScale()) {
		app->render->camera.y = 0;
	}
	//Camera limits
	if (app->render->camera.x >= 0) {
		app->render->camera.x = 0;
	}
	if (app->render->camera.x <= -5500) {
		app->render->camera.x = -5500;
	}

	iPoint mousePos;
	app->input->GetMousePosition(mousePos.x, mousePos.y);
	LOG("mousePos: %d, %d", mousePos.x, mousePos.y);

	// L13: Get the latest calculated path and draw

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest(); 
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest(); 

	if (player->die) {
		if (player->dieAnim.HasFinished() && !gameover) {
			gameover = true;
			app->fade->PassScreens(this, (Module*)app->gameover, 20);
		}
	}

	 
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

Player* Scene::GetPlayer()
{
	return player;
}

bool Scene::LoadState(pugi::xml_node node) {
	if (isEnabled) {
		//Updates the camera position using the state in the xml file
		player->position.x = node.child("Player").attribute("x").as_int() + 16;
		player->position.y = node.child("Player").attribute("y").as_int() - 16;
		app->vida = node.child("Player").attribute("Vida").as_int();
		//checkpoint->num = node.child("Player").attribute("LastCheckpoint").as_int();
		player->PositionUpdate.p.x = node.child("Player").attribute("lastcheckpoint.x").as_int() + 16;
		player->PositionUpdate.p.y = node.child("Player").attribute("lastcheckpoint.y").as_int() - 16;

		player->pbody->body->SetTransform(PIXEL_TO_METERS(b2Vec2(player->position.x, player->position.y)), 0);


		//Se eliminan todos

		while (app->entityManager->enemies.Count() > 0) {
			app->entityManager->DestroyEntity(app->entityManager->enemies.At(app->entityManager->enemies.Count() - 1)->data);
			app->entityManager->enemies.Del(app->entityManager->enemies.At(app->entityManager->enemies.Count() - 1));
		}
		app->entityManager->enemies.Clear();

		////Se cargan todos
		pugi::xml_document configFile;
		pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

		pugi::xml_node config = configFile.child("config").child("scene");

		for (pugi::xml_node FlyingEnemyNode = config.child("flyingEnemy"); FlyingEnemyNode; FlyingEnemyNode = FlyingEnemyNode.next_sibling("flyingEnemy")) {
			FlyingEnemy* flyingenemy = (FlyingEnemy*)app->entityManager->CreateEntity(EntityType::FLYINGENEMY);
			flyingenemy->parameters = FlyingEnemyNode;
			flyingenemy->Awake();
			flyingenemy->Start();
		}

		for (pugi::xml_node WalkingEnemyNode = config.child("walkingEnemy"); WalkingEnemyNode; WalkingEnemyNode = WalkingEnemyNode.next_sibling("walkingEnemy")) {
			WalkingEnemy* walkingenemy = (WalkingEnemy*)app->entityManager->CreateEntity(EntityType::WALKINGENEMY);
			walkingenemy->parameters = WalkingEnemyNode;
			walkingenemy->Awake();
			walkingenemy->Start();
		}

		for (pugi::xml_node itemNode = node.child("Enemies").child("Enemy"); itemNode; itemNode = itemNode.next_sibling("Enemy"))
		{
			if (!itemNode.attribute("Alive").as_bool()) {
				app->entityManager->enemies_dead.Add(iPoint(itemNode.attribute("x").as_int(), itemNode.attribute("y").as_int()));
			}
		}
		app->entityManager->KillEnemiesLoad();
	}
	
	return true;
}

bool Scene::SaveState(pugi::xml_node node) {
	if (isEnabled) {
		//append on node of a new child Camera and add attributtes x,y of the camera position
		pugi::xml_node camNode = node.append_child("Player");
		camNode.append_attribute("x").set_value(player->position.x);
		camNode.append_attribute("y").set_value(player->position.y);
		camNode.append_attribute("Vida").set_value(app->vida);
		camNode.append_attribute("lastcheckpoint.x").set_value(player->PositionUpdate.p.x);
		camNode.append_attribute("lastcheckpoint.y").set_value(player->PositionUpdate.p.y);
		//camNode.append_attribute("LastCheckpoint").set_value(checkpoint->num);
		//player->pbody->body->GetTransform();

		pugi::xml_node EnemiesNodes = node.append_child("Enemies");

		for (int i = 0; i < app->entityManager->enemies.Count(); i++) {
			pugi::xml_node EnemyNode = EnemiesNodes.append_child("Enemy");

			EnemyNode.append_attribute("x").set_value(app->entityManager->enemies.At(i)->data->initialpos.x);
			EnemyNode.append_attribute("y").set_value(app->entityManager->enemies.At(i)->data->initialpos.y);
			EnemyNode.append_attribute("Alive").set_value(app->entityManager->enemies.At(i)->data->active);
		}
	}
	

	return true;
}

void Scene::Enable()
{
	if (!isEnabled)
	{
		isEnabled = true;
		app->map->Enable();
		app->entityManager->Enable();
		app->guiManager->RemoveGuiControl(app->sceneIntro->playButton);
		app->guiManager->RemoveGuiControl(app->sceneIntro->continueButton);
		app->guiManager->RemoveGuiControl(app->sceneIntro->settingsButton);
		app->guiManager->RemoveGuiControl(app->sceneIntro->creditsButton);
		app->guiManager->RemoveGuiControl(app->sceneIntro->exitButton);
		Start();
	}
}

void Scene::Disable()
{
	// TODO 0: Call CleanUp() for disabling a module
	if (isEnabled)
	{
		isEnabled = false;
		app->map->Disable();
		app->entityManager->Disable();
		player->position.x = player->initialpos.x; 
		player->position.y = player->initialpos.y; 
		CleanUp();
	}
}