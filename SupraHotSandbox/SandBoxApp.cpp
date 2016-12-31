#include "Platform.h"
#include <FileSystem.h>
#include "SandBoxApp.h"
#include "MeshComponent.h"

#ifdef PLATFORM_ANDROID 
	#include "WindowAndroid.h"
	#include <android/asset_manager.h>
	#include <android/asset_manager_jni.h>
#endif

#ifdef PLATFORM_WINDOWS
	#include "WindowWin32.h"
	#include <Controls.h>
#endif

#ifdef PLATFORM_EMSCRIPTEN
	#include "WindowEmscripten.h"
#endif

#include <MeshDataLoader.h>
#include <LuaVM.h>
#include <TextureCube.h>
#include <SkyBox.h>
#include <MeshDataRenderer.h>
#include <ShaderLibrary.h>
#include <TextureCache.h>
#include <TextureCubeMaterialProperty.h>
#include <Texture2DMaterialProperty.h>
#include <EntityManager.h>

using namespace SupraHot;

#define SPONZA 1

SandBoxApp::SandBoxApp()
{
}

SandBoxApp::~SandBoxApp()
{
}

void SandBoxApp:: Init(SupraHot::uint32 width, SupraHot::uint32 height, std::string title)
{
	// First we need to initialize the filesystem
#ifdef PLATFORM_WINDOWS
	Utils::FileSystem::GetInstance()->SetRootPath("../Content/");
#endif

#ifdef PLATFORM_EMSCRIPTEN
	Utils::FileSystem::GetInstance()->SetRootPath("Content/");
#endif

	window = new Window();
	window->Init(width, height, title);
	window->SetClearColor(0.7f, 0.3f, 0.7f, 1.0f);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Was excuted before the new window call
	App::Init(width, height, title);

	FBO = new FrameBufferObject();
	FBO->Init(width, height);

	Texture = new Texture2D("SHF Logo");
	Texture->Load("Images/logo.png");
	FBO->SetReadSource(Texture);	
	FBO->SetPixelSize(ShaderLibrary::GetInstance()->ScreenSpace[uint32(ShaderLibrary::ScreenSpace::RenderToScreen)]);

	TextureCache::GetInstance()->Init();

	// Try loading a lua script and run it.
	Scripting::LuaVM::GetInstance()->RunFile("Scripts/test.lua");

	{
		std::vector<MeshComponent*> meshComponents = Utils::MeshDataLoader::GetInstance()->Load(
#if SPONZA == 1
			"Models/Sponza/Sponza_M.shfm"
#elif 
			"Models/ParisApartment/ParisApartment.shfm"
#endif
			);
		for (MeshComponent* meshComponent : meshComponents)
		{
			Entity* entity = new Entity();
			entity->GetTransform().SetScale(
				Vec3(
#if SPONZA == 1
				0.02f, 0.02f, 0.02f
#elif 
				0.05f, 0.05f, 0.05f
#endif
				)
				);

#if SPONZA == 0
			entity->GetTransform().SetLocalRotation(Quat4(Vec3(0, 0, 1), 90) * Quat4(Vec3(0, 1, 0), 90));
#endif

			entity->AddComponent(meshComponent);
			EntityManager::GetInstance()->AddEntity(entity);

			entity->SetName(meshComponent->GetMeshData()->Name);
		}
	}

	// Try to load a 2d .dds file
	Texture2D* ddsTexture = new Texture2D("DDS Test");
	ddsTexture->SetWrapS(GL_CLAMP_TO_EDGE);
	ddsTexture->SetWrapT(GL_CLAMP_TO_EDGE);
	ddsTexture->Load("Textures/test/lion_128f.dds");
	//ddsTexture->Destroy();
	delete ddsTexture;
	
	TextureCube* textureCube = new TextureCube("CubeTexture Test");
	textureCube->SetWrapS(GL_CLAMP_TO_EDGE);
	textureCube->SetWrapT(GL_CLAMP_TO_EDGE);
	textureCube->SetWrapR(GL_CLAMP_TO_EDGE);
	textureCube->Load("Textures/Bridge2/px.png", "Textures/Bridge2/nx.png",
					  "Textures/Bridge2/py.png", "Textures/Bridge2/ny.png",
					  "Textures/Bridge2/pz.png", "Textures/Bridge2/nz.png");
	//textureCube->Destroy();
	delete textureCube;

	DdsCubeTexture = new TextureCube("DDS Cube map");
	DdsCubeTexture->SetWrapS(GL_CLAMP_TO_EDGE);
	DdsCubeTexture->SetWrapT(GL_CLAMP_TO_EDGE);
	DdsCubeTexture->SetWrapR(GL_CLAMP_TO_EDGE);
	DdsCubeTexture->LoadDDS("Textures/MonValley_G_DirtRoad_3k/Diffuse.dds", true, true);

	Texture2D* sphereMap = new Texture2D("sphere map");
	sphereMap->SetWrapS(GL_CLAMP_TO_EDGE);
	sphereMap->SetWrapT(GL_CLAMP_TO_EDGE);
	sphereMap->Load("Textures/MonValley_G_DirtRoad_3k/Static.dds");
	//sphereMap->Destroy();
	delete sphereMap;

	EnvBox = new SkyBox(); 
	EnvBox->SetEnvironmentMap(DdsCubeTexture);
	//EnvBox->SetEnvironmentMap(sphereMap);
	EnvBox->Init(); 
	
	FlyCamera = new Camera(50.0f, 0.05f, 100.0f, static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()));
	
	
	{	// Test smart pointer
		Texture2D* testTex2d = new Texture2D("testTex2d 0");
		testTex2d->SetWrapS(GL_CLAMP_TO_EDGE);
		testTex2d->SetWrapT(GL_CLAMP_TO_EDGE);
		testTex2d->Load("Textures/MonValley_G_DirtRoad_3k/Static.dds");

		Texture2D* testTex2d1 = new Texture2D("testTex2d 1");
		testTex2d1->SetWrapS(GL_CLAMP_TO_EDGE);
		testTex2d1->SetWrapT(GL_CLAMP_TO_EDGE);
		testTex2d1->Load("Textures/MonValley_G_DirtRoad_3k/Static.dds");
		
		Texture2DPtr ptr(testTex2d);

		SHF_PRINTF("ptr count = %d. \n", ptr.use_count());

		SHF_PRINTF("Assign new texture to pointer. \n");

		ptr = Texture2DPtr(testTex2d1);

		SHF_PRINTF("ptr count = %d. \n", ptr.use_count());
	}

	// Test texture cache
	{
		SHF_PRINTF("-------------- \n");
		SHF_PRINTF("Test texture cache \n");
		Texture2D* rawTexture = new Texture2D("RAW Texture2D");
		rawTexture->Load("Textures/Bridge2/px.png");

		Texture2DPtr smartTexture(rawTexture);

		TextureCache::GetInstance()->CacheTexture(smartTexture);
		TextureCache::GetInstance()->FreeTexture(smartTexture);

		// delete rawTexture;

		SHF_PRINTF("-------------- \n"); 
	}


	{	// Load a test model
		std::vector<MeshComponent*> meshComponents = Utils::MeshDataLoader::GetInstance()->Load("Models/Pistol/Pistol_Model.shfm");

		Entity* entity = new Entity();

		entity->GetTransform().SetPosition(Vec3(0, 1.0f, -5.0f));
		entity->GetTransform().SetScale(Vec3(0.05f, 0.05f, 0.05f));
		entity->GetTransform().SetLocalRotation(Quat4(Vec3(0, 0, 1), 90) * Quat4(Vec3(0, 1, 0), 90));

		MeshComponent* meshComponent = meshComponents.at(0);
		entity->AddComponent(meshComponent);


		auto envMap = meshComponent->GetMaterial()->GetMaterialPropertyByName("EnvMap");
		if (envMap != nullptr)
		{
			TextureCubeMaterialProperty* mp = reinterpret_cast<TextureCubeMaterialProperty*>(envMap);
			mp->SetValue("Textures/MonValley_G_DirtRoad_3k/Diffuse.dds");
			printf("type : %s \n", envMap->GetType().c_str());
		} 
		else
		{
			TextureCubeMaterialProperty* mp = new TextureCubeMaterialProperty("EnvMap");
			mp->SetValue("Textures/MonValley_G_DirtRoad_3k/Diffuse.dds");
			meshComponent->GetMaterial()->AddMaterialProperty(mp);
		}

		
		auto albedo = meshComponent->GetMaterial()->GetMaterialPropertyByName("DiffuseTexture");
		if (albedo != nullptr)
		{
			Texture2DMaterialProperty* mp = reinterpret_cast<Texture2DMaterialProperty*>(albedo);
			mp->SetValue("Models/Pistol/albedo.png");
			printf("type : %s \n", albedo->GetType().c_str());
		}
		else
		{
			Texture2DMaterialProperty* mp = new Texture2DMaterialProperty("DiffuseTexture");
			mp->SetValue("Models/Pistol/albedo.png");
			meshComponent->GetMaterial()->AddMaterialProperty(mp);
		}

		meshComponent->UpdateShaderPermution();

	/*	if (meshComponent->GetMaterial()->GetMaterialPropertyByName("EnvMap"))
		{
			meshComponent->GetMaterial()->RemoveMaterialProperty(meshComponent->GetMaterial()->GetMaterialPropertyByName("EnvMap"));
			meshComponent->UpdateShaderPermution();
		}*/

		EntityManager::GetInstance()->AddEntity(entity);
	}

}

