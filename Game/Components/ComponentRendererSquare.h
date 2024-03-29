#pragma once

#include "Engine/Component.h"

class ComponentRendererSquare : public MyEngine::Component  {
public:
	void Init(rapidjson::Value& serializedData) override;
	void Init(int texture_id);
	void Init(std::string texture_id);
	void Render(sre::RenderPass& renderPass) override;
	int GetId() { return _id; }
	void SetRotation(bool rotate_x, bool rotate_y, int rotation);
	void SetScaleX(float scale);
	bool is_floor = false;
private:
	int _id;
	std::shared_ptr<sre::Mesh> _mesh;
	std::shared_ptr<sre::Material> _material;
	std::shared_ptr<sre::Texture> _texture;
	bool _rotate_x;
	bool _rotate_y;
	int _rotation;
	float _scale_x = 1;
};
