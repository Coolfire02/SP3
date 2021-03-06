#include "LightBullet.h"
#include "Debug.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "../Source/SoundController/SoundController.h"

LightBullet::LightBullet(Vector3 scale, GameObject* attachedPlayer, float _bulletSpeed)
	: Bullet(nullptr, -1, SHAPE_TYPE::CIRCLE, BULLET_TYPE::LIGHT, scale, false, 1.f, _bulletSpeed)
	, attachedPlayer(attachedPlayer)
{
	//ANIMATED SPRITE
	mesh = MeshBuilder::GenerateQuad("Wall", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image/lightBullet.tga");

	aliveTimer = 0.0;
	
	//enableCollision = false;
	//physics->SetEnableUpdate(false);
}

LightBullet::~LightBullet()
{
}

void LightBullet::Init()
{
}

void LightBullet::Update(double dt)
{
	aliveTimer += dt;

	if (aliveTimer > 10)
		this->dead = true;
}

void LightBullet::CollidedWith(GameObject* go)
{
	if (go->type != GO_PLAYER)
	{
		CSoundController::GetInstance()->PlaySoundByID(LIGHTBULLET_COLLIDED);
		this->dead = true;
		go->MinusCurrHealth(5);
	}
}

GameObject* LightBullet::Clone()
{
	LightBullet* cloned = new LightBullet(this->scale, this->attachedPlayer, this->bulletSpeed);
	//GameObject::CloneValues(this, cloned);
	return cloned;
}
