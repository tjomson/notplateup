#pragma once

#include "Engine/Component.h"
#include "Engine/Components/ComponentPhysicsBody.h"
#include "ComponentGameLoop.h"


class ComponentController : public MyEngine::Component {
	void Init(rapidjson::Value&) override;
	void Update(float deltaTime) override;
	void KeyEvent(SDL_Event&) override;

public:
    float GetAngle();

private:
	void MoveTable();
	void Interact();
	float mov_speed;
    glm::vec3 direction_vector = {0,0,0};
    float direction_angle;
	std::weak_ptr<ComponentPhysicsBody> _body;

	bool key_down_w = false;
	bool key_down_a = false;
	bool key_down_s = false;
	bool key_down_d = false;
	bool key_down_q = false;
	bool key_down_e = false;

	bool key_down_k = false;
	bool key_down_o = false;
	bool key_down_shift = false;
	float dash_cooldown = 0;
	float dash_timer = 0;
	std::shared_ptr<ComponentGameLoop> _game_loop;
};