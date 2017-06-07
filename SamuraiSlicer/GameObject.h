#pragma once

#include <list>
#include "Vec.h"

class Component;
class DrawComponent;



class GameObject
{
	DrawComponent* drawComponent = nullptr;
	
	std::list<Component*> components;
public:
	GameObject();
	~GameObject();


	Vec3f position;
	Vec3f rotation;
	Vec3f scale = Vec3f(1,1,1);
	bool collision = true;
	int count = 0;
	int index = 0;
	


	void addComponent(Component* component);
	void update(float elapsedTime);
	void draw();
};

