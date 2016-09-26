#include "Renderer.h"


namespace SupraHot
{
	namespace Graphics
	{
		Renderer& Renderer::GetInstance()
		{
			static Renderer* instance(new Renderer);
			return *instance;
		}

		Renderer::Renderer()
		{
		}

		Renderer::~Renderer()
		{
		}

		void Renderer::RenderSingleMeshComponent(MeshComponent* meshComponent)
		{
			// Render the Meshcomponent
		}
	};
};