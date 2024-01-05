#include "Player.h"

#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "transparentWall.h"
#include "Portal.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
	remainingJumpSteps = 0;
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	app->vida = parameters.attribute("vida").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	texturePath2 = parameters.attribute("texturepath2").as_string();

	idleAnim.LoadAnimation("player", "idleAnim");
	jumpAnim.LoadAnimation("player", "jumpAnim");
	walkAnim.LoadAnimation("player", "walkAnim");
	climbAnim.LoadAnimation("player", "climbAnim");
	climbIdleAnim.LoadAnimation("player", "climbIdleAnim");
	attackAnim.LoadAnimation("player", "attackAnim");
	dieAnim.LoadAnimation("player", "dieAnim");
	damagedAnim.LoadAnimation("player", "damagedAnim");
	life0.LoadAnimation("player", "life0");
	life1.LoadAnimation("player", "life1");
	life2.LoadAnimation("player", "life2");
	life3.LoadAnimation("player", "life3");
	life4.LoadAnimation("player", "life4");
	life5.LoadAnimation("player", "life5");

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	texture2 = app->tex->Load(texturePath2);

	//player = app->tex->Load("Assets/Textures/Pink_Monster.png");

	pbody = app->physics->CreateCircle(position.x - 16, position.y + 16, 13, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	Ipos = pbody->body->GetTransform(); //Pos inicial

	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = app->entityManager->entities.start; item != NULL; item = item->next)
	{
		pEntity = item->data;
		if (pEntity->type == EntityType::WALL)
		{
			wall = (transparentWall*)pEntity;
		}
		else if (pEntity->type == EntityType::PORTAL)
		{
			portal = (Portal*)pEntity;
		}
		else if (pEntity->type == EntityType::MOVINGPLATFORM)
		{
			movingplatform = (MovingPlatform*)pEntity;
		}
	}

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, pbody->body->GetLinearVelocity().y); 
	currentAnimation = &idleAnim;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) { 
		app->godmode = false;
		pbody->body->SetTransform(b2Vec2(Ipos.p.x, Ipos.p.y), 0);
	}
	//hacer nivel 2
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		app->godmode = false;
		pbody->body->SetTransform(b2Vec2(Ipos.p.x, Ipos.p.y), 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		app->godmode = false;
		pbody->body->SetTransform(b2Vec2(Ipos.p.x, Ipos.p.y), 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		app->godmode = !app->godmode;
		if (!app->godmode) {
			app->audio->PlayMusic("Assets/Audio/Music/backgroundMusic.ogg");
			LOG("GODMODE DEACTIVATED");
		}

		else if (app->godmode) {
			app->audio->PlayMusic("Assets/Audio/Music/godmode.ogg");
			LOG("GODMODE ACTIVATED");
		}
	}

	if (app->godmode) {

		b2Vec2 vel = b2Vec2(0, 0);
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel = b2Vec2((-speed / 2) * dt, 0);
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel = b2Vec2((speed / 2) * dt, 0);
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			vel = b2Vec2(0, (-speed / 2) * dt);
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			vel = b2Vec2(0, (speed / 2) * dt);
		}
		pbody->body->SetLinearVelocity(vel);
		pbody->body->SetGravityScale(0);
		pbody->body->GetFixtureList()[0].SetSensor(true);

	}

	if (app->godmode == false && !die) {

		pbody->body->SetGravityScale(1);
		pbody->body->GetFixtureList()[0].SetSensor(false);

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			isFacingRight = false;
			vel = b2Vec2((-speed / 2) * dt, pbody->body->GetLinearVelocity().y);
			currentAnimation = &walkAnim;
			if (inmovplat) {
				moving.x--;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			isFacingRight = true;
			vel = b2Vec2((speed / 2) * dt, pbody->body->GetLinearVelocity().y);
			currentAnimation = &walkAnim;
			if (inmovplat) {
				moving.x++;
			}
		}
		

		if ((touchingP && touchingS) || touchingS)
		{
			pbody->body->SetGravityScale(0);
			vel.y = 0;
			currentAnimation = &climbIdleAnim;
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				vel = b2Vec2(GRAVITY_X, (-speed / 2) * dt);
				currentAnimation = &climbAnim;
			}

			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				vel = b2Vec2(GRAVITY_X, (speed / 2) * dt);
				currentAnimation = &climbAnim;
			}
		}

		pbody->body->SetLinearVelocity(vel);

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !touchingS) {
			if (!saltando) {
				app->audio->PlayFx(app->audio->jumpFx);
				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) isFacingRight = true;
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) isFacingRight = false;

				vel.y = 0;
				pbody->body->SetLinearVelocity(vel);
				pbody->body->ApplyLinearImpulse(b2Vec2(0, GRAVITY_Y * 0.1), pbody->body->GetWorldCenter(), true);
				saltando = true;
				inmovplat = false;
				moving.x = 0;
			}
		}

		if (inmovplat) {
			b2Vec2 platformPos = movingplatform->pbody->body->GetTransform().p;
			platformPos.x += PIXEL_TO_METERS(moving.x);
			platformPos.y -= PIXEL_TO_METERS(20);
			pbody->body->SetTransform(platformPos, 0);
			if (pbody->body->GetLinearVelocity().y > 0) {
				inmovplat = false;
			}
		}

		if (saltando) {
			currentAnimation = &jumpAnim;
		}
		
		//ataque personaje
		
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
			app->audio->PlayFx(app->audio->attackFx);
			anim = true; 
			attackAnim.Reset();
		}
		if (anim) {
			currentAnimation = &attackAnim;
			atk = true;
			if (attackAnim.HasFinished()) {
				anim = false; 
				atk = false;
			}
		}
	}

	if (app->godmode == false) {
		if (position.y >= 630 || app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
			die = true;
		}
		if (app->vida <= 0) {
			die = true;
			app->audio->PlayFx(app->audio->deathFx);
		};
		if (die) {
			LOG("PLAYER DIES");
			currentAnimation = &dieAnim;
			if (dieAnim.HasFinished()) {	
				dieAnim.Reset();
				pbody->body->SetTransform(b2Vec2(Ipos.p.x, Ipos.p.y), 0);
				/*app->scene->flyingEnemy->position.x = app->scene->flyingEnemy->initialPos.x;
				app->scene->flyingEnemy->position.y = app->scene->flyingEnemy->initialPos.y;	
				app->scene->walkingEnemy->position.x = app->scene->walkingEnemy->initialPos.x;
				app->scene->walkingEnemy->position.y = app->scene->walkingEnemy->initialPos.y;*/
				
			/*	if (!app->FlyingEnemyAlive) {
					app->FlyingEnemyAlive = true;
				}
				if (!app->SecondFlyingEnemyAlive) {
					app->SecondFlyingEnemyAlive = true;
				}
				if (!app->WalkingEnemyAlive) {
					app->WalkingEnemyAlive = true;
				}*/

				die = false;
				app->vida = 5; 
			}
		}

		if (damage) {
			currentAnimation = &damagedAnim;
			if (damagedAnim.HasFinished()) {
				damagedAnim.Reset();
				damage = false; 
			}
		}
	}
	if (app->vida == 0) currentLifeAnimation = &life0; life1.Reset();
	if (app->vida == 1) currentLifeAnimation = &life1; life2.Reset();
	if (app->vida == 2) currentLifeAnimation = &life2; life3.Reset();
	if (app->vida == 3) currentLifeAnimation = &life3; life4.Reset();
	if (app->vida == 4) currentLifeAnimation = &life4; life5.Reset(); 
	if (app->vida == 5) currentLifeAnimation = &life5; life0.Reset();
	
	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	currentLifeAnimation->Update();
	SDL_Rect rectLife = currentLifeAnimation->GetCurrentFrame(); 
	app->render->DrawTexture(texture2, position.x - 20, position.y - 15, &rectLife);

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame(); 
	
	if (isFacingRight) {
		app->render->DrawTexture(texture, position.x + 8, position.y, &rect, 1, SDL_FLIP_NONE);
	}
	else {
		app->render->DrawTexture(texture, position.x + 8, position.y, &rect, 1, SDL_FLIP_HORIZONTAL);
	}

	//teleport to final position
	if (portal != NULL)
	{
		if (portal->touchingPortal == true) 
		{	
			pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(app->positionportal2.x), PIXEL_TO_METERS(app->positionportal2.y)), 0);
			app->render->camera.x = app->positionportal2.x;
			portal->closePortal.Reset();
			app->audio->PlayFx(app->audio->winFx);
		}
	}

	portal->closePortal.Update();
	SDL_Rect rect2 = portal->closePortal.GetCurrentFrame();
	app->render->DrawTexture(portal->texture, app->positionportal2.x + 2, app->positionportal2.y - 5, &rect2, 1, SDL_FLIP_NONE);

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	switch (physB->ctype)
	{
	case ColliderType::FLYINGENEMY:
		app->audio->PlayFx(app->audio->enemyDeath);
		LOG("Collision FLYINGENEMY");
		/*die = true;*/
		break;
	case ColliderType::WALKINGENEMY:
		LOG("Collision WALKINGENEMY");
		break;
	case ColliderType::PARTICLES: 
		LOG("Collision PARTICLES");
		currentAnimation = &damagedAnim;
		break;
	case ColliderType::HEALITEM:
		LOG("Collision ITEM");
		if(app->vida < 5) app->vida++; 
		break;
	case ColliderType::CANDYITEM:
		LOG("Collision ITEM");
		//score +1
		break;
	case ColliderType::WALL:

		if (wall != NULL)
		{
			wall->touchingW = true;
		} 
		break;
	case ColliderType::PORTAL:
		if (portal != NULL)
		{
			portal->touchingPortal = true;
			app->audio->PlayFx(app->audio->teleportFx);
		}
		break;
	case ColliderType::TUTORIAL:
		LOG("Collision TUTORIAL");
		break;
	case ColliderType::PLATFORM:
		saltando = false;
		touchingP = true;
		jumpAnim.Reset();
		LOG("Collision PLATFORM");
		break;
	case ColliderType::STAIRS:
		touchingS = true; 
		LOG("Collision STAIRS"); 
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::MOVING_PLATFORM:
		saltando = false;
		jumpAnim.Reset();
		movingplatform = (MovingPlatform*)physB->listener;
		if (app->godmode == false) {
			inmovplat = true;
		}
		LOG("Collision MOVING_PLATFORM");
		break;
	}
}

void Player::OnExitCollision(PhysBody* physA, PhysBody* physB)
{
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	switch (physB->ctype)
	{
	case ColliderType::HEALITEM:
		break;
	case ColliderType::WALL:

		if (wall != NULL)
		{
			wall->touchingW = false;
		}
		break;
	case ColliderType::PORTAL:
	
		if (portal != NULL)
		{
			portal->touchingPortal = false;
		}
		break;
	case ColliderType::PLATFORM:
		touchingP = false;
		break;
	case ColliderType::STAIRS:
		touchingS = false; 
		pbody->body->SetGravityScale(1);
		break;
	case ColliderType::UNKNOWN:
		break;
	case ColliderType::MOVING_PLATFORM:
		break;
	}
}