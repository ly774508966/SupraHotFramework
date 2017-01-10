#include "TransformComponentCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		TransformComponentCLI::TransformComponentCLI(EntityCLI^ entityCli)
		{
			this->Entity = entityCli;
		}

		void TransformComponentCLI::SetRotation(Vec3CLI^ rotation)
		{
			Entity->GetHandle()->GetTransform().SetLocalRotation(Quat4(Vec3(0, 0, 1), rotation->z) * Quat4(Vec3(0, 1, 0), rotation->y) * Quat4(Vec3(1, 0, 0), rotation->x));
		}

		void TransformComponentCLI::SetPosition(Vec3CLI^ position)		
		{
			Entity->GetHandle()->GetTransform().SetPosition(Vec3(position->x, position->y, position->z));
		}

		void TransformComponentCLI::SetScale(Vec3CLI^ scale)
		{
			Entity->GetHandle()->GetTransform().SetLocalScale(Vec3(scale->x, scale->y, scale->z));
		}

		Vec3CLI^ TransformComponentCLI::GetPosition()
		{
			Vec3* position = Entity->GetHandle()->GetTransform().GetPosition();
			Vec3CLI^ vec3CLI = gcnew Vec3CLI(position->x, position->y, position->z);
			return vec3CLI;
		}

		Vec3CLI^ TransformComponentCLI::GetRotation()
		{
			Vec3 eulerAngles = Entity->GetHandle()->GetTransform().GetLocalRotation()->ToEulerAngles();
			Vec3CLI^ vec3CLI = gcnew Vec3CLI(eulerAngles.x, eulerAngles.y, eulerAngles.z);
			return vec3CLI;
		}

		Vec3CLI^ TransformComponentCLI::GetScale()
		{
			Vec3* scale = Entity->GetHandle()->GetTransform().GetLocalScale();
			Vec3CLI^ vec3CLI = gcnew Vec3CLI(scale->x, scale->y, scale->z);
			return vec3CLI;
		}
	};
};