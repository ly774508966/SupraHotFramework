#pragma once
#include "Platform.h"
#include "Vec3.h"
#include "Texture2D.h"

namespace SupraHot
{
	using namespace Math;
	namespace Graphics
	{
		class Material
		{
		private:
			Texture2D* AlbedoMap;
			Texture2D* NormalMap;
			Texture2D* RoughnessMap;
			Texture2D* MetalnessMap;
			Texture2D* SpecularMap;
			Texture2D* ComboMap;

		public:
			std::string Name;
			float Ns = 0;				// specular exponent
			float D = 0;				// dissolve factor
			float Ni = 0;				// optical density
			int Illum = 0;				// illumination model 
			Vec3 Ka;					// ambient reflectivity
			Vec3 Kd;					// diffuse reflectivity
			Vec3 Ks;					// specular reflectivity
			float Roughness = 0.0f;     // default roughness
			float Metalness = 0.0f;     // defaul metalness
			float F0 = 0.04f;           // Fresnel0 0.0f = base reflectivity
			Vec3 Color;					// Albedo color

			Material();
			~Material();

			Texture2D* GetAlbedoMap();
			Texture2D* GetNormalMap();
			Texture2D* GetRoughnessMap();
			Texture2D* GetMetalnessMap();
			Texture2D* GetSpecularMap();
			Texture2D* GetComboMap();

			void SetAlbedoMap(Texture2D* tex);
			void SetNormalMap(Texture2D* tex);
			void SetRoughnessMap(Texture2D* tex);
			void SetMetalnessMap(Texture2D* tex);
			void SetSpecularMap(Texture2D* tex);
			void SetComboMap(Texture2D* tex);

			void SetName(std::string name);
			std::string GetName();

			void SetKd(Vec3 v);
			void SetKa(Vec3 v);
			void SetKs(Vec3 v);
			void SetColor(Vec3 v);

			Vec3 GetKd();
			Vec3 GetKa();
			Vec3 GetKs();
			Vec3 GetColor();

			void SetNs(float ns);
			void SetNi(float ni);
			void SetD(float d);
			void SetIllum(unsigned int illum);

			float GetNs();
			float GetNi();
			float GetD();
			unsigned int GetIllum();

			void SetRoughness(float r);
			void SetMetalness(float m);
			void SetF0(float f0);

			float GetRoughness();
			float GetMetalness();
			float GetF0();

			void Destroy();
		};
	};
};

