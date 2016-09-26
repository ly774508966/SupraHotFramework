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
			this->viewMatrix.Identity();
			this->projectionMatrix.Identity();
		}

		void Camera::ApplyTranslation()
		{
			Mat4 translationMatrix;
			translationMatrix.SetTranslationVector(-position);

			this->viewMatrix = translationMatrix * this->viewMatrix;
			this->projectionMatrix = this->projectionMatrix.ProjectPerspective(fieldOfView, aspectRatio, zNear, zFar);
		}

		void Camera::ApplyRotation()
		{
			Quat4 xRotation = Quat4(Vec3(1, 0, 0), pitch).Normalized();
			Quat4 yRotation = Quat4(xRotation.GetUp(), yaw).Normalized();
			Quat4 totalRotation = (yRotation * xRotation).Normalized();

			Mat4 rotationMatrix;
			rotationMatrix = rotationMatrix.ToRotationMatrix(totalRotation);
			this->viewMatrix = rotationMatrix * this->viewMatrix;

			Quat4 xRot = Quat4(Vec3(1, 0, 0), -pitch);
			Quat4 yRot = Quat4(Vec3(0, 1, 0), -yaw);
			Quat4 totRot = totalRotation.Inversed();
			forward = (totRot * Vec3(0, 0, -1)).normalized();
			right = (totRot * Vec3(1, 0, 0)).normalized();
			up = (totRot * Vec3(0, 1, 0)).normalized();
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
			position.x -= sin(rad) * moveSpeed * delta;
			position.z -= cos(rad) * moveSpeed * delta;
		}

		void Camera::moveYAxis(float direction, float delta)
		{
			float rad = ToRadians((pitch + direction));
			position.y += sin(rad) * moveSpeed * delta;
		}


		Mat4* Camera::getViewMatrix()
		{
			return &this->viewMatrix;
		}

		Mat4* Camera::getProjectionMatrix()
		{
			return &this->projectionMatrix;
		}

		Vec3 Camera::getForwardDirection()
		{
			return forward;
		}

		Vec3 Camera::getBackwardDirection()
		{
			return -forward;
		}

		Vec3 Camera::getRightDirection()
		{
			return right;
		}

		Vec3 Camera::getUpDirection()
		{
			return up;
		}

		Vec3 Camera::getDownDirection()
		{
			return -up;
		}

		Vec3 Camera::getLeftDirection()
		{
			return -right;
		}

		void Camera::moveFromLook(float dx, float dy, float dz, float delta)
		{
			position.z += (dx * (float)cos(ToRadians(yaw - 90.0f)) + dz * cos(ToRadians(yaw)) * moveSpeed) * delta;
			position.x -= (dx * (float)sin(ToRadians(yaw - 90.0f)) + dz * sin(ToRadians(yaw)) * moveSpeed) * delta;
			position.y += (dy * (float)sin(ToRadians(pitch - 90.0f)) + dz * sin(ToRadians(pitch)) * moveSpeed) * delta;
		}

	};
};