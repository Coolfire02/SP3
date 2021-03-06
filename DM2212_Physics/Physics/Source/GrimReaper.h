#ifndef GRIMREAPER_H
#define GRIMREAPER_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"
#include "Tombstone.h"

class GrimReaper : public Enemy {
public:
	GrimReaper();
	~GrimReaper();

	void Init(SceneBase* scene, GameObjectManager* GOM, Vector3& target); // keep target only if you need to access player pos
	void Update(double dt);
	bool Interact();
	void CollidedWith(GameObject* go);
	
private:
	enum ENEMY_STATE {
		INACTIVE, // default state, not moving

		CHASING,
		PREWHACKING,
		WHACKING,
		COOLDOWN,

		ESTATE_TOTAL
	};

	float idle_speed;
	float hostile_speed;

	float activeRange;
	float whackRange;
	
	ENEMY_STATE state;
	double attack_timer;
	double cooldown_timer;

	Vector3 *playerPos; // to keep track of player pos
	
	SpriteAnimation* animatedSprites;
	GameObjectManager* goManager;
	

};


#endif