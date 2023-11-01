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
	
	idleAnim.PushBack({7, 4, 17, 28});

	jumpAnim.PushBack({6, 295, 22, 32});
	jumpAnim.PushBack({ 39, 295, 22, 32 });
	jumpAnim.PushBack({ 72, 295, 22, 32 });
	jumpAnim.PushBack({ 102, 295, 22, 32 });
	jumpAnim.PushBack({ 134, 295, 22, 32 });
	jumpAnim.PushBack({ 167, 295, 22, 32 });
	jumpAnim.PushBack({ 200, 295, 22, 32 });
	jumpAnim.PushBack({ 230, 295, 22, 32 });
	jumpAnim.loop = false;
	jumpAnim.speed = 0.3f;

	walkAnim.PushBack({ 10, 460, 18, 28 });
	walkAnim.PushBack({ 42, 460, 18, 28 });
	walkAnim.PushBack({ 74, 460, 18, 28 });
	walkAnim.PushBack({ 106, 460, 18, 28 });
	walkAnim.PushBack({ 138, 460, 18, 28 });
	walkAnim.PushBack({ 170, 460, 18, 28 });
	walkAnim.loop = true;
	walkAnim.speed = 0.2f;

	climbAnim.PushBack({ 6, 122, 20, 29 });
	climbAnim.PushBack({ 39, 122, 20, 29 });
	climbAnim.PushBack({ 71, 122, 20, 29 });
	climbAnim.PushBack({ 101, 122, 20, 29 });
	climbAnim.loop = true;
	climbAnim.speed = 0.1f;


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

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, pbody->body->GetLinearVelocity().y);
	currentAnimation = &idleAnim;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { 
		isFacingRight = false; 
		vel = b2Vec2((- speed / 2) * dt, pbody->body->GetLinearVelocity().y);
		currentAnimation = &walkAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { 
		isFacingRight = true;
		vel = b2Vec2((speed / 2) * dt, pbody->body->GetLinearVelocity().y);
		currentAnimation = &walkAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vel = b2Vec2(GRAVITY_X, (- speed / 2) * dt);
		currentAnimation = &climbAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}

	pbody->body->SetLinearVelocity(vel);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		if(!saltando){ 
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) isFacingRight = true;
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) isFacingRight = false;
		
			vel.y = 0;
			pbody->body->SetLinearVelocity(vel);
			pbody->body->ApplyLinearImpulse(b2Vec2(0, GRAVITY_Y * 0.1), pbody->body->GetWorldCenter(), true);
			saltando = true;
		}
	}

	if (saltando) currentAnimation = &jumpAnim; 


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
