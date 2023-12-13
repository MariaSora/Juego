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
	distance = parameters.attribute("distance").as_int();
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
	pbody->ctype = ColliderType::FLYINGENEMY;


	initialPos.x = position.x;
	initialPos.y = position.y;

	return true;
}

bool FlyingEnemy::Update(float dt)
{
	/*pbody->body->SetGravityScale(0);
	pbody->body->GetFixtureList()[0].SetSensor(true);*/
	currentAnimation = &flyAnim;
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);

	b2Vec2 vel = pbody->body->GetLinearVelocity();

	if (position.x - app->scene->GetPlayer()->position.x <= 200 && position.x - app->scene->GetPlayer()->position.y >= -200)
	{
		/*playerFound = true;*/
		app->map->pathfinding->CreatePath(enemyPos, app->scene->GetPlayer()->position);
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
	/*else playerFound = false;*/

	if (position.x - app->scene->player->position.x <= 40 && position.x - app->scene->player->position.x >= -40) {
		Attack(); 
	}
	else app->attack = false; 


	enemyPos = app->map->WorldToMap(20 + position.x, 30 + position.y - app->render->camera.y);
	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect);

	//app->render->DrawTexture(texture, position.x + 108, position.y + 50, &rect);

	//pbody->body->ApplyForce(b2Vec2(0.0f, -app->physics->world->GetGravity().y * pbody->body->GetMass()), pbody->body->GetWorldCenter(), true);
	//position.x++;

	/*if (type)
	{	
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		if (!dir)
		{
			position.x++;
			if (position.x >= initialPos.x + distance)
			{
				dir = true;
			}
		}
		else
		{
			position.x--;
			if (position.x <= initialPos.x - distance)
			{
			 dir = false;
			}
		}
	}*/

	if (!type)
	{
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		if (!dir)
		{
			position.y++;
			if (position.y >= initialPos.y + distance)
			{
				dir = true;
			}
		}
		else
		{
			position.y--;
			if (position.y <= initialPos.y - distance)
			{
				dir = false;
			}
		}
	}

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

			if (dx > 0)
			{
				vel = { speed, 0 };
			}
			else if (dx < 0)
			{
				vel = { -speed, 0 };
			}

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
