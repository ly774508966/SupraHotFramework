#include "AppEditor.h"
#include <cliext/algorithm>

namespace SupraHot
{
	namespace Graphics
	{
		void AppEditor::Init(WindowEditor^ window)
		{
			Window = window;
			GetHandle()->Init(400, 400, "Editor");
		}

		void AppEditor::Resize(uint32 width, uint32 height)
		{
			Instance->Resize(width, height);
			Window->GetHandle()->Resize(width, height);
		}

		void AppEditor::Update(float deltaTime)
		{
			Instance->Update(deltaTime);
		}

		void AppEditor::LateUpdate(float deltaTime)
		{
			Instance->LateUpdate(deltaTime);
		}

		void AppEditor::Tick(float deltaTime)
		{
			Instance->Tick(deltaTime);
		}

		void AppEditor::Render()
		{
			Instance->Render();
		}

		void AppEditor::Destroy()
		{
			Instance->Destroy();
		}

		void AppEditor::ProgressApp()
		{
			glViewport(0, 0, Window->GetHandle()->GetWidth(), Window->GetHandle()->GetHeight());

			Window->GetHandle()->Clear();

			Update(0.0f);
			LateUpdate(0.0f);
			Tick(0.0f);
			Render();

			Window->GetHandle()->Update();
		}

		void AppEditor::AddEntity(SupraHot::CLI::EntityCLI^ entity)
		{
			Instance->AddEntity(entity->GetHandle());
			Entities.push_back(entity);
		}

		void AppEditor::RemoveEntity(SupraHot::CLI::EntityCLI^ entity)
		{
			Instance->RemoveEntity(entity->GetHandle());
			auto index = cliext::remove(Entities.begin(), Entities.end(), entity);
			Entities.erase(index, Entities.end());
		}

		cliext::vector<SupraHot::CLI::EntityCLI^> AppEditor::GetEntities()
		{
			return Entities;
		}
	};
};