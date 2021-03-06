#pragma once

#include <vector>

#include "SceneBase.h"
#include "Camera.h"
#include "GameObjectManager.h"

enum ABILITY_TYPE
{
	ABILITY_GRAPPLER,
	ABILITY_DASH,
	ABILITY_PORTAL,
	ABILITY_RECALL,
	ABILITY_SLOWTIME,
	ABILITY_BLACKHOLE,

	ABILITY_COUNT
};

class Ability
{

public:
	Ability(char buttonChar, ABILITY_TYPE type, double abilityDurationCooldown, Mesh* mesh = nullptr);
	~Ability();

	virtual void Init() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Reset() = 0;
	virtual ABILITY_TYPE GetAbilityType() = 0;

	void SetCamera(Camera* camera);
	void SetGOManager(GameObjectManager* GOManager);
	void SetScenePointer(SceneBase* scene);
	void SetPlayer(GameObject* player);

	void SetAbilityCooldownDuration(double cooldown);
	void ResetAbilityCooldown();

	double GetCooldownLeft();
	double GetCooldownDuration();
	double GetCooldownTillReadyPercentage();

	Mesh* GetMeshIcon();

	void SetActivatingKey(char key);
	char GetActivatingKey();

protected:

	double abilityCD_timeleft;
	double abilityCooldownDuration;

	int usesLeft;
	int maxUses;

	Mesh* icon;
	Camera* camera;
	GameObjectManager* goManager;
	SceneBase* scene; //Used for custom rendering of abilities
	GameObject* player;
	ABILITY_TYPE type;
	char buttonChar;
};

