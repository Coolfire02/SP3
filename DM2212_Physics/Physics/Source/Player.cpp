
#include "Player.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

void Player::Init()
{
	AkeyDown = false;
	DkeyDown = false;

	speed = 1000.0f;

	animatedSprites = MeshBuilder::GenerateSpriteAnimation(4, 3, 5.0f, 5.0f);
	animatedSprites->AddAnimation("idle", 0, 1);
	animatedSprites->AddAnimation("right", 6, 8);
	animatedSprites->AddAnimation("left", 3, 5);
	animatedSprites->PlayAnimation("idle", -1, 1.0f);
	mesh = animatedSprites;
	mesh->textureID = LoadTGA("Image/girlsprite.tga");
}

void Player::Update(double dt)
{
	animatedSprites->Update(dt);

	if (AkeyDown && !Application::IsKeyPressed('A'))
	{
		AkeyDown = false;
		std::cout << "A Key Released" << std::endl;
		animatedSprites->PlayAnimation("idle", -1, 1.0f);
		vel.x += speed * dt;
	}
	else if (!AkeyDown && Application::IsKeyPressed('A'))
	{
		AkeyDown = true;
		std::cout << "A Key Pressed" << std::endl;
		animatedSprites->PlayAnimation("left", -1, 1.0f);
		vel.x -= speed * dt;
	}
	if (DkeyDown && !Application::IsKeyPressed('D'))
	{
		DkeyDown = false;
		std::cout << "D Key Released" << std::endl;
		animatedSprites->PlayAnimation("idle", -1, 1.0f);
		vel.x -= speed * dt;
	}
	else if (!DkeyDown && Application::IsKeyPressed('D'))
	{
		DkeyDown = true;
		std::cout << "D Key Pressed" << std::endl;
		animatedSprites->PlayAnimation("right", -1, 1.0f);
		vel.x += speed * dt;
	}

	pos += vel * dt;
}
