#include "AppEditorImpl.h"
#include <FileSystem.h>
#include <MeshDataRenderer.h>
#include <ShaderLibrary.h>
#include <TextureCache.h>
#include <algorithm>
#include <EntityManager.h>
#include <MeshDataCache.h>
#include <MaterialCache.h>
#include <Publisher.h>

namespace SupraHot
{
	namespace Graphics
	{
		AppEditorImpl::AppEditorImpl(){}
		AppEditorImpl::~AppEditorImpl(){}

		void AppEditorImpl::Init(SupraHot::uint32 width, SupraHot::uint32 height, std::string title)
		{
			// Applies only, when running from VS
			Utils::FileSystem::GetInstance()->SetRootPath("../../../../Content/");

			ShaderLibrary::GetInstance()->Initialize();

			TextureCache::GetInstance()->Init();

			MaterialCache::GetInstance()->Init();

			FBO = new FrameBufferObject();
			FBO->Init(width, height);

			FlyCamera = new Camera(50.0f, 0.05f, 10000.0f, static_cast<float>(width) / static_cast<float>(height));

			SHF_PRINTF("AppEditorImpl:: FBO->SetClearColor \n");
			FBO->SetClearColor(0.8f, 0.8f, 0.8f);

			MeshDataRenderer::GetInstance().Initialize(FlyCamera, width, height);
		}

		void AppEditorImpl::Resize(SupraHot::uint32 width, SupraHot::uint32 height)
		{
			FBO->Resize(width, height);
			FlyCamera->AspectRatio = static_cast<float>(width) / static_cast<float>(height);
			MeshDataRenderer::GetInstance().Resize(width, height);

			Vec2* resizeInformation = new Vec2(static_cast<float>(width), static_cast<float>(height));
			PubSub::Publisher::GetSystemPublisher().Publish("AppResize", resizeInformation);
			delete resizeInformation;
		}

		void AppEditorImpl::Render()
		{
			FBO->Attach();

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_BLEND); 
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			 
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glCullFace(GL_BACK);
			 			 
			// MeshDataRenderer::GetInstance().ExecuteRenderCommandQueue();
			MeshDataRenderer::GetInstance().RenderMain(FlyCamera);
			 
			glDisable(GL_DEPTH_TEST);

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
			FlyCamera->ResetMatrices();
			FlyCamera->Update(deltaTime);
			EntityManager::GetInstance()->Update(deltaTime);
		}

		void AppEditorImpl::LateUpdate(float deltaTime)
		{
			EntityManager::GetInstance()->LateUpdate(deltaTime);
		}

		void AppEditorImpl::Tick(float deltaTime)
		{
			EntityManager::GetInstance()->FixedUpdate(deltaTime);
		}

		void AppEditorImpl::Destroy()
		{
			EntityManager::GetInstance()->Destroy();

			MaterialCache::GetInstance()->Destroy();

			TextureCache::GetInstance()->Destroy();

			MeshDataCache::GetInstance()->Destroy();

			FBO->Destroy();

			App::Destroy();
		}

		void AppEditorImpl::AddEntity(Entity* entity)
		{
			EntityManager::GetInstance()->AddEntity(entity);
		}

		void AppEditorImpl::RemoveEntity(Entity* entity)
		{
			EntityManager::GetInstance()->RemoveEntity(entity);
		}

		SupraHot::Camera* AppEditorImpl::GetCamera()
		{
			return FlyCamera;
		}
	};
};