#pragma once
#include "Platform.h"
#include "Vertex.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Face
		{
		private:
			Vertex Vertices[3];
			unsigned int Index;
		public:
			Face();///< constructs an empty face
			~Face();

			Vertex& GetVertex(unsigned int index);
			Vertex* GetVertices();

			void SetVertices(const Vertex* vertices);
			void SetVertex(const Vertex& vertex, unsigned int index);
		};
	};
};

