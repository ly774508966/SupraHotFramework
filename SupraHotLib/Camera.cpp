#include "Camera.h"

#ifndef PLATFORM_ANDROID
#include "Controls.h"
#endif

namespace SupraHot
{
	namespace Graphics
	{
		Camera::Camera(float fov, float zNear, float zFar, float aspectRatio)
		{
			this->FieldOfView = fov;
			this->ZNear = zNear;
			this->ZFar = zFar;
			this->AspectRatio = aspectRatio;
		}

		Camera::~Camera()
		{

		}

		void Camera::ResetMatrices()
		{
			this->ViewMatrix.Identity();
			this->ProjectionMatrix.Identity();
		}

		void Camera::Update(float deltaTime)
		{
			ProjectionMatrix = ProjectionMatrix.ProjectPerspective(FieldOfView, AspectRatio, ZNear, ZFar);

			ProcessMouseInput(deltaTime);

			Quat4 orientation = GetQuaternion();

			Mat4 rotation; 
			rotation = rotation.ToRotationMatrix(orientation.Conjugated());

			Mat4 translation;
			translation.SetTranslationVector(-Position);
			
			ViewMatrix = rotation * translation;
			
			CreateViewProjectionMatrix();
			CreateInverseViewProjectionMatrix();
		}

		Mat4* Camera::GetViewMatrix()
		{
			return &this->ViewMatrix;
		}

		Mat4* Camera::GetProjectionMatrix()
		{
			return &this->ProjectionMatrix;
		}

		Mat4* Camera::GetViewProjectionMatrix()
		{
			return &this->ViewProjectionMatrix;
		}

		Mat4* Camera::GetInverseProjectionViewMatrix()
		{
			return &this->InverseViewProjectionMatrix;
		}

		Quat4 Camera::GetQuaternion()
		{
			return (Quat4(Vec3(0, 1, 0), -Yaw).Normalized() * Quat4(Vec3(1, 0, 0), -Pitch).Normalized());
		}

		void Camera::CreateViewProjectionMatrix()
		{
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		}

		void Camera::CreateInverseViewProjectionMatrix()
		{
			InverseViewProjectionMatrix = ViewProjectionMatrix.Inversed();
		}

		void Camera::ProcessMouseInput(float deltaTime)
		{

#ifndef PLATFORM_ANDROID
			if (Controls::GetInstance()->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1))
			{
				MouseGrabbed = true;
				Controls::GetInstance()->GrabMouse();
			}
			else if (Controls::GetInstance()->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
			{
				MouseGrabbed = false;
				Controls::GetInstance()->ReleaseMouse();
			}

			if (MouseGrabbed)
			{
				Pitch += Controls::GetInstance()->MouseDY * deltaTime * MouseSpeed;
				Yaw += Controls::GetInstance()->MouseDX * deltaTime * MouseSpeed;
			}
#endif
		}
	};
};