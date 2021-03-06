
#include "Crab.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "../Source/SoundController/SoundController.h"

Crab::Crab() : Enemy(OCEAN_CRAB)
{
}

Crab::~Crab()
{
}

void Crab::Init(GameObject* target, MOVEMENT_TYPE type)
{
	player = target;
	this->currentHP = 30;
	this->maxHP = 30;
	this->type = GO_CRAB;
	state = IDLE;
	mType = type;
	activeRange = 80.0f;
	hostileRange = 40.0f;
	attackRange = 10.0f;
	crabTimer = 6;
	WLARTimer = 6;
	deathTimer = 2;
	tempVel = 0;

	physics->SetMovable(true);

	//ADD ANIMATIONS
	animatedSprites = MeshBuilder::GenerateSpriteAnimation(10, 6, 2.0f, 2.0f);

	animatedSprites->AddAnimation("attackRight", 0, 8);
	animatedSprites->AddAnimation("deathRight", 8, 16);
	animatedSprites->AddAnimation("idleRight", 16, 20);
	animatedSprites->AddAnimation("walkRight", 20, 28);
	animatedSprites->AddAnimation("attackLeft", 28, 36);
	animatedSprites->AddAnimation("deathLeft", 36, 44);
	animatedSprites->AddAnimation("idleLeft", 44, 48);
	animatedSprites->AddAnimation("walkLeft", 48, 56);
	
	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/enemy_crabSprite.tga");

	animatedSprites->PlayAnimation("idleRight", -1, 1.0f);
}

void Crab::Update(double dt)
{ 
	switch (state)
	{
	case IDLE:
		if ((player->pos - pos).Length() < hostileRange) //checks if player within hostile range, changes state to wlar and chases player
		{
			state = WLAR;
			mType = CHASE;
			updateMType(dt);
		}
		else if ((player->pos - pos).Length() < activeRange) //checks if player within range, if not within range then dont do anything
		{
			if (crabTimer <= 0)
			{
				state = WLAR;
				if (mType == LAR)
				{
					tempVel = 5;
				}
				crabTimer = 6;
				WLARTimer = 3;
			}
			else
			{
				crabTimer -= dt;
			}
		}

		if (tempVel <= 0) //plays idle left anim when facing left
		{
			animatedSprites->PlayAnimation("idleLeft", -1, 1.0f);
		}
		else //plays idle right anim when facing right
		{
			animatedSprites->PlayAnimation("idleRight", -1, 1.0f);
		}
		break;
	case WLAR: 
		CSoundController::GetInstance()->PlaySoundByID(CRABWALK); //crab attack sound
		if (mType == LAR)
		{
			if (crabTimer <= 0) //changes back to idle state after 3 seconds
			{
				state = IDLE;
				crabTimer = 3;
			}
			else
			{
				updateMType(dt); 
				crabTimer -= dt;
			}
		}

		if ((player->pos - pos).Length() < hostileRange) //checks if within hostile range, then chases player
		{
			mType = CHASE;
			updateMType(dt);
		}
		else //if not within hostile range, then walk left and right
		{
			mType = LAR;
			updateMType(dt);
		}

		if (tempVel <= 0)  //plays walk left anim when facing right
		{
			animatedSprites->PlayAnimation("walkLeft", -1, 1.0f);
		}
		else //plays walk right anim when facing right
		{
			animatedSprites->PlayAnimation("walkRight", -1, 1.0f);
		}
		break;
	case ATTACK:
		if (timeout > 0)
		{
			timeout -= dt;
		}

		if ((player->pos - pos).Length() < attackRange) //checks if player within attack range
		{
			if (timeout <= 0) //cooldown for attack
			{
				if (tempVel <= 0)
				{
					animatedSprites->Reset();
					animatedSprites->PlayAnimation("attackLeft", 0, 1.0f);
					if (timeout <= 0)
					{
						player->currentHP -= 5;
						timeout = 0.8;
						CSoundController::GetInstance()->PlaySoundByID(CRABATTACK); //crab attack sound
					}
				}
				else
				{
					animatedSprites->Reset();
					animatedSprites->PlayAnimation("attackRight", 0, 1.0f);
					if (timeout <= 0)
					{
						player->currentHP -= 5;
						timeout = 0.8;
						CSoundController::GetInstance()->PlaySoundByID(CRABATTACK); //crab attack sound
					}
				}
			}
		}
		else //if not within attack range, then set state back to WLAR
		{
			state = WLAR;
			crabTimer = 0;
		}
		break;

	case DIE:
		if (tempVel <= 0)
		{
			animatedSprites->PlayAnimation("deathLeft", 0, 1.0f);
			if (deathTimer <= 0)
			{
				dead = true;
				deathTimer = 0;
				return;
			}
			else {
				deathTimer -= dt;
			}
		}
		else
		{
			animatedSprites->PlayAnimation("deathRight", 0, 1.0f);
			if (deathTimer <= 0)
			{
				dead = true;
				deathTimer = 0;
				return;
			}
			else {
				deathTimer -= dt;
			}
		}
		break;
	}

	if (currentHP <= 0 && state != DIE)
	{
		state = DIE;
		deathTimer = 2;
	}

	animatedSprites->Update(dt);
}

void Crab::updateMType(double dt)
{
	switch (mType)
	{
	case LAR:
		if (WLARTimer <= 0) //if WLAR timer less than 0, change direction it faces
		{
			tempVel *= -1;
			WLARTimer = 3;
		}
		else
		{
			WLARTimer -= dt;
		}
		physics->SetVelocity(Vector3(tempVel, physics->GetVelocity().y, physics->GetVelocity().z));		
		break;
	case CHASE:
		tempVel = (player->pos - pos).Normalized().x * 10;
		physics->SetVelocity(Vector3(tempVel, physics->GetVelocity().y, physics->GetVelocity().z));
		if ((player->pos - pos).Length() < attackRange)
		{
			state = ATTACK;
		}
		break;
	}
}

void Crab::CollidedWith(GameObject* go) //walk into a wall
{
	switch (go->geoTypeID)
	{
	case SceneBase::GEO_OCEAN_1_MIDRIGHT:
	case SceneBase::GEO_OCEAN_1_MIDLEFT:
	case SceneBase::GEO_OCEAN_SEASHELL1:
	case SceneBase::GEO_OCEAN_SEASHELL2:
	case SceneBase::GEO_OCEAN_BOTTLE:
	case SceneBase::GEO_OCEAN_1_BOTLEFT:
	case SceneBase::GEO_OCEAN_1_BOTRIGHT:
	case SceneBase::GEO_OCEAN_1_TOPLEFT:
	case SceneBase::GEO_OCEAN_1_TOPRIGHT:
		tempVel *= -1;
		WLARTimer = 3;
		break;
	}
}
