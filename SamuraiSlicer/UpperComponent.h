#pragma once

#include "Component.h"


class UpperComponent : public Component
{
	float speed;
public:

	UpperComponent();
	UpperComponent(float speed);
	~UpperComponent();

	virtual void update(float elapsedTime) override;
};
