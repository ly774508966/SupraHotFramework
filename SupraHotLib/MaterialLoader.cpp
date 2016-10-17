#include "MaterialLoader.h"
#include "FileReader.h"
#include "StringUtil.h"

namespace SupraHot
{
	namespace Utils
	{
		MaterialLoader* MaterialLoader::GetInstance()
		{
			static MaterialLoader* instance(new MaterialLoader);
			return instance;
		}

		MaterialLoader::MaterialLoader()
		{
		}

		MaterialLoader::~MaterialLoader()
		{
		}

		std::unordered_map<std::string, Graphics::Material> MaterialLoader::LoadMTLFile(std::string path)
		{
#if DEVELOPMENT == 1
			printf("Loading MTL File: %s \n", path.c_str());
#endif

			std::unordered_map<std::string, Graphics::Material> materials;

			Graphics::Material tempMaterial;

			/* control boolean, to detect the first mesh, that needs to be inserted */
			bool gotMaterial = false;

			std::vector<std::string> fileContent = FileReader::GetInstance()->ReadFile(path);;

			/* interpret material data from string vector */
			for (size_t i = 0, l = fileContent.size(); i < l; ++i)
			{
				std::string line = fileContent.at(i);

				if (line.find("new") == 0)
				{
					if (gotMaterial)
					{
						materials.insert(std::make_pair(tempMaterial.GetName(), tempMaterial));
						tempMaterial = Graphics::Material();
					}
					else
					{
						gotMaterial = true;
					}

					std::string tempName = StringUtil::Split(line, "newmtl").at(1);
					tempName = StringUtil::trim(tempName);
					tempMaterial.SetName(tempName.c_str());
				}
				else if (line.find("Kd") == 0)
				{
					float x, y, z;
					SHF_SSCANF(line.c_str(), "Kd %f %f %f", &x, &y, &z);
					tempMaterial.SetKd(Vec3(x, y, z));
				}
				else if (line.find("Ka") == 0)
				{
					float x, y, z;
					SHF_SSCANF(line.c_str(), "Ka %f %f %f", &x, &y, &z);
					tempMaterial.SetKa(Vec3(x, y, z));
				}
				else if (line.find("Ks") == 0)
				{
					float x, y, z;
					SHF_SSCANF(line.c_str(), "Ks %f %f %f", &x, &y, &z);
					tempMaterial.SetKs(Vec3(x, y, z));
				}
				else if (line.find("Ns") == 0)
				{
					float ns;
					SHF_SSCANF(line.c_str(), "Ns %f", &ns);
					tempMaterial.SetNs(ns);
				}
				else if (line.find("Ni") == 0)
				{
					float ni;
					SHF_SSCANF(line.c_str(), "Ni %f", &ni);
					//tempMaterial.SetNi(ni);
				}
				else if (line.find("illum") == 0)
				{
					unsigned int illum;
					SHF_SSCANF(line.c_str(), "illum %u", &illum);
					//tempMaterial.SetIllum(illum);
				}
				else if (line.find("d ") == 0)
				{
					float d;
					SHF_SSCANF(line.c_str(), "d %f", &d);
				//	tempMaterial.SetD(d);
				}
				else if (line.find("map_") == 0)
				{
					std::string subString = StringUtil::Split(line, "map_").at(1);

#if DEVELOPMENT == 1
					printf("Loading Texture: %s for Material: %s \n", subString.c_str(), tempMaterial.GetName().c_str());
#endif

					if (subString.find("Kd") == 0)
					{
						//load albedomap
						subString = StringUtil::Split(subString, "Kd ").at(1);
						subString = StringUtil::trim(subString);
						Graphics::Texture2D* tex = new Graphics::Texture2D(GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						tex->Load(subString.c_str());
						tempMaterial.SetAlbedoMap(tex);
					}
					else if (subString.find("Ks ") == 0)
					{
						// load specularmap
						subString = StringUtil::Split(subString, "Ks ").at(1);
						subString = StringUtil::trim(subString);
						Graphics::Texture2D* tex = new Graphics::Texture2D(GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						tex->Load(subString.c_str());
						tempMaterial.SetSpecularMap(tex);
					}
					else if (subString.find("Ns ") == 0)
					{
						// load normalmap
						subString = StringUtil::Split(subString, "Ns ").at(1);
						subString = StringUtil::trim(subString);
						Graphics::Texture2D* tex = new Graphics::Texture2D(GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						tex->Load(subString.c_str());
						tempMaterial.SetNormalMap(tex);
					}
					else if (subString.find("Ro ") == 0)
					{
						// load roughnessmap
						subString = StringUtil::Split(subString, "Ro ").at(1);
						subString = StringUtil::trim(subString);
						Graphics::Texture2D* tex = new Graphics::Texture2D(GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						tex->Load(subString.c_str());
						tempMaterial.SetRoughnessMap(tex);
					}
					else if (subString.find("Me ") == 0)
					{
						// load metallnessmap
						subString = StringUtil::Split(subString, "Me ").at(1);
						subString = StringUtil::trim(subString);
						Graphics::Texture2D* tex = new Graphics::Texture2D(GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						tex->Load(subString.c_str());
						tempMaterial.SetMetalnessMap(tex);
					}
				}
			}

			/* insert last mesh on 'stack' into the map */
			if (gotMaterial)
			{
				materials.insert(std::make_pair(tempMaterial.GetName(), tempMaterial));
			}

			return materials;
		}
	};
};