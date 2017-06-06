#pragma once

#include "Component.h"

class FallComponent : public Component
{
	float speed;
public:
	FallComponent();
	FallComponent(float speedVar);
	~FallComponent();

	virtual void update(float elapsedTime) override;
};