#include "AppEditor.h"
#include <EntityManager.h>
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
			Window->GetHandle()->SetClearColor(0.3f, 0.3f, 0.3f, 0);

			glViewport(0, 0, Window->GetHandle()->GetWidth(), Window->GetHandle()->GetHeight());

			Window->GetHandle()->Clear();

			Update(0.0f);
			LateUpdate(0.0f);
			Tick(0.0f);
			Render();

			Window->GetHandle()->Update();

			//PostRender();
		}

		/*void AppEditor::PostRender()
		{
			EntityManager::GetInstance()->PostRender();
		}*/

		SupraHot::CLI::CameraCLI^ AppEditor::GetCamera()
		{
			
			if (CameraInstance == nullptr)
			{
				CameraInstance = gcnew SupraHot::CLI::CameraCLI();
				CameraInstance->SetInstance(Instance->GetCamera());
			}


			return CameraInstance;
		}
	};
};