﻿#include "SceneJungle.h"
#include "GL\glew.h"
#include "MeshBuilder.h"
#include "Application.h"
#include "LoadTGA.h"
#include <sstream>
#include "LevelLoader.h"

#include "Debug.h"

//Entity Includes
#include "Player.h"

//...

SceneJungle::SceneJungle()
{
	goManager = new GameObjectManager();
}

SceneJungle::~SceneJungle()
{
	go = NULL;
	input = NULL;
}

void SceneJungle::Init()
{
	SceneBase::Init();
	goManager->Init();

	// Calculating aspect ratio
	m_screenHeight = 100.f;
	m_screenWidth = m_screenHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	m_worldHeight = m_screenHeight * 3;
	m_worldWidth = m_screenWidth * 5;

	//Camera init
	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.SetLimits(m_screenWidth, m_screenHeight, m_worldWidth, m_worldHeight);

	//Inventory init
	inventory = new Inventory();
	inventory->Init(this);

	//Physics code here
	m_speed = 1.f;
	Math::InitRNG();
	grappler = nullptr;

	//Store keyboard instance
	input = Input::GetInstance();

	//Level Loading
	std::vector<GameObject*> tiles;
	if(LevelLoader::GetInstance()->LoadTiles("JUNGLE_1_1", this->meshList, this->tileSize, tiles, gridLength, gridHeight))
		DEBUG_MSG("Level Did not load successfully");
	for (auto& go : tiles)
	{
		if (go->geoTypeID == GEOMETRY_TYPE::GEO_PLAYER_GIRL1)
		{
			player = new Player();
			player->active = true;
			player->scale = go->scale;
			player->pos = go->pos;
			player->physics = go->physics->Clone();
			player->physics->SetInelasticity(0.99f);
			player->physics->SetIsBouncable(false);
			player->Init(Player::PLATFORMER, goManager, inventory);

			player->AddBottomSprite();
			player->bottomSprite->mesh = meshList[GEO_WALL];
			goManager->AddGO(player);


			DEBUG_MSG("From Phy Editor: " << player->scale);
			

			//Delete Grid Player
			delete go;
			go = nullptr;
		}
	}
	tiles.erase(std::remove(tiles.begin(), tiles.end(), nullptr), tiles.end());
	
	//Add all remainding tiles
	goManager->AddAllGO(tiles);


	//Physics Test Initialisations
	// PILLARs
	

	//GameObject* go2 = new GameObject;
	//go2->type = GameObject::GO_WALL;
	//go2->physics->SetNormal(Vector3(cos(Math::DegreeToRadian(45)), sin(Math::DegreeToRadian(45)), 0));
	//go2->physics->shapeType = RECTANGLE;
	//go2->pos.Set(m_screenWidth * 0.25f, m_screenHeight * 0.15f, 0);
	//go2->scale.Set(10, 10, 1);
	//go2->physics->SetMovable(false);
	//go2->active = true;
	//go2->mesh = meshList[GEO_WALL];
	//goManager->AddGO(go2);

	//go2 = new GameObject;
	//go2->type = GameObject::GO_WALL;
	//go2->physics->SetVelocity(Vector3(1, 0, 0));
	//go2->physics->shapeType = RECTANGLE;
	//go2->pos.Set(m_screenWidth * 0.5f, m_screenHeight * 0.35f, 0);
	//go2->scale.Set(5, 5, 1);
	//go2->physics->SetMovable(true);
	//go2->active = true;
	//go2->mesh = meshList[GEO_WALL];
	//goManager->AddGO(go2);

	//test player
	go = new GameObject;
	go->type = GameObject::GO_TILE;
	go->physics->shapeType = RECTANGLE;
	go->physics->SetMass(3);
	go->pos.Set(m_screenWidth * 0.5f, m_screenHeight * 0.5f, 0);
	go->physics->SetNormal(Vector3(0, 1, 0));
	go->scale.Set(2.5, 2.5, 2.5);
	go->physics->SetMovable(true);
	go->physics->SetInelasticity(0.99f);
	go->physics->SetIsBouncable(false);
	go->active = true;
	go->mesh = meshList[GEO_WALL];
	goManager->AddGO(go);

	//floor
	GameObject* go2 = new GameObject;
	go2->type = GameObject::GO_TILE;
	go2->physics->shapeType = RECTANGLE;
	go2->pos.Set(m_screenWidth * 0.5f, m_screenHeight * 0.15f, 0);
	go2->physics->SetNormal(Vector3(0, 1, 0));
	go2->scale.Set(2.5, 70, 2.5);
	go2->physics->SetMovable(false);
	go2->active = true;
	go2->mesh = meshList[GEO_WALL];
	goManager->AddGO(go2);

	std::cout << "FLOOR: " << go2->pos.y + go2->scale.x << std::endl;

	// ABILITIES
	DashAbility* ability = new DashAbility;
	ability->SetCamera(&camera);
	ability->SetScenePointer(this);

	GrapplingAbility* ability2 = new GrapplingAbility;
	ability2->SetCamera(&camera);
	ability2->SetScenePointer(this);

	player->SetAbilities(ability, ability2); 
}

