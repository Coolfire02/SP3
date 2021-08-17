
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue, SHAPE_TYPE shapeType)
	: type(typeValue),
	pos(1, 1, 1),
	scale(1, 1, 1),
	active(false),
	fireInterval(0),
	maxHP(0), currentHP(0), timeout(0)
{
	physics = new Physics(shapeType, pos, scale);
}

GameObject::GameObject(GAMEOBJECT_TYPE typeValue, Mesh* mesh, SHAPE_TYPE shapeType)
	: type(typeValue),
	pos(1, 1, 1),
	scale(1, 1, 1),
	active(false),
	fireInterval(0),
	maxHP(0),
	currentHP(0),
	timeout(0),
	physics(new Physics(shapeType, pos, scale)),
	mesh(mesh)
{
}

GameObject::~GameObject()
{
	delete physics;
	physics = NULL;
}

void GameObject::Init()
{
}

void GameObject::Update()
{
}

GameObject* GameObject::Clone()
{
	GameObject* go = new GameObject();
	go->type = this->type;
	go->active = this->active;
	go->currentHP = this->currentHP;
	go->fireInterval = this->fireInterval;
	go->mat = this->mat;
	go->maxHP = this->maxHP;
	go->mesh = this->mesh;
	go->physics = this->physics->Clone();
	go->pos = this->pos;
	go->scale = this->scale;
	go->timeout = this->timeout;
	return go;
}

