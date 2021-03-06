#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Trident.h"
#include "../Source/SoundController/SoundController.h"

//TRIDENT FUNCTIONS
Trident::Trident() : Weapon(I_TRIDENT)
{
	isStackable = false;
	input = Input::GetInstance();
	mesh = MeshBuilder::GenerateQuad("trident", Color(1, 1, 1), 2.0f);
	mesh->textureID = LoadTGA("Image/Items/Weapons/Trident.tga");
}
Trident::~Trident()
{
}
void Trident::Init(Camera* cam, GameObjectManager* goManager, GameObject* target)
{
	camera = cam;
	GOmanager = goManager;
	tridentGO = nullptr;
	player = target;
}
void Trident::Update(double dt)
{
	double mouseposx, mouseposy;
	CursorToWorldPosition(mouseposx, mouseposy);

	if (input->IsMousePressed(0) && tridentGO == nullptr)
	{
		mouseDownPos.Set(mouseposx, mouseposy, 0);
		//camera->SetMode(Camera::CENTER);
	}
	if (input->IsMouseReleased(0) && tridentGO == nullptr)
	{
		CSoundController::GetInstance()->PlaySoundByID(TRIDENT);
		tridentGO = new TridentGO;
		tridentGO->Init();
		tridentGO->pos = player->pos;
		tridentGO->physics->pos = player->physics->pos;
		tridentGO->scale.Set(5, 5, 5);
		Vector3 mousePos(mouseposx, mouseposy, 0);
		Vector3 vel = mouseDownPos - mousePos;
		tridentGO->physics->SetVelocity(vel);
		GOmanager->AddGO(tridentGO);
		//camera->SetMode(Camera::EDGE);
	}
	if (tridentGO)
	{
		float theta = atan2f(tridentGO->physics->GetVelocity().y, tridentGO->physics->GetVelocity().x);
		theta -= Math::DegreeToRadian(45);
		Vector3 normal(cos(theta), sin(theta), 0);
		tridentGO->physics->SetNormal(normal);
		if (!checkWithinScreen())
		{
			GOmanager->RemoveGO(tridentGO);
			tridentGO = nullptr;
		}
	}
 }
bool Trident::IsEqual(Item* item1)
{
	Trident* checkApple = static_cast<Trident*>(item1);
	return false;
}
void Trident::CollidedWith(GameObject* go)
{
}
void Trident::CursorToWorldPosition(double& theX, double& theY)
{
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	// convert to world space
	x /= (w / scene->GetScreenWidth());
	y = h - y;
	y /= (h / scene->GetScreenHeight());
	x -= scene->GetScreenWidth() * 0.5 - camera->position.x;
	y -= scene->GetScreenHeight() * 0.5 - camera->position.y;

	theX = x;
	theY = y;
}
bool Trident::checkWithinScreen()
{
	if (tridentGO->pos.x > camera->position.x - scene->GetScreenWidth() * 0.5
		&& tridentGO->pos.x < camera->position.x + scene->GetScreenWidth() * 0.5
		&& tridentGO->pos.y > camera->position.y - scene->GetScreenHeight() * 0.5
		&& tridentGO->pos.y < camera->position.y + scene->GetScreenHeight() * 0.5)
	{
		return true;
	}
	return false;
}

//TRIDENT GO FUNCTIONS
void TridentGO::CollidedWith(GameObject* go)
{
	switch (go->type)
	{
	case GameObject::GO_CRAB:
		go->currentHP -= 10;
		this->pos.x = -100;
		this->physics->pos.x = -100;
		break;
	case GameObject::GO_DRAGON:
		if (go->parent != nullptr)
		{
			go->parent->currentHP -= 10;
			std::cout << "parent curHP: " << go->parent->currentHP << std::endl;
		}
		else //parent (dragon head)
		{
			go->currentHP -= 10;
		}
		this->pos.x = -100;
		this->physics->pos.x = -100;
		break;
	case GameObject::GO_THUNDERBLOCK:
		ThunderBlock* thunder = dynamic_cast<ThunderBlock*>(go);
		thunder->SpawnLightning();
		this->pos.x = -100;
		this->physics->pos.x = -100;
		break;
	}
}
TridentGO::TridentGO()
{
	mesh = MeshBuilder::GenerateQuad("trident", Color(1,1,1), 2.0f);
	mesh->textureID = LoadTGA("Image/Items/Weapons/Trident.tga");
	physics->SetMovable(true);
	physics->SetEnableCollisionResponse(false);
	physics->SetGravity(Vector3(0, -10.f, 0));
	slowedObj = false;
}
TridentGO::~TridentGO()
{
	if (mesh)
	{
		delete mesh;
		mesh = NULL;
	}
}
void TridentGO::Init()
{
}
void TridentGO::Update(double dt)
{
}
