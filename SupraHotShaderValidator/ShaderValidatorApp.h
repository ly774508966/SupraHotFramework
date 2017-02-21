#pragma once
#include "Platform.h"
#include "App.h"

namespace SupraHot
{
	class ShaderValidatorApp : public SupraHot::App
	{
	private:
		std::string ShaderDescriptionFileName;
	public:
		ShaderValidatorApp();
		~ShaderValidatorApp();

		void Init(uint32 width, uint32 height, std::string title) override;
		void Resize(uint32 width, uint32 height) override;
		void Render() override;
		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void Tick(float deltaTime) override;
		void Destroy() override;
		void SetFileListener(std::string shaderDescriptionFileName);
	};
};