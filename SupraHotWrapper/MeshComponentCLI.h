#pragma once
#include "Wrapper.h"
#include "MeshComponentCLIImpl.h"
#include "MeshDataCLI.h"
#include "MaterialCLI.h"
#include "ComponentCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		public ref class MeshComponent : public Wrapper<MeshComponentCLIImpl>, public SupraHot::CLI::ComponentCLI
		{
		public:
			MeshComponent();
			void SetMeshData(MeshData^ meshData);
			void SetMaterial(Material^ material);
			virtual void Registered()
			{
				Instance->Registered();
			}

			virtual void Removed()
			{
				Instance->Removed();
			};

			virtual void Destroy()
			{
				Instance->Destroy();
			};

			virtual void Update(float deltaTime)
			{
				Instance->Update(deltaTime);
			};

			virtual void LateUpdate(float deltaTime)
			{
				Instance->LateUpdate(deltaTime);
			};

			virtual void FixedUpdate(float deltaTime)
			{
				Instance->FixedUpdate(deltaTime);
			};

			virtual void Register(CLI::EntityCLI^ parent){}
			virtual void Remove(){}
			virtual CLI::EntityCLI^ GetParent(){ return nullptr; }
		};
	};
};
