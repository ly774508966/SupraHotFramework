#pragma once
#include "Platform.h"
#include "Transform.h"

// TODO: TEMP CAMERA! Will be replaced with a better cam, which can be build from 3 vectors or with lookAt
// TODO: Create cam with a single Transform!

namespace SupraHot
{
	namespace Graphics
	{
		using namespace Math;

		class Camera {
		public:
			Mat4 ViewMatrix;
			Mat4 ProjectionMatrix;
			Mat4 InverseViewProjectionMatrix;
			Mat4 ViewProjectionMatrix;
			
			Vec3 Position;
			Vec3 Rotation;

			float Yaw = 0, 
				  Pitch = 0;

			float MouseX = 0, 
				  MouseY = 0, 
				  MouseDX = 0, 
				  MouseDY = 0;

			bool MouseGrabbed = false;

			float MouseSpeed = 1.0f;
			float MoveSpeed = 2.0f;

			float FieldOfView;
			float ZNear, ZFar;
			float AspectRatio;

			Camera(float fov, float zNear, float zFar, float aspectRatio);
			~Camera();

			void ResetMatrices();
			void Update(float deltaTime);

			Mat4* GetViewMatrix();
			Mat4* GetProjectionMatrix();
			Mat4* GetViewProjectionMatrix();
			Mat4* GetInverseProjectionViewMatrix();

			Quat4 GetQuaternion();

		private:
			void CreateViewProjectionMatrix();
			void CreateInverseViewProjectionMatrix();
			void ProcessMouseInput(float deltaTime);
		};

	};
};