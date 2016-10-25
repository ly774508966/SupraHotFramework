#include "Camera.h"

namespace SupraHot
{
	namespace Graphics
	{
		Camera::Camera(float fov, float zNear, float zFar, float aspectRatio)
		{
			this->fieldOfView = fov;
			this->zNear = zNear;
			this->zFar = zFar;
			this->aspectRatio = aspectRatio;
		}

		Camera::~Camera()
		{

		}

		void Camera::ResetMatrices()
		{
			this->ViewMatrix.Identity();
			this->ProjectionMatrix.Identity();
		}

		void Camera::ApplyTranslation()
		{
			Mat4 translationMatrix;
			translationMatrix.SetTranslationVector(-Position);

			this->ViewMatrix = translationMatrix * this->ViewMatrix;
			this->ProjectionMatrix = this->ProjectionMatrix.ProjectPerspective(fieldOfView, aspectRatio, zNear, zFar);
		}

		void Camera::ApplyRotation()
		{
			Quat4 xRotation = Quat4(Vec3(1, 0, 0), pitch).Normalized();
			Quat4 yRotation = Quat4(xRotation.GetUp(), yaw).Normalized();
			Quat4 totalRotation = (yRotation * xRotation).Normalized();

			Mat4 rotationMatrix;
			rotationMatrix = rotationMatrix.ToRotationMatrix(totalRotation);
			this->ViewMatrix = rotationMatrix * this->ViewMatrix;

			Quat4 xRot = Quat4(Vec3(1, 0, 0), -pitch);
			Quat4 yRot = Quat4(Vec3(0, 1, 0), -yaw);
			Quat4 totRot = totalRotation.Inversed();
			Forward = (totRot * Vec3(0, 0, -1)).normalized();
			Right = (totRot * Vec3(1, 0, 0)).normalized();
			Up = (totRot * Vec3(0, 1, 0)).normalized();
		}

		/*void Camera::UpdateInput(Controls* controls, float deltaTime)
		{
			mouseGrabbed = controls->IsMouseGrabbed();

			mouseX = static_cast<float>(controls->GetMouseX());
			mouseY = static_cast<float>(controls->GetMouseY());

			mouseDX = static_cast<float>(controls->GetMouseDX());
			mouseDY = static_cast<float>(controls->GetMouseDY());

			// update yaw & pitch
			if (mouseGrabbed)
			{
				pitch += mouseDY * mouseSpeed * deltaTime;
				yaw += mouseDX * mouseSpeed * deltaTime;
			}
			else
			{
				mouseDX = 0;
				mouseDY = 0;
			}

			if (pitch > 90.0f)
			{
				pitch = 90.0f;
			}
			else if (pitch < -90.0f)
			{
				pitch = -90.0f;
			}

			if (yaw < 0.0f)
			{
				yaw += 360.0f;
			}
			else if (yaw > 360.0f)
			{
				yaw -= 360.0f;
			}

		}*/

		void Camera::setMouseSpeed(float speed)
		{
			mouseSpeed = speed;
		}

		void Camera::setMoveSpeed(float speed)
		{
			moveSpeed = speed;
		}

		void Camera::moveDirection(float direction, float delta)
		{
			float rad = ToRadians((yaw + direction));
			Position.x -= sin(rad) * moveSpeed * delta;
			Position.z -= cos(rad) * moveSpeed * delta;
		}

		void Camera::moveYAxis(float direction, float delta)
		{
			float rad = ToRadians((pitch + direction));
			Position.y += sin(rad) * moveSpeed * delta;
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

		void Camera::CreateViewProjectionMatrix()
		{
			ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
		}

		void Camera::CreateInverseViewProjectionMatrix()
		{
			InverseViewProjectionMatrix = ViewProjectionMatrix.Inversed();
		}

		Vec3 Camera::GetForwardDirection()
		{
			return Forward;
		}

		Vec3 Camera::GetBackwardDirection()
		{
			return -Forward;
		}

		Vec3 Camera::GetRightDirection()
		{
			return Right;
		}

		Vec3 Camera::GetUpDirection()
		{
			return Up;
		}

		Vec3 Camera::GetDownDirection()
		{
			return -Up;
		}

		Vec3 Camera::GetLeftDirection()
		{
			return -Right;
		}

		void Camera::moveFromLook(float dx, float dy, float dz, float delta)
		{
			Position.z += (dx * (float)cos(ToRadians(yaw - 90.0f)) + dz * cos(ToRadians(yaw)) * moveSpeed) * delta;
			Position.x -= (dx * (float)sin(ToRadians(yaw - 90.0f)) + dz * sin(ToRadians(yaw)) * moveSpeed) * delta;
			Position.y += (dy * (float)sin(ToRadians(pitch - 90.0f)) + dz * sin(ToRadians(pitch)) * moveSpeed) * delta;
		}

	};
};