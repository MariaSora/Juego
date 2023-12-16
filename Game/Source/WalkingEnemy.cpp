#include "WalkingEnemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Player.h"
#include "Map.h"


WalkingEnemy::WalkingEnemy() : Entity(EntityType::WALKINGENEMY)
{
	name.Create("WalkingEnemy");
}

WalkingEnemy::~WalkingEnemy() {}

bool WalkingEnemy::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	type = parameters.attribute("type").as_bool(); 
	texturePath = parameters.attribute("texturepath").as_string();
	drawPath = parameters.attribute("path2").as_string(); 
	//distance = parameters.attribute("distance").as_int();
	app->livewalkingenemy = parameters.attribute("vida").as_int();

	idleAnim.LoadAnimation("walkingEnemy", "idleAnim");
	walkAnim.LoadAnimation("walkingEnemy", "walkAnim");
	deathAnim.LoadAnimation("walkingEnemy", "deathAnim");
	attackAnim.LoadAnimation("walkingEnemy", "attackAnim");
	jumpAnim.LoadAnimation("walkingEnemy", "jumpAnim");

	return true;
}

bool WalkingEnemy::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	texture2 = app->tex->Load(drawPath);
	pbody = app->physics->CreateCircle(position.x, position.y, 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::WALKINGENEMY;
	
	initialPos.y = position.y;
	initialPos.x = position.x;
	return true;
}

bool WalkingEnemy::Update(float dt)
{

	pbody->body->SetGravityScale(10);
	currentAnimation = &idleAnim;

	b2Vec2 vel = pbody->body->GetLinearVelocity();

	enemyPos = app->map->WorldToMap(position.x, position.y);
	playerPos = app->map->WorldToMap(app->scene->GetPlayer()->position.x - 10, position.y);

	if (enemyPos.x - playerPos.x <= 10 && enemyPos.x - playerPos.x >= -10)
	{
		app->map->pathfinding2->CreatePath(enemyPos, playerPos);
		path = app->map->pathfinding2->GetLastPath();
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
	/*	app->attack = false;*/
		vel = { 0,0 };
		pbody->body->SetLinearVelocity(vel);
	}

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	//app->render->DrawTexture(texture, position.x, position.y);
	if (!app->statewalkingenemy) { 
		idleAnim.Reset();
		currentAnimation = &attackAnim; 
		if (attackAnim.HasFinished()) {
			for (b2ContactEdge* ce = pbody->body->GetContactList(); ce; ce = ce->next) {
				b2Contact* c = ce->contact;
				if (c->GetFixtureA()->GetBody() == app->scene->player->pbody->body) {
					LOG("Player contact");
					app->vida--;
					app->scene->player->damage = true; 
				}
			}
			app->statewalkingenemy = true;
		}
	}
	if (app->statewalkingenemy) {
		attackAnim.Reset();
		currentAnimation = &idleAnim; 
		counter++;
		if (counter == 50) {
			counter = 0;
			app->statewalkingenemy = false;
		}
	}
	/*if (type)
	{
		if (!direction)
		{
			currentAnimation = &jumpAnim;
			if (jumpAnim.HasFinished()) {
				jumpAnim.Reset();
			}
			position.x++;
			if (position.x >= initialpos.x + distance)
			{
				direction = true;
				jumpAnim.Reset();
			}
		}
		else
		{
			position.x--;
			if (position.x <= initialpos.x - distance)
			{
				direction = false;
			}
		}
	}
	else
	{
		if (!direction)
		{
			position.y++;
			if (position.y >= initialpos.y + distance)
			{
				direction = true;
			}
		}
		else
		{
			position.y--;
			if (position.y <= initialpos.y - distance)
			{
				direction = false;
			}
		}
	}*/
	/*currentAnimation = &attackAnim;*/
	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y + 5, &rect);

	//walkingenemy dies
	if (app->livewalkingenemy == 0) die = true;
	if (die) {
		LOG("WALKINGENEMY DIES");
		currentAnimation = &deathAnim;
		if (deathAnim.HasFinished()) { 
			deathAnim.Reset();
			//cuando acabe la animacion falta destruir la textura + collider cuando se muere
		}
	}



	return true;
}

void WalkingEnemy::MoveToPlayer(iPoint& enemyPos, float speed, const DynArray<iPoint>* path)
{
	b2Vec2 vel = pbody->body->GetLinearVelocity();

	if (path->Count() > 0)
	{
		iPoint nextNode;
		if (app->map->pathfinding2->Move(enemyPos, nextNode))
		{
			int dx = nextNode.x - enemyPos.x;
			int dy = nextNode.y - enemyPos.y;

			vel = { dx * speed, dy * speed };

			enemyPos = nextNode;
		}
	}
	pbody->body->SetLinearVelocity(vel);
}

void WalkingEnemy::Attack()
{
	MoveToPlayer(enemyPos, 1.0f, path);
	/*app->attack = true;*/

}

bool WalkingEnemy::CleanUp()
{
	return true;
}

void WalkingEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {
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
	case ColliderType::MOVING_PLATFORM:
		LOG("Collision MOVING_PLATFORM");
		break;
	}
}