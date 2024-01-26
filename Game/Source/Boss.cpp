#include "Boss.h"
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


Boss::Boss() : Entity(EntityType::BOSS)
{
	name.Create("Boss");
}

Boss::~Boss() {}

bool Boss::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	 
	texturePath = parameters.attribute("texturepath").as_string();
	texturePath2 = parameters.attribute("texturepath2").as_string();//textura barra de vida
	drawPath2 = parameters.attribute("path2").as_string(); //textura para el path

	idleAnim.LoadAnimation("Boss", "idleAnim");
	walkAnim.LoadAnimation("Boss", "walkAnim");
	attackAnim.LoadAnimation("Boss", "attackAnim");
	dieAnim.LoadAnimation("Boss", "dieAnim");
	damagedAnim.LoadAnimation("Boss", "damagedAnim");
	life0.LoadAnimation("Boss", "life0");
	life1.LoadAnimation("Boss", "life1");
	life2.LoadAnimation("Boss", "life2");
	life3.LoadAnimation("Boss", "life3");
	life4.LoadAnimation("Boss", "life4");
	life5.LoadAnimation("Boss", "life5");

	return true;
}

bool Boss::Start() {

	initialpos.y = position.y;
	initialpos.x = position.x;

	//initilize textures
	texture = app->tex->Load(texturePath);
	texture2 = app->tex->Load(drawPath2);
	texture3 = app->tex->Load(texturePath2);

	pbody = app->physics->CreateCircle(position.x, position.y, 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::BOSS; 

	active = true;

	return true;
}

bool Boss::Update(float dt)
{
	pbody->body->SetGravityScale(10);

	if (!active) pbody->body->SetActive(false);

	if (playerPos.x > BossPos.x) isFacingRight = true;
	if (playerPos.x < BossPos.x) isFacingRight = false;

	BossPos = app->map->WorldToMap(position.x, position.y);
	playerPos = app->map->WorldToMap(app->scene->GetPlayer()->position.x - 10, position.y);
	
	currentAnimation = &walkAnim; 

	BossFunctionality();
	LivesManagement();

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentLifeAnimation->Update();
	SDL_Rect rectLife = currentLifeAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture2, (-app->render->camera.x * app->scene->speedUI) + 10, 15, &rectLife);

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame(); 
	
	if (isFacingRight) {
		app->render->DrawTexture(texture, position.x + 8, position.y, &rect, 1, SDL_FLIP_NONE);
	}
	else {
		app->render->DrawTexture(texture, position.x + 8, position.y, &rect, 1, SDL_FLIP_HORIZONTAL);
	}

	return true;
}

void Boss::MoveToPlayer(iPoint& enemyPos, float speed, const DynArray<iPoint>* path)
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

void Boss::Attack()
{
	MoveToPlayer(BossPos, 1.0f, path);
}

void Boss::LivesManagement()
{
	if (vida == 0) currentLifeAnimation = &life0; life1.Reset();
	if (vida == 1) currentLifeAnimation = &life1; life2.Reset();
	if (vida == 2) currentLifeAnimation = &life2; life3.Reset();
	if (vida == 3) currentLifeAnimation = &life3; life4.Reset();
	if (vida == 4) currentLifeAnimation = &life4; life5.Reset();
	if (vida == 5) currentLifeAnimation = &life5; life0.Reset();
}

void Boss::BossFunctionality()
{
	b2Vec2 vel = pbody->body->GetLinearVelocity(); 

	if (vida != 0) {
		if (BossPos.x - playerPos.x <= 10 && BossPos.x - playerPos.x >= -10)
		{
			app->map->pathfinding2->CreatePath(BossPos, playerPos);
			path = app->map->pathfinding2->GetLastPath();
			if (app->physics->debug)
			{
				for (uint i = 0; i < path->Count(); i++)
				{
					iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
					app->render->DrawTexture(texture2, pos.x, pos.y);
					currentAnimation = &walkAnim;
				}
			}
		}
		if (app->scene->player->touchingP) {
			if (BossPos.x - playerPos.x <= 5 && BossPos.x - playerPos.x >= -5) {
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
		
	}

	if (vida == 0) {
		currentAnimation = &dieAnim;
		vel = { 0,0 };
		pbody->body->SetLinearVelocity(vel);
		if (dieAnim.HasFinished()) {
			active = false;
			pbody->body->SetActive(false);
			//currentAnimation = &idleAnim;
			SDL_DestroyTexture(texture);
			app->map->pathfinding->ClearLastPath();
		}
	}
}

bool Boss::CleanUp()
{
	return true;
}

void Boss::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision PLAYER-BOSS");
		if (app->godmode == false) {
			app->vida--;
			app->scene->player->damage = true;
			if (app->scene->player->currentAnimation = &app->scene->player->attackAnim) {
				vida--;
				currentAnimation = &damagedAnim;
			}
		}
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