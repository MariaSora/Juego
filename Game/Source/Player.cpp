#include "Player.h"
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
	texturePath = parameters.attribute("texturepath").as_string();
	

	idleAnim.LoadAnimation("player", "idleAnim");
	jumpAnim.LoadAnimation("player", "jumpAnim");
	walkAnim.LoadAnimation("player", "walkAnim");
	climbAnim.LoadAnimation("player", "climbAnim");
	climbIdleAnim.LoadAnimation("player", "climbIdleAnim");
	attackAnim.LoadAnimation("player", "attackAnim");
	dieAnim.LoadAnimation("player", "dieAnim");

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	//player = app->tex->Load("Assets/Textures/Pink_Monster.png");

	pbody = app->physics->CreateCircle(position.x - 16, position.y + 16, 13, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	//pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	//attackFx = app->audio->LoadFx("---");
	//jumpFx = app->audio->LoadFx("---");
	//killFx = app->audio->LoadFx("---");

	Ipos = pbody->body->GetTransform(); //Pos inicial

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
		if (app->godmode) {
			LOG("GODMODE ACTIVATED");
		}
		else LOG("GODMODE DEACTIVATED");
	}

	if (app->godmode) {

		b2Vec2 vel = b2Vec2(0,0); 
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
	
	if (app->godmode == false) {

		pbody->body->SetGravityScale(1);
		pbody->body->GetFixtureList()[0].SetSensor(false);

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			isFacingRight = false;
			vel = b2Vec2((-speed / 2) * dt, pbody->body->GetLinearVelocity().y);
			currentAnimation = &walkAnim;
			if (inmovplat) {
				algo--;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			isFacingRight = true;
			vel = b2Vec2((speed / 2) * dt, pbody->body->GetLinearVelocity().y);
			currentAnimation = &walkAnim;
			if (inmovplat) {
				algo++;
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

		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
			if (!saltando) {
				if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) isFacingRight = true;
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) isFacingRight = false;

				vel.y = 0;
				pbody->body->SetLinearVelocity(vel);
				pbody->body->ApplyLinearImpulse(b2Vec2(0, GRAVITY_Y * 0.1), pbody->body->GetWorldCenter(), true);
				saltando = true;
				inmovplat = false;
				algo = 0;
			}
		}

		if (saltando) {
			currentAnimation = &jumpAnim;
			//app->audio->PlayFx(jumpFx);
		}
			

		if (position.y >= 630 || app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
			die = true;
		}
		if (app->vida == 0) die = true; /*holis = true;*/
		if (die) {
			LOG("PLAYER DIES");
			currentAnimation = &dieAnim;
			if (currentAnimation->HasFinished()) { 
				dieAnim.Reset();
				//pbody->body->SetTransform(b2Vec2(Ipos.p.x, Ipos.p.y), 0);
				die = false;	

			}
		}

		//ataque personaje
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {   
			//app->audio->PlayFx(attackFx); 
			currentAnimation = &attackAnim;
		}
	}

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame(); 

	if (isFacingRight) {
		app->render->DrawTexture(texture, position.x + 8, position.y, &rect, 1, SDL_FLIP_NONE);
	}
	else {
		app->render->DrawTexture(texture, position.x + 8, position.y, &rect, 1, SDL_FLIP_HORIZONTAL);
	}

	if (inmovplat) {
		b2Vec2 algo1 = movingplatform->pbody->body->GetTransform().p;
		algo1.x += PIXEL_TO_METERS(algo);
		pbody->body->SetTransform(algo1, 0);
		if (pbody->body->GetLinearVelocity().y > 0) {
			inmovplat = false;
		}
	}

	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::FLYINGENEMY:
	/*	app->audio->PlayFx(killFx);*/
		LOG("Collision FLYINGENEMY");
		/*die = true;*/
		break;
	case ColliderType::WALKINGENEMY:
	/*	app->audio->PlayFx(killFx);*/
		LOG("Collision WALKINGENEMY");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		//app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::TUTORIAL:
		LOG("Collision TUTORIAL");
		break;
	case ColliderType::PLATFORM:
		saltando = false;
		touchingP = true;
		jumpAnim.Reset();
	/*	if (holis) {
			dieAnim.Reset();
			holis = false;
		}*/
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
		inmovplat = true;
		LOG("Collision MOVING_PLATFORM");
		break;
	}
}

void Player::OnExitCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::ITEM:
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
