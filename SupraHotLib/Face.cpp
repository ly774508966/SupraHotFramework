#include "Face.h"

namespace SupraHot
{
	namespace Graphics
	{
		Face::Face()
		{
		}

		Face::~Face()
		{
		}

		Vertex& Face::GetVertex(unsigned int index)
		{
			return Vertices[index];
		}

		Vertex* Face::GetVertices()
		{
			return Vertices;
		}

		void Face::SetVertices(const Vertex* vertices)
		{
			this->Vertices[0] = vertices[0];
			this->Vertices[1] = vertices[1];
			this->Vertices[2] = vertices[2];
		}

		void Face::SetVertex(const Vertex& vertex, unsigned int index)
		{
			Vertices[index] = vertex;
		}
	};
};
