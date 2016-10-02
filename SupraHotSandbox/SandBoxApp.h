#pragma once
#include <App.h>
#include <FrameBufferObject.h>

class SandBoxApp : public SupraHot::App
{
private:
	SupraHot::FrameBufferObject* fbo;
	SupraHot::Texture2D* texture;
public:
	SandBoxApp();
	~SandBoxApp();

	void Init(SupraHot::uint32 width, SupraHot::uint32 height, std::string title) override;
	void Resize(SupraHot::uint32 width, SupraHot::uint32 height) override;
	void Render() override;
	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
	void Tick(float deltaTime) override;
	void Destroy() override;
};

