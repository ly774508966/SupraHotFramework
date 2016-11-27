#include "AppEditor.h"

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

		void AppEditor::AddEntity(SupraHot::CLI::Entity^ entity)
		{
			Instance->AddEntity(entity->GetHandle());
		}

		void AppEditor::RemoveEntity(SupraHot::CLI::Entity^ entity)
		{
			Instance->RemoveEntity(entity->GetHandle());
		}
	};
};