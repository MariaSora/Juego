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

	type = parameters.attribute("type").as_bool();
	if (type) {
		position.x = parameters.attribute("x").as_int();
		position.y = parameters.attribute("y").as_int();
	}
	if (!type) {
		position2.x = parameters.attribute("x").as_int();
		position2.y = parameters.attribute("y").as_int();
	}

	texturePath = parameters.attribute("texturepath").as_string();
	drawPath = parameters.attribute("path").as_string(); 
	drawPath4 = parameters.attribute("path4").as_string(); 
	dir = parameters.attribute("direction").as_bool();
	

	flyAnim.LoadAnimation("flyingEnemy", "flyAnim");
	deathAnim.LoadAnimation("flyingEnemy", "deathAnim");

	return true;
}

bool FlyingEnemy::Start() {

	if (type) {
		initialpos.y = position.y;
		initialpos.x = position.x;
	}
	if (!type) {
		initialpos.y = position2.y;
		initialpos.x = position2.x;
	}
	active = true;
	app->FlyingEnemyAlive = true;
	app->SecondFlyingEnemyAlive = true;

	//initilize textures
	texture = app->tex->Load(texturePath);	
	texture2 = app->tex->Load(drawPath);
	texture4 = app->tex->Load(drawPath4);
	pbody = app->physics->CreateRectangleSensor(position.x + 16, position.y + 16, 16, 16, bodyType::KINEMATIC);
	pbody2 = app->physics->CreateRectangleSensor(position2.x + 16, position2.y + 16, 16, 16, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody2->listener = this;
	pbody->ctype = ColliderType::FLYINGENEMY;
	pbody2->ctype = ColliderType::FLYINGENEMY;



	return true;
}

bool FlyingEnemy::Update(float dt)
{		
	b2Vec2 vel = pbody->body->GetLinearVelocity();
	b2Vec2 vel2 = pbody2->body->GetLinearVelocity();
	
	if (!active) pbody->body->SetActive(false);
	playerPos = app->map->WorldToMap(app->scene->GetPlayer()->position.x, app->scene->GetPlayer()->position.y - 80);

	if (type) {
		if (app->FlyingEnemyAlive) {
			currentAnimation = &flyAnim;

			enemyPos = app->map->WorldToMap(position.x - 10, position.y - 10);
			
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
				app->attack = true;
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
			if (deathAnim.HasFinished()) {
				app->map->pathfinding->ClearLastPath();
				pbody->body->SetActive(false);
			}
		}
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x + 10, position.y + 10, &rect);
	}
	if (!type) {
		if (app->SecondFlyingEnemyAlive) {
			currentAnimation = &flyAnim;

			enemyPos = app->map->WorldToMap(position2.x - 10, position2.y - 10);

			if (enemyPos.x - playerPos.x <= 10 && enemyPos.x - playerPos.x >= -10)
			{
				app->map->pathfinding4->CreatePath(enemyPos, playerPos);
				path = app->map->pathfinding4->GetLastPath();
				if (app->physics->debug)
				{
					for (uint i = 0; i < path->Count(); i++)
					{
						iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
						app->render->DrawTexture(texture4, pos.x, pos.y);
					}
				}
			}

			if (enemyPos.x - playerPos.x <= 5 && enemyPos.x - playerPos.x >= -5) {
				Attack();
				app->attack2 = true;
			}
			else {
				app->attack2 = false;
				vel = { 0,0 };
				pbody2->body->SetLinearVelocity(vel);
			}

			position2.x = METERS_TO_PIXELS(pbody2->body->GetTransform().p.x) - 16;
			position2.y = METERS_TO_PIXELS(pbody2->body->GetTransform().p.y) - 16;
		}
		if (!app->SecondFlyingEnemyAlive) {
			LOG("FLYINGENEMY DIES");
			currentAnimation = &deathAnim;
			app->attack2 = false;
			position2.y += 2;
			vel = { 0,0 };
			pbody2->body->SetLinearVelocity(vel);
			if (deathAnim.HasFinished()) { 
				app->map->pathfinding->ClearLastPath();
				pbody2->body->SetActive(false);
			}

		}
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position2.x + 10, position2.y + 10, &rect);
	}

	currentAnimation->Update();
	
	return true;
}

void FlyingEnemy::Attack()
{	
	MoveToPlayer(enemyPos, 1.0f, path);
}
void FlyingEnemy::MoveToPlayer(iPoint& enemyPos, float speed, const DynArray<iPoint>* path)
{
	b2Vec2 vel = pbody->body->GetLinearVelocity();
	b2Vec2 vel2 = pbody2->body->GetLinearVelocity();

	if (path->Count() > 0)
	{
		iPoint nextNode;
		if (app->map->pathfinding->Move(enemyPos, nextNode))
		{
			int dx = nextNode.x - enemyPos.x;
			int dy = nextNode.y - enemyPos.y;

			vel = { dx * speed, dy * speed };

			enemyPos = nextNode;
			pbody->body->SetLinearVelocity(vel);
		}
		if (app->map->pathfinding4->Move(enemyPos, nextNode))
		{
			int dx = nextNode.x - enemyPos.x;
			int dy = nextNode.y - enemyPos.y;

			vel = { dx * speed, dy * speed };

			enemyPos = nextNode;
			pbody2->body->SetLinearVelocity(vel);
		}
	}

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
			active = false;
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
