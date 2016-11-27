#pragma once
#include "Wrapper.h"
#include "AppEditorImpl.h"
#include "WindowEditor.h"
#include "EntityCLI.h"

namespace SupraHot
{
	namespace Graphics
	{
		public ref class AppEditor : public Wrapper<AppEditorImpl>
		{
		private:
			WindowEditor^ Window;
		public:
			void Init(WindowEditor^ window);
			void Resize(uint32 width, uint32 height);
			void Update(float deltaTime);
			void LateUpdate(float deltaTime);
			void Tick(float deltaTime);
			void Render();
			void Destroy();
			void ProgressApp();

			void AddEntity(SupraHot::CLI::Entity^ entity);
			void RemoveEntity(SupraHot::CLI::Entity^ entity);
		};
	};
};