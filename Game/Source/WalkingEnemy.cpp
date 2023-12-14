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
	pbody = app->physics->CreateCircle(position.x, position.y, 8, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::WALKINGENEMY;
	

	return true;
}

bool WalkingEnemy::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//app->render->DrawTexture(texture, position.x, position.y);
	if (app->statewalkingenemy == false) { 
		idleAnim.Reset();
		currentAnimation = &attackAnim; 
		counter++; 
		if (counter == 50) {
			counter = 0;
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

	if (type) {
		app->render->DrawTexture(texture, position.x + 15, position.y + 5, &rect);
		
	}
	
	if (!type) {
		app->render->DrawTexture(texture, position.x + 35, position.y + 5, &rect);

	}

	return true;
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