#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "movingPlatform.h"
#include "transparentWall.h"
#include "Portal.h"
#include "Particles.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
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
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	if (config.child("particles")) {
		particles = (Particles*)app->entityManager->CreateEntity(EntityType::PARTICLES);
		particles->parameters = config.child("particles");
	}

	for (pugi::xml_node FlyingEnemyNode = config.child("flyingEnemy"); FlyingEnemyNode; FlyingEnemyNode = FlyingEnemyNode.next_sibling("flyingEnemy")) {
		FlyingEnemy* flyingenemy = (FlyingEnemy*)app->entityManager->CreateEntity(EntityType::FLYINGENEMY); 
		flyingenemy->parameters = FlyingEnemyNode;
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
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
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

	if (app->godmode) {
		float camSpeed = 1;
		if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y -= (int)ceil(camSpeed * dt);

		if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y += (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x -= (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x += (int)ceil(camSpeed * dt);
		pugi::xml_node parameters;
		//Si estoy en godmode puedo restaurar la vida del player y enemigos
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) { 

			app->vida = parameters.attribute("vida").as_int(); 
			app->WalkingEnemyAlive = true;
			app->FlyingEnemyAlive = true;
			app->SecondFlyingEnemyAlive = true;

		}
			
	}
	else {
		if (app->render->camera.x != player->position.x * app->win->GetScale()) {
			app->render->camera.x = -player->position.x * app->win->GetScale() + 200;
		} 
		if (app->render->camera.y != player->position.y * app->win->GetScale()) { 
			app->render->camera.y = 0;
		}
		
		if (app->render->camera.x >= 0) {
			app->render->camera.x = 0;
		}
		if (app->render->camera.x <= -5500) {
			app->render->camera.x = -5500;
		}
	}
	
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest(); 
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest(); 
	 
	// Renders the image in the center of the screen 
	//app->render->DrawTexture(img, (int)textPosX, (int)textPosY);
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
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

	//Updates the camera position using the state in the xml file
	player->position.x = node.child("Player").attribute("x").as_int();
	player->position.y = node.child("Player").attribute("y").as_int();
	app->vida = node.child("Player").attribute("Vida").as_int();

	player->pbody->body->SetTransform(PIXEL_TO_METERS(b2Vec2(player->position.x, player->position.y)), 0); 
	
	for (pugi::xml_node WalkingEnemyNode = node.child("walkingEnemy"); WalkingEnemyNode; WalkingEnemyNode = WalkingEnemyNode.next_sibling("walkingEnemy")) {

		walkingEnemy->position.x = node.child("WalkingEnemy").attribute("x").as_int();
		walkingEnemy->position.y = node.child("WalkingEnemy").attribute("y").as_int();
		app->WalkingEnemyAlive = node.child("WalkingEnemy").attribute("Alive").as_bool();

		walkingEnemy->pbody->body->SetTransform(PIXEL_TO_METERS(b2Vec2(walkingEnemy->position.x, walkingEnemy->position.y)), 0);
	}

	for (pugi::xml_node FlyingEnemyNode = node.child("flyingEnemy"); FlyingEnemyNode; FlyingEnemyNode = FlyingEnemyNode.next_sibling("flyingEnemy")) {

		flyingEnemy->position.x = node.child("FlyingEnemy").attribute("x").as_int();
		flyingEnemy->position.y = node.child("FlyingEnemy").attribute("y").as_int();
		app->FlyingEnemyAlive = node.child("FlyingEnemy").attribute("Alive").as_bool();

		flyingEnemy->pbody->body->SetTransform(PIXEL_TO_METERS(b2Vec2(flyingEnemy->position.x, flyingEnemy->position.y)), 0);
	}
	
	return true;
}

bool Scene::SaveState(pugi::xml_node node) {

	//append on node of a new child Camera and add attributtes x,y of the camera position
	pugi::xml_node camNode = node.append_child("Player"); 
	camNode.append_attribute("x").set_value(player->position.x); 
	camNode.append_attribute("y").set_value(player->position.y); 
	camNode.append_attribute("Vida").set_value(app->vida); 

	player->pbody->body->GetTransform();

	//ListItem<Entity*>* item; 

	//for (item = app->entityManager->entities.start; item != NULL; item = item->next)
	//{
	//	pugi::xml_node nodes = node.append_child("Entity");
	//	for (int i = 0; item != NULL; i++) {
	//		nodes.append_attribute("x").set_value(nodes.p); 
	//	}
	//	

	//}


	for (pugi::xml_node WalkingEnemyNode = node.child("WalkingEnemy"); WalkingEnemyNode; WalkingEnemyNode = WalkingEnemyNode.next_sibling("WalkingEnemy")) {

		WalkingEnemyNode.append_attribute("x").set_value(walkingEnemy->position.x);
		WalkingEnemyNode.append_attribute("y").set_value(walkingEnemy->position.y);
		WalkingEnemyNode.append_attribute("Alive").set_value(app->WalkingEnemyAlive);

		walkingEnemy->pbody->body->GetTransform();
	}

	for (pugi::xml_node FlyingEnemyNode = node.child("flyingEnemy"); FlyingEnemyNode; FlyingEnemyNode = FlyingEnemyNode.next_sibling("flyingEnemy")) {

		FlyingEnemyNode.append_attribute("x").set_value(flyingEnemy->position.x); 
		FlyingEnemyNode.append_attribute("y").set_value(flyingEnemy->position.y); 
		FlyingEnemyNode.append_attribute("Type").set_value(app->scene->flyingEnemy->type); 
		FlyingEnemyNode.append_attribute("Alive").set_value(app->FlyingEnemyAlive);

		flyingEnemy->pbody->body->GetTransform();
	}

	return true;
}