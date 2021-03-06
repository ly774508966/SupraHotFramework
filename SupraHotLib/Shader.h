#pragma once
#include "Platform.h"
#include "ShaderCompileOptions.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat4.h"
#include "Utility.h"

namespace SupraHot
{
	using namespace Math;
	namespace Graphics
	{
		class TextureCube;
		class Texture2D;

		class Shader
		{
		public:
			// This BRDF-Types dictate how the given material should be rendererd
			// and through which post-processing passses it needs to go.
			enum class BRDFType
			{
				None  = 0x0,
				GGX   = BITSHIFT(0),
				Skin  = BITSHIFT(1),
				Cloth = BITSHIFT(2),
				Hair  = BITSHIFT(3),
				Eye   = BITSHIFT(4)
			};
		private:
			uint32 VertexShader = 0, 
				   PixelShader = 0, 
				   GeometryShader = 0, 
				   ComputeShader = 0;

			uint32 ShaderProgrammID = 0;
			uint64 ShaderPermutationIndex = 0;
			uint64 UUID = 0;

			BRDFType BRDF = BRDFType::None;

#if DEVELOPMENT == 1
			ShaderCompileOptions Options;
			std::string ShaderSource;
#endif

			static void CheckError(uint32 shaderID, std::string type, std::string path);
			static void LoadShaderInternal(std::string path, std::string &destination, ShaderCompileOptions& compileOptions);

			void GenerateUUID();
		protected:
			std::string Name = "";
			
		public:
			uint32 LastUsedTextureSlot = 0;

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

			uint64 GetShaderPermutationIndex();
			void SetShaderPermutationIndex(uint64 shaderIndex);

			void SetBRDF(BRDFType brdf);
			BRDFType GetBRDF();

			uint64 GetUUID();

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
			void SetVec4(int location, const Vec4& v);
			void SetMat3(int location, const Mat4& m);
			void SetMat4(int location, const Mat4& m);
			void SetTexture2D(int location, Texture2D* texture2d, int slot);
			void SetTextureCube(int location, TextureCube* textureCube, int slot);
			uint32 GetUniformLocation(std::string name);

#if DEVELOPMENT == 1
			void Print();
#endif

			// GL specific
			uint32 GetShaderID();
		};
	};
};