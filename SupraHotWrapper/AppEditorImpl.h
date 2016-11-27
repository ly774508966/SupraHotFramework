#pragma once
#include <App.h>
#include <Entity.h>
#include <vector>
#include <FrameBufferObject.h>
#include <Camera.h>

namespace SupraHot
{
	namespace Graphics
	{
		class AppEditorImpl : public App
		{
		private:
			SupraHot::FrameBufferObject* FBO;
			std::vector<Entity*> Entities;
			SupraHot::Camera* FlyCamera;
		public:
			AppEditorImpl();
			~AppEditorImpl();

			void Init(SupraHot::uint32 width, SupraHot::uint32 height, std::string title) override;
			void Resize(SupraHot::uint32 width, SupraHot::uint32 height) override;
			void Render() override;
			void Update(float deltaTime) override;
			void LateUpdate(float deltaTime) override;
			void Tick(float deltaTime) override;
			void Destroy() override;

			void AddEntity(Entity* entity);
			void RemoveEntity(Entity* entity);
		};
	};
};
