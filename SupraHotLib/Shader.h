#pragma once
#include "Platform.h"
#include "ShaderCompileOptions.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Mat4.h"

namespace SupraHot
{
	using namespace Math;
	namespace Graphics
	{
		class TextureCube;
		class Texture2D;

		class Shader
		{
		private:
			uint32 VertexShader = 0, 
				   PixelShader = 0, 
				   GeometryShader = 0, 
				   ComputeShader = 0;

			uint32 ShaderProgrammID = 0;

			static void CheckError(uint32 shaderID, std::string type, std::string path);
			static void LoadShaderInternal(std::string path, std::string &destination, ShaderCompileOptions& compileOptions);

		protected:
			std::string Name;

		public:

			// Shader enum
			enum ShaderType 
			{
				VERTEX_SHADER = 0,
				PIXEL_SHADER,
				GEOMETRY_SHADER,
				COMPUTE_SHADER,
			};

			Shader();
			~Shader();

			std::string GetName();
			void SetName(std::string name);

			void Attach();
			void Detach();
			void Destroy();
			bool LoadShaderFromFile(ShaderType type, std::string pathToFile, ShaderCompileOptions compileOptions = {});
			bool CompileShader();

			void SetInteger(int location, int value);
			void SetFloat(int location, float value);
			void SetVec2(int location, const Vec2& v);
			void SetVec3(int location, const Vec3& v);
			void SetVec4(int location, const Vec3& v, float w);
			void SetMat3(int location, const Mat4& m);
			void SetMat4(int location, const Mat4& m);
			void SetTexture2D(int location, Texture2D* texture2d, int slot);
			void SetTextureCube(int location, TextureCube* texture2d, int slot);
			uint32 GetUniformLocation(std::string name);

			// GL specific
			uint32 GetShaderID();
		};
	};
};