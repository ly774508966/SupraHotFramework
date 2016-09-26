#pragma once
#include "Platform.h"
#include "Component.h"
#include "Subscriber.h"

namespace SupraHot
{
	class SubscriberComponent : public Component, PubSub::Subscriber
	{
	public:
		SubscriberComponent();
		~SubscriberComponent();
		void Registered() override;
		void Destroy() override;
		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
		bool OnMessage(std::string message, void* data) override;
	};
};
