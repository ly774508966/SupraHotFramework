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
			Vec3 Forward;
			Vec3 Right;
			Vec3 Up;

			Vec3 Position;
			float yaw = 0, pitch = 0;
			float mouseX = 0, mouseY = 0, mouseDX = 0, mouseDY = 0;
			bool mouseGrabbed = false;

			float mouseSpeed = 1.5f;
			float moveSpeed = 5.0f;

			float fieldOfView;
			float zNear, zFar;
			float aspectRatio;

			Camera(float fov, float zNear, float zFar, float aspectRatio);
			~Camera();

			void ResetMatrices();
			void ApplyTranslation();
			void ApplyRotation();
			//void UpdateInput(Controls* controls, float deltaTime);

			void setMouseSpeed(float speed);
			void setMoveSpeed(float speed);
			void moveDirection(float direction, float delta);
			void moveYAxis(float direction, float delta);
			void moveFromLook(float dx, float dy, float dz, float delta);

			Vec3 GetForwardDirection();
			Vec3 GetBackwardDirection();
			Vec3 GetRightDirection();
			Vec3 GetLeftDirection();
			Vec3 GetUpDirection();
			Vec3 GetDownDirection();

			Mat4* GetViewMatrix();
			Mat4* GetProjectionMatrix();
			Mat4* GetViewProjectionMatrix();
			Mat4* GetInverseProjectionViewMatrix();

			void CreateViewProjectionMatrix();
			void CreateInverseViewProjectionMatrix();
		};

	};
};