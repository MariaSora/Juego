#include "EntityManager.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "movingPlatform.h"
#include "transparentWall.h"
#include "Portal.h"
#include "Particles.h"
#include "Pathfinding.h"
#include "Map.h"
#include "Checkpoints.h"
#include "Boss.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::HEALITEM:
		entity = new HealItem();
		break;
	case EntityType::CANDYITEM:
		entity = new CandyItem();
		break;
	case EntityType::FLYINGENEMY:
		entity = new FlyingEnemy();
		enemies.Add(entity);
		break;
	case EntityType::WALKINGENEMY:
		entity = new WalkingEnemy();
		enemies.Add(entity); 
		break;
	case EntityType::MOVINGPLATFORM:
		entity = new MovingPlatform();
		break;
	case EntityType::BOSS: 
		entity = new Boss();
		break;
	case EntityType::WALL:
		entity = new transparentWall();
		break;
	case EntityType::PORTAL:
		entity = new Portal();
		break;
	case EntityType::PARTICLES:
		entity = new Particles();
		break;
	case EntityType::CHECKPOINT:
		entity = new Checkpoints();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

void EntityManager::KillEnemiesLoad()
{
	ListItem<Entity*>* item;
	for (item = enemies.start; item != NULL; item = item->next)
	{
		ListItem<iPoint>* destroyEnemy;
		for (destroyEnemy = enemies_dead.start; destroyEnemy != NULL; destroyEnemy = destroyEnemy->next) {
			if (iPoint(destroyEnemy->data.x, destroyEnemy->data.y) == item->data->initialpos) {
				item->data->active = false;

			}
		}
	}
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

