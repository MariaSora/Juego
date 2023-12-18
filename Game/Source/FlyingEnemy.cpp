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
	//distance = parameters.attribute("distance").as_int();
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
	if (app->scene->player->die) {
			position.x = initialPos.x;
			position.y = initialPos.y;
			app->scene->flyingEnemy->die = false;
		}
	b2Vec2 vel = pbody->body->GetLinearVelocity();
	if(!die) {

		/*pbody->body->SetGravityScale(0);*/
		/*pbody->body->GetFixtureList()[0].SetSensor(true);*/
		currentAnimation = &flyAnim;
		// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  

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
	else {
		LOG("FLYINGENEMY DIES");
		currentAnimation = &deathAnim;
		app->attack = false;
		position.y += 2;
		vel += { 0,0.5f };
		pbody->body->SetLinearVelocity(vel);
		//falta destruir la textura + collider
	}

		currentAnimation->Update();
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x + 10, position.y + 10, &rect);
		
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

	/*if (!type)
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
	}*/

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
		die = true; 
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
