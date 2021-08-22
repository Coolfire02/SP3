#ifndef SCENE_PHYSICS_H
#define SCENE_PHYSICS_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "Item.h"
#include "Inventory.h"
#include "Apple.h"
#include "Cheese.h"
#include "Keyboard.h"
#include "GameObjectManager.h"

class ScenePhysics : public SceneBase
{
public:
	ScenePhysics();
	~ScenePhysics();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

protected:

	float m_speed;
	float dashTimer;
	bool isDashing = false;
	bool isGrappling = false;

	float m_screenWidth;
	float m_screenHeight;
	float m_worldWidth;
	float m_worldHeight;

	Vector3 temp;
	Vector3 temp2;
	Vector3 displacement2;
	GameObject* plr;
	GameObject* grappler;
	Inventory* inventory;
	Keyboard* keyboard;
	GameObjectManager* goManager;

	double gridLength, gridHeight;

	GameObject* FetchGO(bool isMovable);
	bool CheckCollision(GameObject* go1, GameObject* go2, float dt); // Checks for collision
	void ApplyFriction(GameObject* ball, Vector3 normal, double dt); // Apply Friction
	void ApplyInelastic(GameObject* ball, Vector3 normal, double dt); //Decrease vel of object
	void CollisionResponse(GameObject* go1, GameObject* go2, float dt);
};

#endif