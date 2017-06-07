#pragma once

#include "Component.h"


class LowerComponent : public Component
{
	float speed;
public:

	LowerComponent();
	LowerComponent(float speed);
	~LowerComponent();

	virtual void update(float elapsedTime) override;
};
