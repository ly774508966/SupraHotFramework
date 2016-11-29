#include "AppEditorImpl.h"
#include <FileSystem.h>
#include <MeshDataRenderer.h>
#include <ShaderLibrary.h>
#include <algorithm>

namespace SupraHot
{
	namespace Graphics
	{
		AppEditorImpl::AppEditorImpl(){}
		AppEditorImpl::~AppEditorImpl(){}

		void AppEditorImpl::Init(SupraHot::uint32 width, SupraHot::uint32 height, std::string title)
		{
			Utils::FileSystem::GetInstance()->SetRootPath("../../../../Content/");

			ShaderLibrary::GetInstance()->Initialize();

			FBO = new FrameBufferObject();
			FBO->Init(width, height);

			FlyCamera = new Camera(50.0f, 0.05f, 100.0f, static_cast<float>(height) / static_cast<float>(height));
		}

		void AppEditorImpl::Resize(SupraHot::uint32 width, SupraHot::uint32 height)
		{
			FBO->Resize(width, height);
			FlyCamera->AspectRatio = static_cast<float>(width) / static_cast<float>(height);
		}

		void AppEditorImpl::Render()
		{
			FBO->Attach();

			MeshDataRenderer::GetInstance().Render(FlyCamera);

			FBO->Detach();
			FBO->SetReadSource(FBO->GetColorRenderTarget());

			FBO->RenderToScreen(
				ShaderLibrary::GetInstance()->ScreenSpace
				[
					uint32(ShaderLibrary::ScreenSpace::RenderToScreen)
				]
			);
		}

		void AppEditorImpl::Update(float deltaTime)
		{
			for (uint32 i = 0, l = static_cast<uint32>(Entities.size()); i < l; ++i)
			{
				Entities.at(i)->Update(deltaTime);
			}
		}

		void AppEditorImpl::LateUpdate(float deltaTime)
		{
			for (uint32 i = 0, l = static_cast<uint32>(Entities.size()); i < l; ++i)
			{
				Entities.at(i)->LateUpdate(deltaTime);
			}
		}

		void AppEditorImpl::Tick(float deltaTime)
		{
			for (uint32 i = 0, l = static_cast<uint32>(Entities.size()); i < l; ++i)
			{
				Entities.at(i)->FixedUpdate(deltaTime);
			}
		}

		void AppEditorImpl::Destroy()
		{
			for (uint32 i = 0, l = static_cast<uint32>(Entities.size()); i < l; ++i)
			{
				Entities.at(i)->Destroy();
				
			} Entities.clear();

			FBO->Destroy();

			App::Destroy();
		}

		void AppEditorImpl::AddEntity(Entity* entity)
		{
			Entities.push_back(entity);
		}

		void AppEditorImpl::RemoveEntity(Entity* entity)
		{
			Entities.erase(std::remove(Entities.begin(), Entities.end(), entity), Entities.end());
		}

	};
};