void SandBoxApp::Resize(SupraHot::uint32 width, SupraHot::uint32 height)
{
#if PLATFORM_ANDROID
	window->Resize(width, height);
#endif

	FBO->Resize(width, height);
	FlyCamera->AspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void SandBoxApp::Render()
{
	FBO->Attach();

	EnvBox->Render(FlyCamera, ShaderLibrary::GetInstance()->Skybox[uint32(ShaderLibrary::SkyboxShader::CubeMap)]);

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

void SandBoxApp::Update(float deltaTime)
{
	window->SetClearColor(0.7f, 0.3f, 0.7f, 1.0f);

	FlyCamera->ResetMatrices();
	FlyCamera->Update(deltaTime);
	
#if PLATFORM_WINDOWS
	Controls::GetInstance()->Update(window);

	if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetForward().Normalized() * deltaTime * FlyCamera->MoveSpeed;
	}
	else if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetBack().Normalized() * deltaTime * FlyCamera->MoveSpeed;
	}

	if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetLeft().Normalized() * deltaTime * FlyCamera->MoveSpeed;
	}
	else if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetRight().Normalized() * deltaTime * FlyCamera->MoveSpeed;
	}

	if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_SPACE))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetUp().Normalized() * deltaTime * FlyCamera->MoveSpeed;
	}
	else if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetDown().Normalized() * deltaTime * FlyCamera->MoveSpeed;
	}
