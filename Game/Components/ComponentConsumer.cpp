#include "ComponentConsumer.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "glm/gtx/transform.hpp"
#include "ComponentItem.h"
#include "Engine/MyEngine.h"
#include "ComponentRendererSquare.h"
#include "ComponentGameLoop.h"
#include "ComponentNotifier.h"

void ComponentConsumer::Update(float delta) {
	if (is_ordering) patience_left -= delta;
	if (indicator) {
		auto r = indicator->FindComponent<ComponentRendererSquare>().lock();
		r->SetScaleX(patience_left / total_patience);
	}
}

void ComponentConsumer::Init(std::string id) {
	_id = id;
}

void ComponentConsumer::CreateOrder(std::string item_id, float patience) {
	auto go = GetGameObject().lock();
	std::cout << "Placing order " << go->GetName() << std::endl;
	is_ordering = true;
	patience_left = patience;
	total_patience = patience;
	_id = item_id;

	auto pos = go->GetPosition();
	CreateConsumerIndicator(item_id, pos.x, pos.y);
}

void ComponentConsumer::Interact() {
	auto heldVal = "itm-held";
	auto engine = MyEngine::Engine::GetInstance();
	auto notifier = engine->GetGameObject("notifier").lock()->FindComponent<ComponentNotifier>().lock();
	auto isHeld = engine->GameObjectExists(heldVal);

	if (!is_ordering) {
		std::cout << "This table is not awaiting food" << std::endl;
		notifier->SetMessage("This table is not awaiting food");
		return;
	}

	if (isHeld) {
		auto held = engine->GetGameObject(heldVal).lock();
		auto item = held->FindComponent<ComponentItem>().lock();
		auto item_id = item->GetTypeId();
		if (GeneralId() == item_id) {
			auto gl = engine->GetGameObject("game_loop").lock()->FindComponent<ComponentGameLoop>().lock();
			gl->OrderCompleted();

			engine->DeleteGameObject(heldVal);
			indicator = nullptr;
			auto pos = GetGameObject().lock()->GetPosition();
			auto indicator_name = "indicator-" + std::to_string((int)pos.x) + "-" + std::to_string((int)pos.y);
			engine->DeleteGameObject(indicator_name);

			is_ordering = false;
		} else {
			notifier->SetMessage("That customer did not order that food");
			std::cout << "consumer and item are different types" << std::endl;
		}
	}
}

void ComponentConsumer::CreateConsumerIndicator(std::string id, int x, int y) {
	auto engine = MyEngine::Engine::GetInstance();
	auto name = "indicator-" + std::to_string(x) + "-" + std::to_string(y);
	auto indicator_go = engine->CreateGameObject(name).lock();
	indicator = indicator_go;

	auto indicator_renderer = std::make_shared<ComponentRendererSquare>();
	indicator_renderer->Init(id);
	indicator_renderer->SetRotation(false, true, 0);
	indicator_go->AddComponent(indicator_renderer);

	auto pos = glm::vec3(x, y, 2);
	auto rot = glm::vec3(0, 0, 0);
	auto scl = glm::vec3(0.5, 0.5, 0.5);

	indicator_go->transform =
		glm::translate(pos) *
		glm::mat4_cast(glm::quat(glm::radians(rot))) *
		glm::scale(scl);
}