#pragma once

/**
*	Simple mesh data renderer.
*	(Temporary)
*/
namespace SupraHot
{
	namespace Graphics{
		class Camera;
		class Shader;
		class MeshData;
	}

	class MeshDataRenderer
	{
	private:
		MeshDataRenderer();
	public:
		static MeshDataRenderer& GetInstance();

		void Render(Graphics::Camera* camera, Graphics::MeshData* meshData, Graphics::Shader* shader);
		~MeshDataRenderer();
	};
};