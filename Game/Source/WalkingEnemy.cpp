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
	drawPath2 = parameters.attribute("path2").as_string(); 
	drawPath3 = parameters.attribute("path3").as_string(); 
	//distance = parameters.attribute("distance").as_int();

	idleAnim.LoadAnimation("walkingEnemy", "idleAnim");
	walkAnim.LoadAnimation("walkingEnemy", "walkAnim");
	deathAnim.LoadAnimation("walkingEnemy", "deathAnim");
	attackAnim.LoadAnimation("walkingEnemy", "attackAnim");
	jumpAnim.LoadAnimation("walkingEnemy", "jumpAnim");
	damageAnim.LoadAnimation("walkingEnemy", "damageAnim");

	return true;
}

bool WalkingEnemy::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	texture2 = app->tex->Load(drawPath2);
	texture3 = app->tex->Load(drawPath3);
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

	b2Vec2 vel = pbody->body->GetLinearVelocity();

	if (playerPos.x > enemyPos.x) isFacingRight = true;
	if (playerPos.x < enemyPos.x) isFacingRight = false;

	enemyPos = app->map->WorldToMap(position.x, position.y);
	playerPos = app->map->WorldToMap(app->scene->GetPlayer()->position.x - 10, position.y);
	//app->render->DrawTexture(texture, position.x, position.y);
	if (type)
	{
		if (app->WalkingEnemyAlive2) {
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
			if (app->scene->player->touchingP) {
				if (enemyPos.x - playerPos.x <= 5 && enemyPos.x - playerPos.x >= -5) {
					Attack();
				}
				else {
					vel = { 0,0 };
					pbody->body->SetLinearVelocity(vel);
				}
			}
			else {
				vel = { 0,0 };
				pbody->body->SetLinearVelocity(vel);
			}
			currentAnimation = &jumpAnim;
			if (jumpAnim.HasFinished()) jumpAnim.Reset();
		}

		//walkingenemy dies
		if (position.y >= 630) 	app->WalkingEnemyAlive2 = false;

		if (!app->WalkingEnemyAlive2) {
			currentAnimation = &deathAnim;
			vel = { 0,0 };
			pbody->body->SetLinearVelocity(vel);
			if (deathAnim.HasFinished()) {
				//currentAnimation = &idleAnim;
				SDL_DestroyTexture(texture); 
				app->map->pathfinding->ClearLastPath();
				/*app->physics->world->DestroyBody(pbody->body);*/
				//SDL_DestroyRenderer(this);
			}
		}
	}
	else
	{
		if (app->WalkingEnemyAlive) {
			if (enemyPos.x - playerPos.x <= 10 && enemyPos.x - playerPos.x >= -10)
			{
				app->map->pathfinding3->CreatePath(enemyPos, playerPos);
				path = app->map->pathfinding3->GetLastPath();
				if (app->physics->debug)
				{
					for (uint i = 0; i < path->Count(); i++)
					{
						iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
						app->render->DrawTexture(texture3, pos.x, pos.y);
					}
				}
			}
			if (app->scene->player->touchingP) {
				if (enemyPos.x - playerPos.x <= 10 && enemyPos.x - playerPos.x >= -10) {
					Attack();
				}
				else {
					vel = { 0,0 };
					pbody->body->SetLinearVelocity(vel);
				}
			}
			else {
				vel = { 0,0 };
				pbody->body->SetLinearVelocity(vel);
			}

			if (!app->statewalkingenemy) {
				idleAnim.Reset();
				currentAnimation = &attackAnim;
				if (attackAnim.HasFinished()) {
					for (b2ContactEdge* ce = pbody->body->GetContactList(); ce; ce = ce->next) {
						b2Contact* c = ce->contact;
						if (c->GetFixtureA()->GetBody() == app->scene->player->pbody->body) {
							if (app->godmode == false) {
								LOG("Player contact");
								app->vida--;
								app->scene->player->damage = true;
							}
						}
					}
					app->statewalkingenemy = true;
				}
			}
			else {
				attackAnim.Reset();
				currentAnimation = &idleAnim;
				counter++;
				if (counter == 50) {
					counter = 0;
					app->statewalkingenemy = false;
				}
			}
		}
		//walkingenemy dies
		if (position.y >= 630) 	app->WalkingEnemyAlive = false;

		if (!app->WalkingEnemyAlive) {
			currentAnimation = &deathAnim;
			vel = { 0,0 };
			pbody->body->SetLinearVelocity(vel);
			if (deathAnim.HasFinished()) {
				//currentAnimation = &idleAnim;
				SDL_DestroyTexture(texture); 
				app->map->pathfinding->ClearLastPath();
				/*app->physics->world->DestroyBody(pbody->body);*/
				//SDL_DestroyRenderer(this);
			}
		}
	}


	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentAnimation->Update(); 
	SDL_Rect rect = currentAnimation->GetCurrentFrame(); 
	
	if (isFacingRight) {
		app->render->DrawTexture(texture, position.x + 8, position.y, &rect, 1, SDL_FLIP_HORIZONTAL);
	}
	else {
		app->render->DrawTexture(texture, position.x + 8, position.y, &rect, 1, SDL_FLIP_NONE);
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
		if (app->map->pathfinding3->Move(enemyPos, nextNode))
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
		if (app->scene->player->atk = true) {
			if (type) app->WalkingEnemyAlive2 = false;
			if (!type) 	app->WalkingEnemyAlive = false;
		}
		
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