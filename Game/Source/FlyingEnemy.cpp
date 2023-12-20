#include "FlyingEnemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"

FlyingEnemy::FlyingEnemy() : Entity(EntityType::FLYINGENEMY)
{
	name.Create("FlyingEnemy");
}

FlyingEnemy::~FlyingEnemy() {}

bool FlyingEnemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	drawPath = parameters.attribute("path").as_string(); 
	dir = parameters.attribute("direction").as_bool();
	type = parameters.attribute("type").as_bool();

	flyAnim.LoadAnimation("flyingEnemy", "flyAnim");
	deathAnim.LoadAnimation("flyingEnemy", "deathAnim");

	return true;
}

bool FlyingEnemy::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);	
	texture2 = app->tex->Load(drawPath);
	pbody = app->physics->CreateRectangleSensor(position.x + 16, position.y + 16, 16, 16, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::FLYINGENEMY;

	initialPos.y = position.y;
	initialPos.x = position.x;

	return true;
}

bool FlyingEnemy::Update(float dt)
{		
	b2Vec2 vel = pbody->body->GetLinearVelocity();

	if (type) {
		if (app->FlyingEnemyAlive) {
			currentAnimation = &flyAnim;

			enemyPos = app->map->WorldToMap(position.x - 10, position.y - 10);
			playerPos = app->map->WorldToMap(app->scene->GetPlayer()->position.x, app->scene->GetPlayer()->position.y - 80);

			if (enemyPos.x - playerPos.x <= 10 && enemyPos.x - playerPos.x >= -10)
			{
				app->map->pathfinding->CreatePath(enemyPos, playerPos);
				path = app->map->pathfinding->GetLastPath();
				if (app->physics->debug)
				{
					for (uint i = 0; i < path->Count(); i++)
					{
						iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
						app->render->DrawTexture(texture2, pos.x, pos.y);
					}
				}
			}

			if (enemyPos.x - playerPos.x <= 5 && enemyPos.x - playerPos.x >= -5) {
				Attack();
			}
			else {
				app->attack = false;
				vel = { 0,0 };
				pbody->body->SetLinearVelocity(vel);
			}

			position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
			position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
		}
		if(!app->FlyingEnemyAlive) {
			LOG("FLYINGENEMY DIES");
			currentAnimation = &deathAnim;
			app->attack = false;
			position.y += 2;
			vel = { 0,0 };
			pbody->body->SetLinearVelocity(vel);
			if (deathAnim.HasFinished()) app->map->pathfinding->ClearLastPath();

		}
	}
	if (!type) {
		if (app->SecondFlyingEnemyAlive) {
			currentAnimation = &flyAnim;

			enemyPos = app->map->WorldToMap(position.x - 10, position.y - 10);
			playerPos = app->map->WorldToMap(app->scene->GetPlayer()->position.x, app->scene->GetPlayer()->position.y - 80);

			if (enemyPos.x - playerPos.x <= 10 && enemyPos.x - playerPos.x >= -10)
			{
				app->map->pathfinding->CreatePath(enemyPos, playerPos);
				path = app->map->pathfinding->GetLastPath();
				if (app->physics->debug)
				{
					for (uint i = 0; i < path->Count(); i++)
					{
						iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
						app->render->DrawTexture(texture2, pos.x, pos.y);
					}
				}
			}

			if (enemyPos.x - playerPos.x <= 5 && enemyPos.x - playerPos.x >= -5) {
				Attack();
			}
			else {
				app->attack = false;
				vel = { 0,0 };
				pbody->body->SetLinearVelocity(vel);
			}

			position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
			position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
		}
		if (!app->SecondFlyingEnemyAlive) {
			LOG("FLYINGENEMY DIES");
			currentAnimation = &deathAnim;
			app->attack = false;
			position.y += 2;
			vel = { 0,0 };
			pbody->body->SetLinearVelocity(vel);
			if (deathAnim.HasFinished()) app->map->pathfinding->ClearLastPath();

		}
	}
	

		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x + 10, position.y + 10, &rect);
		
	
	return true;
}

void FlyingEnemy::MoveToPlayer(iPoint& enemyPos, float speed, const DynArray<iPoint>* path)
{
	b2Vec2 vel = pbody->body->GetLinearVelocity();

	if (path->Count() > 0)
	{
		iPoint nextNode;
		if (app->map->pathfinding->Move(enemyPos, nextNode))
		{
			int dx = nextNode.x - enemyPos.x;
			int dy = nextNode.y - enemyPos.y;

			vel = { dx * speed, dy * speed };

			enemyPos = nextNode; 
		}
	}
   	pbody->body->SetLinearVelocity(vel);
}

void FlyingEnemy::Attack()
{
	MoveToPlayer(enemyPos, 1.0f, path);
	app->attack = true; 
}

bool FlyingEnemy::CleanUp()
{	
	return true;
}

void FlyingEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		if (type) {
			app->FlyingEnemyAlive = false;
		}
		if (!type) {
			app->SecondFlyingEnemyAlive = false;
		}
		LOG("Collision PLAYER");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::STAIRS:
		LOG("Collision STAIRS");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}
