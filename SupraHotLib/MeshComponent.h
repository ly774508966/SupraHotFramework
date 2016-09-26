#pragma once
#include "Component.h"
#include "Platform.h"

namespace SupraHot
{
	class MeshComponent : public Component
	{
	public:
		std::string Identifier = "MeshComponent";

		MeshComponent();
		~MeshComponent();

		void Registered() override;
		void Destroy() override;
		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
	};
};