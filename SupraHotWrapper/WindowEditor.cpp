#include "WindowEditor.h"

namespace SupraHot
{
	namespace Graphics
	{
		void WindowEditor::SetHWND(void* hwnd)
		{
			Instance->SetHWND(static_cast<HWND>(hwnd));
		}

		void WindowEditor::Init(uint32 width, uint32 height)
		{
			Instance->Init(width, height, "EditorWindow");
		}

		void WindowEditor::Update()
		{
			Instance->Update();
		}

		void WindowEditor::Clear()
		{
			Instance->Clear();
		}

		void WindowEditor::SetClearColor(float r, float g, float b, float a)
		{
			Instance->SetClearColor(r, g, b, a);
		}
	};
};
