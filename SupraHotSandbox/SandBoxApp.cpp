#include "Platform.h"
#include "SandBoxApp.h"

#include <FileSystem.h>
#include <MeshComponent.h>
#include <MeshDataLoader.h>
#include <LuaVM.h>
#include <TextureCube.h>
#include <SkyBox.h>
#include <MeshDataRenderer.h>
#include <ShaderLibrary.h>
#include <TextureCache.h>
#include <EntityManager.h>
#include <GenericSerializer.h>

#ifdef PLATFORM_ANDROID 
#include <WindowAndroid.h>
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
#include <GenericSerializer.h>
#include <MeshDataCache.h>
#include <MaterialCache.h>
#include <Material.h>
#include <Octree.h>
#include <MurmurHash.h>
#include <Sorting.h>
#include <Publisher.h>

using namespace SupraHot;

#define SPONZA 1

SandBoxApp::SandBoxApp()
{
}

SandBoxApp::~SandBoxApp()
{
}
 
void SandBoxApp::Init(SupraHot::uint32 width, SupraHot::uint32 height, std::string title)
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

#ifdef PLATFORM_WINDOWS
	glfwSwapInterval(0);
#endif
	
	// Intel: 73 - 83
	// Nvidia: ~ 450 - 460

	// with culling and sorting
	// Intel: 87 - 92, 94, 87
	// Nvidia: 500 - ~545-555 || 600 - 750

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

	MaterialCache::GetInstance()->Init();

	TextureCache::GetInstance()->Init();

	// Try loading a lua script and run it.
	Scripting::LuaVM::GetInstance()->RunFile("Scripts/test.lua");

	

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

	TextureCube* DdsCubeTexture = new TextureCube("DDS Cube map");
	DdsCubeTexture->SetWrapS(GL_CLAMP_TO_EDGE);
	DdsCubeTexture->SetWrapT(GL_CLAMP_TO_EDGE);
	DdsCubeTexture->SetWrapR(GL_CLAMP_TO_EDGE);
	DdsCubeTexture->LoadDDS("Textures/MonValley_G_DirtRoad_3k/Diffuse.dds", true, true);

	TextureCubePtr ddsCubePtr = TextureCache::GetInstance()->WrapTexture(DdsCubeTexture);
	TextureCache::GetInstance()->CacheTexture(ddsCubePtr);

	Texture2D* sphereMap = new Texture2D("sphere map");
	sphereMap->SetWrapS(GL_CLAMP_TO_EDGE);
	sphereMap->SetWrapT(GL_CLAMP_TO_EDGE);
	sphereMap->Load("Textures/MonValley_G_DirtRoad_3k/Static.dds");
	//sphereMap->Destroy();
	delete sphereMap;

	EnvBox = new SkyBox();
	EnvBox->SetEnvironmentMap(ddsCubePtr);
	//EnvBox->SetEnvironmentMap(sphereMap);
	EnvBox->Init();

	FlyCamera = new Camera(50.0f, 0.05f, 1000.0f, static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()));
	MeshDataRenderer::GetInstance().Initialize(FlyCamera, window->GetWidth(), window->GetHeight());

	{
		std::string path;
#if SPONZA == 1
		path = "Models/Sponza/Sponza_M.shfm";
#else 
		path = "Models/ParisApartment/ParisApartment.shfm";
#endif

		std::vector<MeshDataPtr>* cachedMeshes = Utils::MeshDataLoader::GetInstance()->Load(path);

		sponza = new Entity();
		sponza->SetName("Sponza Root");

		for (uint32 m = 0, l = static_cast<uint32>(cachedMeshes->size()); m < l; ++m)
		{
			MeshDataPtr meshData = cachedMeshes->at(m);

			Material* material = new Material(MaterialCache::GetInstance()->GetMeshDefaultMaterial());

			MeshComponent* meshComponent = new MeshComponent(meshData, material, path, m);

			Entity* entity = new Entity();

#if SPONZA == 0
			entity->GetTransform().SetLocalRotation(Quat4(Vec3(0, 0, 1), 90) * Quat4(Vec3(0, 1, 0), 90));
#endif
			sponza->AddChild(entity);
			entity->AddComponent(meshComponent);
			entity->SetName(meshComponent->GetMeshData()->Name);
		}

		EntityManager::GetInstance()->AddEntity(sponza);
		sponza->GetTransform().SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));

	}


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
		std::vector<MeshDataPtr>* cachedMeshes = Utils::MeshDataLoader::GetInstance()->Load("Models/Pistol/Pistol_Model.shfm");

		for (uint32 m = 0, l = static_cast<uint32>(cachedMeshes->size()); m < l; ++m)
		{
			MeshDataPtr meshData = cachedMeshes->at(m);

			MaterialCache::GetInstance()->LoadIntoCache("Materials/xXXxx.json");
			Material* material = new Material(MaterialCache::GetInstance()->GetCached("Materials/xXXxx.json"));

			MeshComponent* meshComponent = new MeshComponent(meshData, material, "Models/Pistol/Pistol_Model.shfm", m);

			Entity* entity = new Entity();

			entity->AddComponent(meshComponent);

			entity->GetTransform().SetLocalPosition(Vec3(0.0f, 0.75f, -7.0f));
			entity->GetTransform().SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
			entity->GetTransform().SetRotation(Quat4(Vec3(0, 0, 1), 90) * Quat4(Vec3(0, 1, 0), 90));

			meshComponent->UpdateShaderPermution();

			EntityManager::GetInstance()->AddEntity(entity);
			SHF_PRINTF("PISTOL ENTITY END \n");
		}




		{	// Load a test model
			std::vector<MeshDataPtr>* cachedMeshes2 = Utils::MeshDataLoader::GetInstance()->Load("Models/Pistol/Pistol_Model.shfm");

			for (uint32 m = 0, l = static_cast<uint32>(cachedMeshes2->size()); m < l; ++m)
			{
				MeshDataPtr meshData = cachedMeshes2->at(m);

				MaterialCache::GetInstance()->LoadIntoCache("Materials/YYYYY.json");
				Material* material = new Material(MaterialCache::GetInstance()->GetCached("Materials/YYYYY.json"));

				MeshComponent* meshComponent = new MeshComponent(meshData, material, "Models/Pistol/Pistol_Model.shfm", m);

				Entity* entity2 = new Entity();
				entity2->SetName("2nd pistol");
				entity2->AddComponent(meshComponent);
				entity2->GetTransform().SetLocalPosition(Vec3(0.0f, 0.75f, -7.0f));
				entity2->GetTransform().SetLocalScale(Vec3(0.05f, 0.05f, 0.05f));
				entity2->GetTransform().SetRotation(Quat4(Vec3(0, 0, 1), 90) * Quat4(Vec3(0, 1, 0), 90));

				meshComponent->UpdateShaderPermution();
				EntityManager::GetInstance()->AddEntity(entity2);
				SHF_PRINTF("PISTOL ENTITY END \n");
			}
		}

	}


	// EulerAngles -> Quat4

	Vec3 eulerAnglesInput(90.0f * DEGREES_TO_RADIANS, 66.0f * DEGREES_TO_RADIANS, 33.0f * DEGREES_TO_RADIANS);

	printf("input: \n");
	eulerAnglesInput.print();

	Quat4 q;
	q.FromEulerAngles(eulerAnglesInput);
	printf("Q = [%f, %f, %f, %f] \n", q.v.x, q.v.y, q.v.z, q.w);

	// Test Quat4 -> EulerAngles

	Vec3 eulerAnglesOutput = q.ToEulerAngles() * RADIANS_TO_DEGREES;
	printf("ouput: \n");
	eulerAnglesOutput.print();
	
	// Test octree
	{
		Octree<Vec3> octree;
		//octree.GetRoot();

	}

	// Test murmur hash
	{
		std::string stringToHash = "Wurstbein";
		Utils::MurmurHash::Hash codeHash = Utils::MurmurHash::GenerateHash_32(stringToHash.data(), int(stringToHash.length()), 0);
		printf("Hashed value = %s \n", codeHash.ToString().c_str());
		printf("Hashed value = %llu \n", codeHash.A);
		printf("Hashed value = %llu \n", codeHash.B);
		//return cacheDir + codeHash.ToString() + L".cache";
	}
}

