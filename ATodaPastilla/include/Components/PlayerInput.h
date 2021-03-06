#pragma once

#include "Entity\Component.h"

class PlayerInput : public Component {
public:
	PlayerInput(json& args);
	~PlayerInput();
	void alwaysLateUpdate() override;
	void init(json& j) override;
};

