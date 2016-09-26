#include "Shader.h"
#include "FileReader.h"
#include "Texture2D.h"
#include <iostream>

namespace SupraHot
{
	namespace Graphics
	{
		void Shader::CheckError(uint32 shaderID, std::string type, std::string path)
		{
			char error[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, error);
#if DEVELOPMENT == 1
			printf("CheckError for %s, Type = %s \n", path.c_str(), type.c_str());
			std::cout << error << std::endl;
#endif
		}

		void Shader::LoadShaderInternal(std::string path, std::string& destination)
		{
			std::vector<std::string> fileContent = Utils::FileReader::GetInstance()->ReadFile(path);

			for (unsigned int i = 0; i < fileContent.size(); i++)
			{
				destination += fileContent[i] + "\n";
			}
		}

		
		std::string Shader::GetName()
		{
			return Name;
		}

		void Shader::SetName(std::string name)
		{
		}

		void Shader::Attach()
		{
			glUseProgram(ShaderProgrammID);
		}

		void Shader::Destroy()
		{
		/*	glDetachShader(ShaderProgrammID, VertexShader);
			glDetachShader(ShaderProgrammID, PixelShader);
			glDetachShader(ShaderProgrammID, GeometryShader);
			glDetachShader(ShaderProgrammID, ComputeShader);*/

			glDeleteProgram(ShaderProgrammID);

			ShaderProgrammID = 0;
			VertexShader = 0;
			PixelShader = 0;
			GeometryShader = 0;
			ComputeShader = 0;

			printf("ShaderGL %s #%d destroyed \n", Name.c_str(), ShaderProgrammID);
		}

		bool Shader::LoadShaderFromFile(ShaderType type, std::string pathToFile)
		{
			unsigned int shaderC = 0;
			std::string shaderType = "UNKNOWN_SHADER_TYPE";

			if (type == VERTEX_SHADER)
			{
				shaderC = glCreateShader(GL_VERTEX_SHADER);
				VertexShader = shaderC;
				shaderType = "VERTEX_SHADER";
			}
			else if (type == PIXEL_SHADER)
			{
				shaderC = glCreateShader(GL_FRAGMENT_SHADER);
				PixelShader = shaderC;
				shaderType = "PIXEL_SHADER";
			}
			else if (type == GEOMETRY_SHADER)
			{
#ifdef PLATFORM_WINDOWS
				shaderC = glCreateShader(GL_GEOMETRY_SHADER);
				GeometryShader = shaderC;
				shaderType = "GEOMETRY_SHADER";
#endif
			}
			else if (type == GL_COMPUTE_SHADER)
			{
				shaderC = glCreateShader(GL_COMPUTE_SHADER);
				ComputeShader = shaderC;
				shaderType = "GL_COMPUTE_SHADER";
			}

			std::string shaderSource;
			LoadShaderInternal(pathToFile, shaderSource);

			const char* shadersource = shaderSource.c_str();
			glShaderSource(shaderC, 1, &shadersource, nullptr);
			glCompileShader(shaderC);
			CheckError(shaderC, shaderType, pathToFile);

			glAttachShader(ShaderProgrammID, shaderC);

			return true;
		}

		bool Shader::CompileShader()
		{
			/* link to shader program */
			glLinkProgram(ShaderProgrammID);
			glUseProgram(ShaderProgrammID);

			/* check errors */
			int err = glGetError();
			if (err != 0)
			{
#if DEVELOPMENT == 1
				printf("Error %d happened while creating the shaderProgramm \n", err);
#endif
				return false;
			}


			if (VertexShader > 0)
			{
				glDeleteShader(VertexShader);
			}

			if (PixelShader > 0)
			{
				glDeleteShader(PixelShader);
			}

			if (GeometryShader > 0)
			{
				glDeleteShader(GeometryShader);
			}

			if (ComputeShader > 0)
			{
				glDeleteShader(ComputeShader);
			}

			return true;
		}

		void Shader::SetInteger(int location, int value)
		{
			glUniform1i(location, value);
		}

		void Shader::SetFloat(int location, float value)
		{
			glUniform1f(location, value);
		}

		void Shader::SetVec2(int location, const Vec2& v)
		{
			glUniform2f(location, v.x, v.y);
		}

		void Shader::SetVec3(int location, const Vec3& v)
		{
			glUniform3f(location, v.x, v.y, v.z);
		}

		void Shader::SetVec4(int location, const Vec3& v, float w)
		{
			glUniform4f(location, v.x, v.y, v.z, w);
		}

		void Shader::SetMat3(int location, const Mat4& mat)
		{
			float mat3[3][3];
			mat3[0][0] = mat.m[0][0];	mat3[1][0] = mat.m[1][0];	mat3[2][0] = mat.m[2][0];
			mat3[0][1] = mat.m[0][1];	mat3[1][1] = mat.m[1][1];	mat3[2][1] = mat.m[2][1];
			mat3[0][2] = mat.m[0][2];	mat3[1][2] = mat.m[1][2];	mat3[2][2] = mat.m[2][2];

			glUniformMatrix3fv(location, 1, GL_FALSE, &(mat3[0][0]));
		}

		void Shader::SetMat4(int location, const Mat4& m)
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, &(m.m[0][0]));
		}

		void Shader::SetTexture2D(int location, Texture2D* texture2d, int slot)
		{
			glActiveTexture(slot);
			glBindTexture(GL_TEXTURE_2D, texture2d->GetID());
			SetInteger(location, (slot - GL_TEXTURE0));
		}

		uint32 Shader::GetShaderID()
		{
			return ShaderProgrammID;
		}


		Shader::Shader()
		{
			ShaderProgrammID = glCreateProgram();
		}

		Shader::~Shader()
		{
		}
	};
};
