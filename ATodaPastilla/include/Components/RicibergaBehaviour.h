#pragma once
#include "Entity\Component.h"

class RicibergaBehaviour :public Component
{
public:
	RicibergaBehaviour(json& j);
	virtual ~RicibergaBehaviour() {};
	void init(json& j);
	void start() override;
	void update() override;
	void OnCollision(Entity* other) override;
	virtual bool ReceiveEvent(Event& event) override;

private:
	float dyingTime;
	float timeToDie=0;
	float damage;
	float speed;
	float bottom;
	bool seeksPlayer;
	bool sinusoidalMovement;

	float sinusoidalMagnitude;
	float sinusoidalFrequency;

	Entity* player=nullptr;

	std::string hitSound;
	std::string deathSound;
};
	

