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
			Entity->GetHandle()->GetTransform().SetRotation(Quat4(Vec3(0, 0, 1), rotation->z) * Quat4(Vec3(0, 1, 0), rotation->y) * Quat4(Vec3(1, 0, 0), rotation->x));
		}

		void TransformComponentCLI::SetLocalPosition(Vec3CLI^ position)		
		{
			SHF_PRINTF("SetLocalPosition to [%f, %f, %f] \n", position->x, position->y, position->z);
			Entity->GetHandle()->GetTransform().SetLocalPosition(Vec3(position->x, position->y, position->z));
		}

		void TransformComponentCLI::SetScale(Vec3CLI^ scale)
		{
			SHF_PRINTF("SetLocalScale to [%f, %f, %f] \n", scale->x, scale->y, scale->z);
			Entity->GetHandle()->GetTransform().SetLocalScale(Vec3(scale->x, scale->y, scale->z));
		}

		void TransformComponentCLI::SetGlobalScale(Vec3CLI^ scale)
		{
			Entity->GetHandle()->GetTransform().SetGlobalScale(Vec3(scale->x, scale->y, scale->z));
		}

		void TransformComponentCLI::SetGlobalPosition(Vec3CLI^ position)
		{
			Entity->GetHandle()->GetTransform().SetGlobalPosition(Vec3(position->x, position->y, position->z));
		}

		Vec3CLI^ TransformComponentCLI::GetLocalPosition()
		{
			Vec3* position = Entity->GetHandle()->GetTransform().GetLocalPosition();
			Vec3CLI^ vec3CLI = gcnew Vec3CLI(position->x, position->y, position->z);
			return vec3CLI;
		}

		Vec3CLI^ TransformComponentCLI::GetRotation()
		{
			Vec3 eulerAngles = Entity->GetHandle()->GetTransform().GetRotation()->ToEulerAngles();
			Vec3CLI^ vec3CLI = gcnew Vec3CLI(eulerAngles.x * RADIANS_TO_DEGREES, eulerAngles.y * RADIANS_TO_DEGREES, eulerAngles.z * RADIANS_TO_DEGREES);
			return vec3CLI;
		}

		Vec3CLI^ TransformComponentCLI::GetScale()
		{
			Vec3* scale = Entity->GetHandle()->GetTransform().GetLocalScale();
			Vec3CLI^ vec3CLI = gcnew Vec3CLI(scale->x, scale->y, scale->z);
			return vec3CLI;
		}

		Vec3CLI^ TransformComponentCLI::GetGlobalScale()
		{
			Vec3* scale = Entity->GetHandle()->GetTransform().GetGlobalScale();
			Vec3CLI^ vec3CLI = gcnew Vec3CLI(scale->x, scale->y, scale->z);
			return vec3CLI;
		}

		Vec3CLI^ TransformComponentCLI::GetGlobalPosition()
		{
			Vec3 position = Entity->GetHandle()->GetTransform().GetGlobalPosition();
			Vec3CLI^ vec3CLI = gcnew Vec3CLI(position.x, position.y, position.z);
			return vec3CLI;
		}
	};
};