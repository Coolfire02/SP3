#ifndef SCENE_CHURCH_H
#define SCENE_CHURCH_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Item.h"
#include "Inventory.h"
#include "Player.h"
#include "Input.h"
#include "GameObjectManager.h"

class SceneChurch : public SceneBase
{
public:
	SceneChurch();
	~SceneChurch();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	virtual void InitLights();

protected:

	float m_speed;

	Inventory* inventory;
	Input* input;
	GameObjectManager* goManager;
	Player* player;


	double gridLength, gridHeight;
	void CursorToWorldPosition(double& theX, double& theY);
};

#endif