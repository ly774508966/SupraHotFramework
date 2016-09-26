// Example app.
// Shows how to use the lib.


#include "stdafx.h"

#ifdef PLATFORM_ANDROID
#include "WindowAndroid.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "WindowWin32.h"
#include "stdafx.h"
#include <tchar.h>
#endif

#include "Publisher.h"
#include "FrameBufferObject.h"
#include "Shader.h"
#include "MeshDataLoader.h"
#include "App.h"
#include <string>

using namespace SupraHot;


// App class

class DemoApp : public App
{
private:
	FrameBufferObject* fbo;
	Texture2D* testTexture;
	Shader* fboShader;

	// Publisher test
	PubSub::Publisher RandomPublisher;
	struct PubSubData
	{
		int x = 5;
		int y = 10;
	};

	class SubTester : public PubSub::Subscriber
	{
	public:
		bool OnMessage(std::string message, void* data) override
		{
			printf("I GOT MESSAGED BRO\nWHAT DO FAM?!\n%s\n", message.c_str());

			PubSubData* dat = static_cast<PubSubData*>(data);
			printf("data.x = %d, data.y = %d \n", dat->x, dat->y);
			return false;
		};
	};

	SubTester* subTester;
	PubSubData* dat;

public:

	DemoApp()
	{
		App::App();
	}

	~DemoApp() override
	{
	};

	void Resize(uint32 width, uint32 height) override
	{
		fbo->Resize(width, height);
	}

	void Init(uint32 width, uint32 height, std::string title) override
	{
		App::Init(width, height, title);

		window = new WindowWin32(this);
		window->Init(width, height, title);

		fbo = new FrameBufferObject();
		fbo->Init(width, height);

		testTexture = new Texture2D("Test Texture");
		testTexture->Load("../Content/Images/test.png");
		fbo->SetReadSource(testTexture);
		printf("testTexture id %d\n", testTexture->GetID());

		std::vector<MeshData*> meshData = Utils::MeshDataLoader::GetInstance()->Load("../Content/Models/cube.mh", Utils::MeshDataLoader::ModelFileFormat::OBJX);
		printf("MeshData Facecount: %d \n", meshData.at(0)->FaceCount);
		printf("Material Name %s \n", meshData.at(0)->MeshMaterial.GetName().c_str());
		printf("Material Albedo Tex ID %d \n", meshData.at(0)->MeshMaterial.GetAlbedoMap()->GetID());

		// Load Shaders
		fboShader = new Shader();
		fboShader->LoadShaderFromFile(Shader::VERTEX_SHADER, "../Content/Shaders/fbo.vs");
		fboShader->LoadShaderFromFile(Shader::PIXEL_SHADER, "../Content/Shaders/fbo.fs");
		fboShader->CompileShader();

		// Subtester
		subTester = new SubTester();
		RandomPublisher.AddSubscriber(subTester, "SubTesterNews");

		dat = new PubSubData();
		RandomPublisher.Publish("SubTesterNews", static_cast<void*>(dat));
	};

	void Render() override
	{
		fbo->Attach();
		fbo->Detach();
		fbo->RenderToScreen(fboShader);
	};

	void Update(float deltaTime) override
	{
		// TODO: Wire this up in the Run() Method
	};

	void LateUpdate(float deltaTime) override
	{
		// TODO: Wire this up in the Run() Method
	};

	void Tick(float deltaTime) override
	{
		// TODO: Wire this up in the Run() Method
	};

	void Destroy() override
	{
		testTexture->Destroy();
		fboShader->Destroy();
		fbo->Destroy();
		window->Destroy();
	};
};

int _tmain(int argc, _TCHAR* argv[])
{
	DemoApp app;
	app.Init(640, 480, "Demo Application");
	app.Run();
	app.Destroy();
	return 0;
}


