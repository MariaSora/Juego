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
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y);

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {  
		saltando = true;
		
		//vel = b2Vec2(GRAVITY_X, -speed + GRAVITY_Y);
	}

	if (saltando) {
		progreso_salto += dt;
		if (progreso_salto > duracion_salto) {
			progreso_salto = duracion_salto;
		}
		 
		vel = b2Vec2(GRAVITY_X, -speed + GRAVITY_Y ); 

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			//Con esto se mueve en diagonal para saltar de una plataforma a otra
			vel = b2Vec2(- speed * dt, -speed + GRAVITY_Y);
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			
			vel = b2Vec2(speed * dt, -speed + GRAVITY_Y);
			
		}

		if (progreso_salto == duracion_salto) {
			progreso_salto = 0;
			saltando = false;
		}
	}

	else if (!saltando) {
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) { 
			vel = b2Vec2(-speed * dt, -GRAVITY_Y); 

			//if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) { //Con esto se mueve en diagonal para saltar de una plataforma a otra
			//	vel = b2Vec2(-speed * dt, -speed + GRAVITY_Y); 
			//} 
			//else {
			//	vel = b2Vec2(-speed * dt, -GRAVITY_Y);
			//}
		}

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) { 
			vel = b2Vec2(speed * dt, -GRAVITY_Y); 
			/*if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				vel = b2Vec2(speed * dt, -speed + GRAVITY_Y);
			}
			else {
				vel = b2Vec2(speed * dt, -GRAVITY_Y);
			}*/

		}
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}
	if (position.x < 100) {
		position.x = 100;
	}
	if (app->render->camera.x != position.x) {
		app->render->camera.x = -position.x+100;
		//app->render->camera.x--;
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

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
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}