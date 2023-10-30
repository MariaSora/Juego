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

	jumpRAnim.PushBack({6, 295, 22, 32});
	jumpRAnim.PushBack({ 39, 295, 22, 32 });
	jumpRAnim.PushBack({ 72, 295, 22, 32 });
	jumpRAnim.PushBack({ 105, 295, 22, 32 });
	jumpRAnim.PushBack({ 138, 295, 22, 32 });
	jumpRAnim.PushBack({ 171, 295, 22, 32 });
	jumpRAnim.PushBack({ 204, 295, 22, 32 });
	jumpRAnim.PushBack({ 237, 295, 22, 32 });
	jumpRAnim.loop = true;
	jumpRAnim.speed = 0.1f;

	jumpLAnim.PushBack({ 6, 540, 22, 32 });
	jumpLAnim.PushBack({ 39, 540, 22, 32 });
	jumpLAnim.PushBack({ 72, 540, 22, 32 });
	jumpLAnim.PushBack({ 105, 540, 22, 32 });
	jumpLAnim.PushBack({ 138, 540, 22, 32 });
	jumpLAnim.PushBack({ 171, 540, 22, 32 });
	jumpLAnim.PushBack({ 204, 540, 22, 32 });
	jumpLAnim.PushBack({ 237, 540, 22, 32 });
	jumpLAnim.loop = true;
	jumpLAnim.speed = 0.1f;

	walkRAnim.PushBack({ 10, 460, 18, 28 });
	walkRAnim.PushBack({ 42, 460, 18, 28 });
	walkRAnim.PushBack({ 74, 460, 18, 28 });
	walkRAnim.PushBack({ 106, 460, 18, 28 });
	walkRAnim.PushBack({ 138, 460, 18, 28 });
	walkRAnim.PushBack({ 170, 460, 18, 28 });
	walkRAnim.loop = true;
	walkRAnim.speed = 0.2f;
	
	walkLAnim.PushBack({ 175, 584, 19, 28 });	
	walkLAnim.PushBack({ 143, 584, 19, 28 });
	walkLAnim.PushBack({ 111, 584, 19, 28 });
	walkLAnim.PushBack({ 78, 584, 19, 28 });
	walkLAnim.PushBack({ 47, 584, 19, 28 });
	walkLAnim.PushBack({ 15, 584, 19, 28 });
	walkLAnim.loop = true;
	walkLAnim.speed = 0.2f;

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
		vel = b2Vec2((- speed / 2) * dt, pbody->body->GetLinearVelocity().y);
		currentAnimation = &walkLAnim;
	
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { 
		vel = b2Vec2((speed / 2) * dt, pbody->body->GetLinearVelocity().y);
		currentAnimation = &walkRAnim;
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
			vel.y = 0;
			pbody->body->SetLinearVelocity(vel);
			pbody->body->ApplyLinearImpulse(b2Vec2(0, GRAVITY_Y * 0.1), pbody->body->GetWorldCenter(), true);
			saltando = true;
		}
	}

	//Set the velocity of the pbody of the player
	

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	currentAnimation->Update();

	SDL_Rect rect = currentAnimation->GetCurrentFrame(); 
	app->render->DrawTexture(texture, position.x, position.y, &rect);


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
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		saltando = false;
		LOG("Collision UNKNOWN");
		break;
	}
}