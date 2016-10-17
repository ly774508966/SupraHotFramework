#include "Material.h"

namespace SupraHot
{
	namespace Graphics
	{
		Material::Material()
		{
		}

		Material::~Material()
		{
		}

		Texture2D* Material::GetAlbedoMap()
		{
			return AlbedoMap;
		}

		Texture2D* Material::GetNormalMap()
		{
			return NormalMap;
		}

		Texture2D* Material::GetRoughnessMap()
		{
			return RoughnessMap;
		}

		Texture2D* Material::GetMetalnessMap()
		{
			return MetalnessMap;
		}

		Texture2D* Material::GetSpecularMap()
		{
			return SpecularMap;
		}

		Texture2D* Material::GetComboMap()
		{
			return ComboMap;
		}

		void Material::SetAlbedoMap(Texture2D* tex)
		{
			this->AlbedoMap = tex;
		}

		void Material::SetNormalMap(Texture2D* tex)
		{
			this->NormalMap = tex;
		}

		void Material::SetRoughnessMap(Texture2D* tex)
		{
			this->RoughnessMap = tex;
		}

		void Material::SetMetalnessMap(Texture2D* tex)
		{
			this->MetalnessMap = tex;
		}

		void Material::SetSpecularMap(Texture2D* tex)
		{
			this->SpecularMap = tex;
		}

		void Material::SetName(std::string name)
		{
			this->Name = name;
		}

		void Material::SetKd(Vec3 v)
		{
			this->Kd = v;
		}

		void Material::SetKa(Vec3 v)
		{
			this->Ka = v;
		}

		void Material::SetKs(Vec3 v)
		{
			this->Ks = v;
		}

		void Material::SetKe(Vec3 v)
		{
			this->Ke = v;
		}

		Vec3 Material::GetKd()
		{
			return Kd;
		}

		Vec3 Material::GetKa()
		{
			return Ka;
		}

		Vec3 Material::GetKs()
		{
			return Ks;
		}

		Vec3 Material::GetKe()
		{
			return Ke;
		}

		void Material::SetNs(float ns)
		{
			this->Ns = ns;
		}

		float Material::GetNs()
		{
			return Ns;
		}

		void Material::SetRoughness(float r)
		{
			this->Roughness = r;
		}

		void Material::SetMetalness(float m)
		{
			this->Metalness = m;
		}

		void Material::SetF0(float f0)
		{
			this->F0 = f0;
		}

		float Material::GetRoughness()
		{
			return Roughness;
		}

		float Material::GetMetalness()
		{
			return Metalness;
		}

		float Material::GetF0()
		{
			return F0;
		}

		void Material::Destroy()
		{
			if (AlbedoMap != nullptr && AlbedoMap->GetID() > 0)
			{
				AlbedoMap->Destroy();
			}

			if (NormalMap != nullptr && NormalMap->GetID() > 0)
			{
				NormalMap->Destroy();
			}

			if (RoughnessMap != nullptr && RoughnessMap->GetID() > 0)
			{
				RoughnessMap->Destroy();
			}

			if (MetalnessMap != nullptr && MetalnessMap->GetID() > 0)
			{
				MetalnessMap->Destroy();
			}

			if (SpecularMap != nullptr && SpecularMap->GetID() > 0)
			{
				SpecularMap->Destroy();
			}

			if (ComboMap != nullptr && ComboMap->GetID() > 0)
			{
				ComboMap->Destroy();
			}
		}

		std::string Material::GetName()
		{
			return Name;
		}

		void Material::SetComboMap(Texture2D* tex)
		{
			this->ComboMap = tex;
		}
	};
};