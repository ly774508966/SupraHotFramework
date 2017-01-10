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
			void SetPosition(Vec3CLI^ position);
			void SetScale(Vec3CLI^ scale);

			Vec3CLI^ GetPosition();
			Vec3CLI^ GetRotation();
			Vec3CLI^ GetScale();
		};

	};
};