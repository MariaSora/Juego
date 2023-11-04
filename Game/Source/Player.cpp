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
	

	idleAnim.LoadAnimation("idleAnim");
	jumpAnim.LoadAnimation("jumpAnim");
	walkAnim.LoadAnimation("walkAnim");
	climbAnim.LoadAnimation("climbAnim");

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	//player = app->tex->Load("Assets/Textures/Pink_Monster.png");

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 13, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	Ipos = pbody->body->GetTransform(); //Pos inicial

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, pbody->body->GetLinearVelocity().y); 
	currentAnimation = &idleAnim;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) { 
		pbody->body->SetTransform(b2Vec2(Ipos.p.x, Ipos.p.y), 0);
	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) { 
		pbody->body->SetTransform(b2Vec2(Ipos.p.x, Ipos.p.y), 0); 
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_REPEAT) {
		app->godmode = !app->godmode;
	}

	if (app->godmode) {

		b2Vec2 vel = b2Vec2(0,0); 
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { 
			isFacingRight = false; 
			vel = b2Vec2((-speed / 2) * dt, 0); 
			currentAnimation = &walkAnim;
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { 
			isFacingRight = true;
			vel = b2Vec2((speed / 2) * dt, 0);
			currentAnimation = &walkAnim;
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) { 
			vel = b2Vec2(0, (-speed / 2) * dt); 
			currentAnimation = &climbAnim;
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
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			isFacingRight = true;
			vel = b2Vec2((speed / 2) * dt, pbody->body->GetLinearVelocity().y);
			currentAnimation = &walkAnim;
		}

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			vel = b2Vec2(GRAVITY_X, (-speed / 2) * dt);
			currentAnimation = &climbAnim;
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			//
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
			}
		}

		if (saltando) currentAnimation = &jumpAnim;

	}

	

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame(); 

	if (isFacingRight) {
		app->render->DrawTexture(texture, position.x, position.y, &rect, 1, SDL_FLIP_NONE);
	}
	else {
		app->render->DrawTexture(texture, position.x, position.y, &rect, 1, SDL_FLIP_HORIZONTAL);
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
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		saltando = false; 
		jumpAnim.Reset();
		LOG("Collision PLATFORM");
		break;
	case ColliderType::STAIRS:
		LOG("Collision STAIRS");
			
		break;
	case ColliderType::TRANSFERABLE:
		LOG("Collision TRANSFERABLE");
		saltando = false;
		jumpAnim.Reset();

		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}

//bool Player::LoadJumpAnim(pugi::xml_node& node, jumpAnims* jump)
//{
//	return false;
//}
//
//bool Player::LoadAllJumpAnims(pugi::xml_node animNode)
//{
//	return false;
//}