void SceneJungle::Update(double dt)
{
	SceneBase::Update(dt);
	inventory->Update(dt);
	camera.Update(camera.position, dt);

	if (input->IsKeyPressed('P'))
	{
		std::cout << "PRESSESD P" << std::endl;
		Apple* newApple = new Apple();
		inventory->AddItem(newApple);
		//inventory.setmax(i_apple, 10);
	}
	if (input->IsKeyPressed('O'))
	{
		std::cout << "PRESSESD O" << std::endl;
		Cheese* newCheese = new Cheese();
		inventory->AddItem(newCheese);
	}
	if (input->IsKeyPressed('L'))
	{
		std::cout << "PRESSED L" << std::endl;
		//Cheese* newCheese = new Cheese(2);
		//inventory->AddItem(newCheese);
	}

	if(input->IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(input->IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	//if (input->IsKeyPressed('A'))
	//{
	//	go->physics->SetVelocity(go->physics->GetVelocity() + Vector3(-5,0,0));
	//}
	//else if (input->IsKeyPressed('D'))
	//{
	//	go->physics->SetVelocity(go->physics->GetVelocity() + Vector3(5, 0, 0));
	//}
	//else
	//{
	//	go->physics->SetVelocity(go->physics->GetVelocity() + Vector3(0, 0, 0));
	//}


	//if (input->IsMousePressed(0))
	//{
	//	double x, y;
	//	Application::GetCursorPos(&x, &y);
	//	int w = Application::GetWindowWidth();
	//	int h = Application::GetWindowHeight();
	//	// convert to world space
	//	x /= (w / m_screenWidth);
	//	y = h - y;
	//	y /= (h / m_screenHeight);
	//	std::cout << x << " " << y << std::endl;

	//	temp = Vector3(x, y, 0);
	//	isGrappling = true;
	//	displacement2 = temp - player->pos;
	//	std::cout << "initial pos" << player->pos << std::endl;

	//	grappler = new GameObject;
	//	grappler->active = true;
	//	grappler->mesh = meshList[GEO_WALL];
	//	grappler->enableCollision = false;
	//}

	//if (isGrappling)
	//{
	//	Vector3 displacement = temp - player->pos;
	//	Vector3 displacement3 = player->pos - temp;

	//	grappler->scale = Vector3(displacement.Length() / 2, 1, 1);
	//	grappler->pos = player->pos + Vector3(displacement.x / 2, displacement.y / 2, 0);
	//	grappler->physics->SetNormal(displacement.Normalized());

	//	player->physics->AddVelocity(Vector3(displacement2.x, 0, 0));
	//	if (player->physics->GetVelocity().x > 0)
	//	{
	//		if (player->pos.x >= temp.x - displacement3.x)
	//		{
	//			std::cout << "Stopped grappling" << std::endl;
	//			std::cout << displacement3 << std::endl;
	//			std::cout << player->pos.x << std::endl;
	//			isGrappling = false;
	//			grappler->active = false;
	//		}
	//	}
	//	else
	//	{
	//		if (player->pos.x <= temp.x - displacement3.x)
	//		{
	//			std::cout << "Stopped grappling" << std::endl;
	//			std::cout << displacement3 << std::endl;
	//			std::cout << player->pos.x << std::endl;
	//			isGrappling = false;
	//			grappler->active = false;
	//		}
	//	}
	//}

	goManager->Update(dt);
}

void SceneJungle::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Projection matrix : Orthographic Projection
	Mtx44 projection;
//	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projection.SetToOrtho(-1 * m_screenWidth * 0.5f, m_screenWidth * 0.5f, -1 * m_screenHeight * 0.5f, m_screenHeight * 0.5f, -10, 10);

	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	if (inventory->GetCurrentItem())
	{
		std::stringstream ss;
		ss << "curr item: " << inventory->GetCurrentItemType();

		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1.0f, 1.0f, 1.0f), 4, 10, 10);
	}

	if (grappler && grappler->active)
	{
		float angle = Math::RadianToDegree(atan2f(grappler->physics->GetNormal().y, grappler->physics->GetNormal().x));
		modelStack.PushMatrix();
		modelStack.Translate(grappler->pos.x, grappler->pos.y, grappler->pos.z);
		modelStack.Rotate(angle + grappler->physics->GetRotateZ(), 0, 0, 1);
		modelStack.Scale(grappler->scale.x, grappler->scale.y, grappler->scale.z);
		RenderMesh(grappler->mesh, false);
		modelStack.PopMatrix();
	}
	

	goManager->Render(this);
}

void SceneJungle::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	goManager->Exit();
}