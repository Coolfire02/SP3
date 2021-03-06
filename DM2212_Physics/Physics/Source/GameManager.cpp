#include "GameManager.h"
#include "MyMath.h"
#include "UIManager.h"
#include "BulletSpawner.h"
#include "BlackHoleBullet.h"

GameManager::GameManager()
{
	for (int i = 0; i < 2; i++)
	{
		currAbility[i] = nullptr;
	}
}

GameManager::~GameManager()
{
	for (int i = 0; i < ABILITY_COUNT; i++)
	{
		if (abilityArray[i])
		{
			delete abilityArray[i];
			abilityArray[i] = NULL;
		}
	}
}

void GameManager::Init()
{
	gameElapsed = 0.f;
	score = 0;
	coinsCollected = 0;

	// set state to GS_INTRO
	game_state = GS_INTRO;

	// set machine part status
	for (int i = 0; i < 4; i++)
	{
		timeMachineStatus[i] = false;
	}

	// create abilities
	Mesh* blackholeIcon = MeshBuilder::GenerateQuad("ability_blackhole", Color(1, 1, 1), 2.0f);
	blackholeIcon->textureID = LoadTGA("Image//black_hole.tga");
	Mesh* recallIcon = MeshBuilder::GenerateQuad("ability_recall", Color(1, 1, 1), 2.0f);
	recallIcon->textureID = LoadTGA("Image//recall_ability.tga");
	Mesh* portalIcon = MeshBuilder::GenerateQuad("ability_portal", Color(1, 1, 1), 2.0f);
	portalIcon->textureID = LoadTGA("Image//PortalAbilityIcon.tga");
	Mesh* grappleIcon = MeshBuilder::GenerateQuad("ability_grapple", Color(1, 1, 1), 2.0f);
	grappleIcon->textureID = LoadTGA("Image//Grappling.tga");
	Mesh* dashIcon = MeshBuilder::GenerateQuad("ability_dash", Color(1, 1, 1), 2.0f);
	dashIcon->textureID = LoadTGA("Image//Dash.tga");
	Mesh* slowtimeIcon = MeshBuilder::GenerateQuad("ability_slowtime", Color(1, 1, 1), 2.0f);
	slowtimeIcon->textureID = LoadTGA("Image//slow_time.tga");

	abilityArray[ABILITY_PORTAL] = new PortalAbility(portalIcon);
	abilityArray[ABILITY_DASH] = new DashAbility(dashIcon);
	abilityArray[ABILITY_BLACKHOLE] = new BlackHoleAbility(new BulletSpawner(), blackholeIcon);
	abilityArray[ABILITY_GRAPPLER] = new GrapplingAbility(grappleIcon);
	abilityArray[ABILITY_SLOWTIME] = new SlowTimeAbility(slowtimeIcon);
	abilityArray[ABILITY_RECALL] = new RecallAbility(3.0f, recallIcon);

	ABILITY_KEYBIND_1 = 'Q';
	ABILITY_KEYBIND_2 = 'Z';
	INTERACT_KEYBIND = 'E';
}

bool GameManager::getMachineStatus(int partNum)
{
	return timeMachineStatus[partNum - 1];
}

void GameManager::setMachineStatus(int partNum, bool obtained)
{
	timeMachineStatus[partNum - 1] = obtained;
}

GameManager::GAMESTORY_STATE GameManager::getGameState()
{
	return game_state;
}

void GameManager::setGameState(GAMESTORY_STATE state)
{
	game_state = state;
}

void GameManager::initAbilities(SceneBase* scene, Camera* camera, GameObjectManager* GOM, GameObject* player)
{
	// General Inits
	for (int i = 0; i < ABILITY_COUNT; i++)
	{
		abilityArray[i]->SetCamera(camera);
		abilityArray[i]->SetScenePointer(scene);
		abilityArray[i]->SetGOManager(GOM);
		abilityArray[i]->SetPlayer(player);
		abilityArray[i]->Init();
	}
	// Unique Inits
	BlackHoleAbility* blackholeAbility = dynamic_cast<BlackHoleAbility*>(abilityArray[ABILITY_BLACKHOLE]);
	blackholeAbility->InitSpawner(GOM, new BlackHoleBullet(scene->GetMesh(SceneBase::GEO_BLACKHOLE), SceneBase::GEO_BLACKHOLE, Vector3(3, 3, 3), player, 40));
}

Ability* GameManager::getCurrAbility(int abilityNum)
{
	return currAbility[abilityNum - 1];
}

void GameManager::setAbility(int abilityNum, ABILITY_TYPE type)
{
	currAbility[abilityNum - 1] = abilityArray[type];
	if (abilityNum == 1)
	{
		currAbility[abilityNum - 1]->SetActivatingKey(ABILITY_KEYBIND_1);
	}
	else
	{
		currAbility[abilityNum - 1]->SetActivatingKey(ABILITY_KEYBIND_2);
	}
}

void GameManager::removeAbility(int abilityNum)
{
	currAbility[abilityNum - 1] = nullptr;
}

void GameManager::addScore(float score)
{
	if (score > 0)
		this->score += score;
}

void GameManager::setScore(float score)
{
	this->score = score;
}

void GameManager::removeScore(float score)
{
	if (this->score -= score < 0)
		this->score = 0;
	this->score -= score;
}

float GameManager::getScore()
{
	return score;
}

void GameManager::addCoins(int coinsCollected)
{
	coinsCollected = abs(coinsCollected);
	this->coinsCollected += coinsCollected;

	//This UI is created in LobbyState
	UIManager::GetInstance()->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS)->getButtonByName("coinvalue")->setText(std::to_string(this->coinsCollected));
}

void GameManager::removeCoins(int coinsCollected)
{
	if (this->coinsCollected -= coinsCollected < 0)
		this->coinsCollected = 0;
	this->coinsCollected -= coinsCollected;

	//This UI is created in LobbyState
	UIManager::GetInstance()->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS)->getButtonByName("coinvalue")->setText(std::to_string(this->coinsCollected));
}

void GameManager::setCoins(int coinsCollected)
{
	this->coinsCollected = Math::Clamp(this->coinsCollected, 0, abs(coinsCollected));
	//This UI is created in LobbyState
	UIManager::GetInstance()->GetButtonManager(UI_TYPE::UNIVERSAL_GAMEPLAY_STATS)->getButtonByName("coinvalue")->setText(std::to_string(this->coinsCollected));
}

void GameManager::resetAll()
{
	for (int i = 0; i < 2; i++)
	{
		currAbility[i] = nullptr;
	}

	gameElapsed = 0.f;
	score = 0;
	coinsCollected = 0;

	// set state to GS_INTRO
	game_state = GS_INTRO;

	// set machine part status
	for (int i = 0; i < 4; i++)
	{
		timeMachineStatus[i] = false;
	}

	ABILITY_KEYBIND_1 = 'Q';
	ABILITY_KEYBIND_2 = 'Z';
}

int GameManager::getCoins()
{
	return this->coinsCollected;
}

float GameManager::getGameElapsed()
{
	return gameElapsed;
}

