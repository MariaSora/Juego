#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "movingPlatform.h"
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
	
	if (config.child("flyingEnemy")) {
		flyingEnemy = (FlyingEnemy*)app->entityManager->CreateEntity(EntityType::FLYINGENEMY);
		flyingEnemy->parameters = config.child("flyingEnemy");
	}

	if (config.child("particles")) {
		particles = (Particles*)app->entityManager->CreateEntity(EntityType::PARTICLES);
		particles->parameters = config.child("particles");
	}

	if (config.child("walkingEnemy")) {
		walkingEnemy = (WalkingEnemy*)app->entityManager->CreateEntity(EntityType::WALKINGENEMY);
		walkingEnemy->parameters = config.child("walkingEnemy");
	}

	for (pugi::xml_node platformNode = config.child("movingplatform"); platformNode; platformNode = platformNode.next_sibling("movingplatform")) {
		MovingPlatform* movingplatform = (MovingPlatform*)app->entityManager->CreateEntity(EntityType::MOVINGPLATFORM);
		movingplatform->parameters = platformNode;
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
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

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
	player->position.x = node.child("position_player").attribute("x").as_int();
	player->position.y = node.child("position_player").attribute("y").as_int();

	player->pbody->body->SetTransform(PIXEL_TO_METERS(b2Vec2(player->position.x, player->position.y)), 0); 
	
	walkingEnemy->position.x = node.child("position_walkingenemy").attribute("x").as_int();
	walkingEnemy->position.y = node.child("position_walkingenemy").attribute("y").as_int();
	
	walkingEnemy->pbody->body->SetTransform(PIXEL_TO_METERS(b2Vec2(walkingEnemy->position.x, walkingEnemy->position.y)), 0);
	
	flyingEnemy->position.x = node.child("position_flyingenemy").attribute("x").as_int();
	flyingEnemy->position.y = node.child("position_flyingenemy").attribute("y").as_int();

	flyingEnemy->pbody->body->SetTransform(PIXEL_TO_METERS(b2Vec2(flyingEnemy->position.x, flyingEnemy->position.y)), 0);

	return true;
}

// L14: TODO 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool Scene::SaveState(pugi::xml_node node) {

	//append on node of a new child Camera and add attributtes x,y of the camera position
	pugi::xml_node camNode = node.append_child("position_player"); 
	camNode.append_attribute("x").set_value(player->position.x); 
	camNode.append_attribute("y").set_value(player->position.y); 
	
	player->pbody->body->GetTransform();

	pugi::xml_node camNode1 = node.append_child("position_walkingenemy");
	camNode1.append_attribute("x").set_value(walkingEnemy->position.x);
	camNode1.append_attribute("y").set_value(walkingEnemy->position.y);

	walkingEnemy->pbody->body->GetTransform();

	pugi::xml_node camNode2 = node.append_child("position_flyingenemy");
	camNode2.append_attribute("x").set_value(flyingEnemy->position.x);
	camNode2.append_attribute("y").set_value(flyingEnemy->position.y);

	flyingEnemy->pbody->body->GetTransform();

	return true;
}