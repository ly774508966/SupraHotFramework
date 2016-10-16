#include "Vertex.h"


namespace SupraHot
{
	namespace Graphics
	{
		Vertex::Vertex()
		{
		}


		Vertex::~Vertex()
		{
		}

		void Vertex::SetPosition(const Vec3& position)
		{
			this->Position = position;
		}

		void Vertex::SetNormal(const Vec3& normal)
		{
			this->Normal = normal;
		}

		void Vertex::SetTangent(const Vec3& tangent)
		{
			this->Tangent = tangent;
		}

		void Vertex::SetBiNormal(const Vec3& biNormal)
		{
			this->BiNormal = biNormal;
		}

		void Vertex::SetUVCoord(const Vec2& uvCoord)
		{
			this->UV = uvCoord;
		}

		Vec3 Vertex::GetPosition()
		{
			return Position;
		}

		Vec3 Vertex::GetNormal()
		{
			return Normal;
		}

		Vec3 Vertex::GetTangent()
		{
			return Tangent;
		}

		Vec3 Vertex::GetBiNormal()
		{
			return BiNormal;
		}

		Vec2 Vertex::GetUVCoord()
		{
			return UV;
		}
	};
};
