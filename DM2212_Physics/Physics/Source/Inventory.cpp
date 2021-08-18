/**
 Inventory Class
 By: Jordan Low
 Date: 17 Aug 2021
 */
#include "Inventory.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"

/**
 @brief Init Inventory
 */
void Inventory::Init()
{
	keyboard = Keyboard::GetInstance();
}

/**
 @brief Update Inventory
 @param dt A double for delta time
 */
void Inventory::Update(double dt)
{
	if (currentItem)
		currentItem->Update(dt);


	if (keyboard->IsKeyPressed(VK_LEFT))
	{
		std::cout << "cycle backward" << std::endl;
		CycleItem(false);
		std::cout << "qty: " << currentItem->GetQuantity() << std::endl;
	}
	if (keyboard->IsKeyPressed(VK_RIGHT))
	{
		std::cout << "cycle forward" << std::endl;
		CycleItem(true);
		std::cout << "qty: " << currentItem->GetQuantity() << std::endl;
	}
	if (Application::IsKeyReleased('5'))
	{
		std::cout << "1" << std::endl;
		SwitchItem(0);
	}
	if (Application::IsKeyReleased('6'))
	{
		std::cout << "2" << std::endl;
		SwitchItem(1);
	}
}

/**
 @brief Cycle through current item with item vector
 */
void Inventory::CycleItem(bool forward)
{
	int currentItemIndex = -1;
	
	//search for the current item's index in the vector
	auto it = std::find(itemVector.begin(), itemVector.end(), currentItem);
	if (it != itemVector.end())
		currentItemIndex = std::distance(itemVector.begin(), it);

	if (forward) //cycle forward
	{
		//if successfully found curr item index
		if (currentItemIndex != -1)
		{
			//if curr item index is the last element in vector
			if (currentItemIndex == itemVector.size() - 1)
				currentItemIndex = -1;
			currentItem = itemVector[currentItemIndex + 1];
			std::cout << "curr: " << currentItem->GetType() << std::endl;
		}
	}
	else //cycle backwards
	{
		//if successfully found curr item index
		if (currentItemIndex != -1)
		{
			//if curr item index is the last element in vector
			if (currentItemIndex == 0)
				currentItemIndex = itemVector.size();
			currentItem = itemVector[currentItemIndex - 1];
			std::cout << "curr: " << currentItem->GetType() << std::endl;
		}
	}
}

void Inventory::SwitchItem(int index)
{
	if (index >= itemVector.size())
		return;
	currentItem = itemVector[index];
}

/**
 @brief Add new item to inventory
 @param newItem An Item* which gets added into the inventory
 */
void Inventory::AddItem(Item* newItem)
{
	if (itemVector.size() <= 0)
	{
		itemVector.push_back(newItem);
		currentItem = newItem;
		std::cout << currentItem->GetType() << std::endl;
		return;
	}

	bool itemExist = false;
	bool isStackable = false;

	for (Item* item : itemVector)
	{
		if (newItem->GetType() == item->GetType())
		{
			if (newItem->GetIsStackable() && item->IsEqual(newItem))
			{
				std::cout << "adding new qty to item: " << item->GetType() << std::endl;
				item->AddQuantity(1);
				return;
			}
		}
	}

	std::cout << "adding new item to vector" << std::endl;
	itemVector.push_back(newItem);
	currentItem = newItem;
}
