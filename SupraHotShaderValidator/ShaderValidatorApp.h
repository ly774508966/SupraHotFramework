#pragma once
#include "Platform.h"
#include "App.h"

namespace SupraHot
{
	class ShaderValidatorApp : public SupraHot::App
	{
	private:
		FILETIME ShaderDescriptionDate = {};
		FILETIME VertexShaderDate = {};
		FILETIME PixelShaderDate = {};

		std::string ShaderDescriptionFileName;
		std::string VertexShaderFilePath;
		std::string PixelShaderFilePath;

		void Recompile();
		void CheckJson();
		void CheckVertexShader();
		void CheckPixelShader();

		FILETIME CheckFileTime(std::string pathToFile);

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