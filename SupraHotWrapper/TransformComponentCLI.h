#pragma once
#include <Platform.h>
#include <Entity.h>
#include <Transform.h>
#include "EntityCLI.h"
#include "Vec3CLI.h"

namespace SupraHot
{
	namespace CLI
	{
		public ref class TransformComponentCLI
		{
		private:
			EntityCLI^ Entity;
		public:
			TransformComponentCLI(EntityCLI^ entityCli);

			void SetRotation(Vec3CLI^ rotation);
			void SetLocalPosition(Vec3CLI^ position);
			void SetScale(Vec3CLI^ scale);
			void SetGlobalScale(Vec3CLI^ scale);
			void SetGlobalPosition(Vec3CLI^ position);

			Vec3CLI^ GetLocalPosition();
			Vec3CLI^ GetRotation();
			Vec3CLI^ GetScale();
			Vec3CLI^ GetGlobalScale();
			Vec3CLI^ GetGlobalPosition();
		};

	};
};