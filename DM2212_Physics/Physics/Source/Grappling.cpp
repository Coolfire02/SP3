
#include "Grappling.h"
#include "Application.h"
#include "MeshBuilder.h"


GrapplingAbility::GrapplingAbility(Mesh* mesh) : Ability('Z', ABILITY_GRAPPLER, 5.0f, mesh)
{
	//Store keyboard instance
	input = Input::GetInstance();

	isGrappling = false;
	maxVel = 0;
	gradualVelTimer = 0;
	playerPhysics = nullptr;
}

GrapplingAbility::~GrapplingAbility()
{
}

void GrapplingAbility::Update(double dt)
{
	//attach grappling hook
	if (input->IsKeyPressed(buttonChar))
	{
		double x, y;
		CursorToWorldPosition(x, y);

		//check if mouse clicked pos is on any tile block
		for (GameObject* go : goManager->GetStationaryList())
		{
			if ((x > go->pos.x - go->scale.x && x < go->pos.x + go->scale.x) && (y > go->pos.y - go->scale.y && y < go->pos.y + go->scale.y))
			{
				temp = Vector3(x, y, 0);
				initialDisplacement = temp - playerPos;
				isGrappling = true;
				grapplingHook.active = true;
			}
		}


	}
	else if (input->IsKeyReleased(buttonChar))	//detach grappling hook
	{
		isGrappling = false;
		grapplingHook.active = false;
	}


	if (isGrappling)
	{
		Vector3 displacement = temp - playerPos;
		Vector3 displacement3 = playerPos - temp;

		grapplingHook.scale = Vector3(displacement.Length() / 2, 1, 1);
		grapplingHook.pos = playerPos + Vector3(displacement.x / 2, displacement.y / 2, 0);
		grapplingHook.physics->SetNormal(displacement.Normalized());

		//Vector3 halfDisplacement = Vector3(displacement.x / 2, displacement.y / 2, displacement.z);
		playerPhysics->AddVelocity(displacement);

		//playerPhysics->AddVelocity(Vector3(initialDisplacement.x, 0, 0));
		maxVel = 100;

		//if (playerPhysics->GetVelocity().x > 0)
		//{
		//	if (playerPos.x >= temp.x - displacement3.x)
		//	{
		//		//playerPhysics->AddVelocity(Vector3(0, initialDisplacement.Length(), 0));
		//		std::cout << "Stopped grappling" << std::endl;
		//		std::cout << displacement3 << std::endl;
		//		std::cout << playerPos.x << std::endl;
		//		isGrappling = false;
		//		grapplingHook.active = false;
		//	}
		//}
		//else
		//{
		//	if (playerPos.x <= temp.x - displacement3.x)
		//	{
		//		//playerPhysics->AddVelocity(Vector3(0, initialDisplacement.Length(), 0));
		//		std::cout << "Stopped grappling" << std::endl;
		//		std::cout << displacement3 << std::endl;
		//		std::cout << playerPos.x << std::endl;
		//		isGrappling = false;
		//		grapplingHook.active = false;
		//	}
		//}
		//std::cout << maxVel << std::endl;
	}
	else if (playerPhysics != nullptr)
	{
		if (playerPhysics->GetVelocity().x < 1 && playerPhysics->GetVelocity().x > -1)
		{
			std::cout << "DONE" << std::endl;
			maxVel = 20;
			endGrappled = true;
		}
	}
}

void GrapplingAbility::UpdatePlayer(Vector3& pos, Physics* _playerPhysics, float& _maxVel)
{
	playerPhysics = _playerPhysics;
	if (isGrappling || !endGrappled)
	{
		playerPos = pos;
		_maxVel = maxVel;
	}
}


void GrapplingAbility::Render()
{
	if (grapplingHook.active)
	{
		float angle = Math::RadianToDegree(atan2(grapplingHook.physics->GetNormal().y, grapplingHook.physics->GetNormal().x));
		scene->modelStack.PushMatrix();
		scene->modelStack.Translate(grapplingHook.pos.x, grapplingHook.pos.y, grapplingHook.pos.z);
		scene->modelStack.Rotate(angle + grapplingHook.physics->GetRotateZ(), 0, 0, 1);
		scene->modelStack.Scale(grapplingHook.scale.x, grapplingHook.scale.y, grapplingHook.scale.z);
		scene->RenderMesh(grapplingHook.mesh, false);
		scene->modelStack.PopMatrix();
	}
}

ABILITY_TYPE GrapplingAbility::GetAbilityType()
{
	return type;
}


void GrapplingAbility::CursorToWorldPosition(double& theX, double& theY)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	// convert to world space
	x /= (w / camera->screenWidth);
	y = h - y;
	y /= (h / camera->screenHeight);
	x -= camera->screenWidth * 0.5 - camera->position.x;
	y -= camera->screenHeight * 0.5 - camera->position.y;

	theX = x;
	theY = y;
}