void SandBoxApp::Resize(SupraHot::uint32 width, SupraHot::uint32 height)
{
#if PLATFORM_ANDROID
	window->Resize(width, height);
#endif

	FBO->Resize(width, height);
	FlyCamera->AspectRatio = static_cast<float>(width) / static_cast<float>(height);

	MeshDataRenderer::GetInstance().Resize(width, height);

	Vec2* resizeInformation = new Vec2(static_cast<float>(width), static_cast<float>(height));
	PubSub::Publisher::GetSystemPublisher().Publish("AppResize", resizeInformation);
	delete resizeInformation;
}

void SandBoxApp::Render()
{
	FBO->Attach();

	EnvBox->Render(FlyCamera, ShaderLibrary::GetInstance()->Skybox[uint32(ShaderLibrary::SkyboxShader::CubeMap)]);

	MeshDataRenderer::GetInstance().ExecuteRenderCommandQueue();

	FBO->Detach();
	FBO->SetReadSource(FBO->GetColorRenderTarget());

	FBO->RenderToScreen(
		ShaderLibrary::GetInstance()->ScreenSpace
		[
			uint32(ShaderLibrary::ScreenSpace::RenderToScreen)
		]
	);
}

float angle = 0.0f;
void SandBoxApp::Update(float deltaTime)
{
	Entity* pistol = EntityManager::GetInstance()->GetEntities()->at(1);

	MeshComponent* meshComponent = static_cast<MeshComponent*>(pistol->GetComponent("MeshComponent"));
	Vec3 min = meshComponent->GetMeshData()->BoundingBox.GetMinimum();
	Vec3 max = meshComponent->GetMeshData()->BoundingBox.GetMaximum();

	Quat4 q;
	q.FromEulerAngles(Vec3(-90.0f * DEGREES_TO_RADIANS, angle * DEGREES_TO_RADIANS, 0.0f));
	pistol->GetTransform().SetRotation(q);

	angle++;
	if (angle > 360)
	{
		angle = 0;
	}

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

	EntityManager::GetInstance()->Update(deltaTime);
}

void SandBoxApp::LateUpdate(float deltaTime)
{
	EntityManager::GetInstance()->LateUpdate(deltaTime);
}

void SandBoxApp::Tick(float deltaTime)
{
	EntityManager::GetInstance()->FixedUpdate(deltaTime);
	SHF_PRINTF("FPS: %d \n", FPS);
}

void SandBoxApp::Destroy()
{
	EnvBox->Destroy();
	delete EnvBox;

	EntityManager::GetInstance()->DestroyAndDelete();

	MaterialCache::GetInstance()->Destroy();

	TextureCache::GetInstance()->Destroy();

	MeshDataCache::GetInstance()->Destroy();

	App::Destroy();

	Texture->Destroy();
	FBO->Destroy();
	window->Destroy();

	delete Texture;
	delete FBO;
	delete window;
	delete FlyCamera;

	SHF_PRINTF("TODO: EntityManager & Caches should init'd and destroyed inside the App-Interface \n");
}