#pragma once
#include "Wrapper.h"
#include "AppEditorImpl.h"
#include "WindowEditor.h"
#include "EntityCLI.h"
#include <cliext/vector>

namespace SupraHot
{
	namespace Graphics
	{
		public ref class AppEditor : public Wrapper<AppEditorImpl>
		{
		private:
			WindowEditor^ Window;
			cliext::vector<SupraHot::CLI::EntityCLI^> Entities;
		public:
			void Init(WindowEditor^ window);
			void Resize(uint32 width, uint32 height);
			void Update(float deltaTime);
			void LateUpdate(float deltaTime);
			void Tick(float deltaTime);
			void Render();
			void Destroy();
			void ProgressApp();

			void AddEntity(SupraHot::CLI::EntityCLI^ entity);
			void RemoveEntity(SupraHot::CLI::EntityCLI^ entity);
			cliext::vector<SupraHot::CLI::EntityCLI^> GetEntities();
		};
	};
};