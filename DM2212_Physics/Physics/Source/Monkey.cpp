
#include "Monkey.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Flashlight.h"
#include "../Source/SoundController/SoundController.h"

Monkey::Monkey() : Enemy(JG_MONKEY)
{
}

Monkey::~Monkey()
{
}

void Monkey::Init(SceneBase* scene, GameObjectManager* _goManager, Inventory* inventory, Vector3 &target, BulletSpawner* _bulletSpawner)
{
	this->scene = scene;
	this->inventory = inventory;
	this->bulletSpawner = _bulletSpawner;
	this->goManager = _goManager;
	playerPos = &target;

	state = IDLE;

	movement_speed = 20.0f;
	activeRange = 50.0f;
	shootTimer = 1.5f;

	currentHP = 7;
	maxHP = 7; // IN SECONDS

	state_timer = 0;
	state_interval;

	physics->SetMovable(true);
	physics->SetEnableCollisionResponse(true);
	physics->SetGravity(Vector3(0, -98.f, 0));
	this->AddToResponseWhitelist(GO_PLAYER);

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(1, 18, 2.0f, 2.0f);
	animatedSprites->AddAnimation("jumpRight", 0, 2);
	animatedSprites->AddAnimation("jumpLeft", 3, 5);
	animatedSprites->AddAnimation("runRight", 6, 10);
	animatedSprites->AddAnimation("runLeft", 11, 15);
	animatedSprites->AddAnimation("idle", 16, 18);

	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/monkey1.tga");

	animatedSprites->PlayAnimation("idle", -1, 1.0f);
}

void Monkey::Update(double dt)
{ 
	if (currentHP <= 0)
	{
		GameObject* go = new GameObject();
		go->pos = pos;
		go->scale = Vector3(3, 3, 3);
		go->physics->SetMovable(true);
		go->mesh = scene->GetMeshList(SceneBase::GEO_JUNGLE_BANANA);
		go->geoTypeID = SceneBase::GEO_JUNGLE_BANANA;
		this->goManager->AddGO(go);

		dead = true;
		return;
	}
	switch (state)
	{
	case IDLE:
		if ((*playerPos - pos).Length() < activeRange)
		{
			state = JUMP;
		}
		break;
	case JUMP:
		physics->AddVelocity(Vector3((*playerPos - pos).Normalized().x * 5, 50, 0));
		physics->SetVelocity(Vector3(physics->GetVelocity().x, Math::Clamp(physics->GetVelocity().y, -50.f, 50.f), physics->GetVelocity().z));
		state = ATTACK;
		break;
	case ATTACK:
	{
		if (!physics->GetOnGround())
			break;
		if (playerPos->y - 3 > pos.y)
		{
			state = JUMP;
			break;
		}
		else
		{
			if ((*playerPos - pos).Length() > activeRange)
				state = IDLE;
			shootTimer += dt;
			if (!(*playerPos - pos).IsZero())
			{
				physics->SetVelocity(Vector3((*playerPos - this->pos).Normalized().x * movement_speed, physics->GetVelocity().y, physics->GetVelocity().z));
				if (shootTimer >= 2)
				{
					CSoundController::GetInstance()->PlaySoundByID(MONKEY_BANANA);
					bulletSpawner->SpawnBullet(this->pos, Vector3((*playerPos - pos).Normalized().x, 0, 0),(*playerPos - this->pos));
					shootTimer = 0;
				}
			}
		}
	}
	break;
	}

	animatedSprites->Update(dt);

	if (!physics->GetOnGround())
	{
		if (physics->GetVelocity().x > 0)
		{
			animatedSprites->PlayAnimation("jumpRight", -1, 1.0f);
		}
		else if (physics->GetVelocity().x < 0)
		{
			animatedSprites->PlayAnimation("jumpLeft", -1, 1.0f);
		}
	}
	else
	{
		if (physics->GetVelocity().x > 0)
		{
			animatedSprites->PlayAnimation("runRight", -1, 1.0f);
		}
		else if (physics->GetVelocity().x < 0)
		{
			animatedSprites->PlayAnimation("runLeft", -1, 1.0f);
		}
		else
		{
			animatedSprites->PlayAnimation("idle", -1, 1.0f);
		}
	}
}

void Monkey::StartAttackCooldown()
{
	state_interval = 2.0f;
}

bool Monkey::isBeingAttacked()
{
	/*Flashlight* flashlight = dynamic_cast<Flashlight*>(inventory->GetCurrentItem());
	if (flashlight->isIntensified())
	{
		return true;
	}*/
	return false;
}