#endif

	std::vector<Entity*>* entities = EntityManager::GetInstance()->GetEntities();

	for (uint32 i = 0, l = static_cast<uint32>(entities->size()); i < l; ++i)
	{
		entities->at(i)->Update(deltaTime);
	}
}

void SandBoxApp::LateUpdate(float deltaTime)
{
	std::vector<Entity*>* entities = EntityManager::GetInstance()->GetEntities();

	for (uint32 i = 0, l = static_cast<uint32>(entities->size()); i < l; ++i)
	{
		entities->at(i)->LateUpdate(deltaTime);
	}
}

void SandBoxApp::Tick(float deltaTime)
{
	std::vector<Entity*>* entities = EntityManager::GetInstance()->GetEntities();

	for (uint32 i = 0, l = static_cast<uint32>(entities->size()); i < l; ++i)
	{
		entities->at(i)->FixedUpdate(deltaTime);
	}

	SHF_PRINTF("FPS: %d \n", FPS);
}

void SandBoxApp::Destroy()
{
	EntityManager::GetInstance()->DestroyAndDelete();

	TextureCache::GetInstance()->Destroy();

	App::Destroy();

	Texture->Destroy();
	DdsCubeTexture->Destroy();
	FBO->Destroy();
	window->Destroy();

	delete Texture;
	delete DdsCubeTexture;
	delete FBO;
	delete window;
	delete FlyCamera;
}