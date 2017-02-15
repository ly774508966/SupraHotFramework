#include "SetCameraMatricesCmd.h"
#include "Camera.h"
#include "RenderCommandQueue.h"
#include "Shader.h"

namespace SupraHot
{
	namespace Graphics
	{
		SetCameraMatricesCmd::SetCameraMatricesCmd(Graphics::Camera* camera)
		{
			this->Camera = camera;
		}

		SetCameraMatricesCmd::~SetCameraMatricesCmd()
		{

		}

		void SetCameraMatricesCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			renderCommandQueueState->ActiveCamera = Camera;
			
			Mat4 viewMatrix = *Camera->GetViewMatrix();
			Mat4 projectionMatrix = *Camera->GetProjectionMatrix();
			Mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

			GLuint shaderProgramID = renderCommandQueueState->ActiveShader->GetShaderID();
			
			renderCommandQueueState->ActiveShader->SetMat4(glGetUniformLocation(shaderProgramID, "ViewMatrix"), viewMatrix);
			renderCommandQueueState->ActiveShader->SetMat4(glGetUniformLocation(shaderProgramID, "ProjectionMatrix"), projectionMatrix);
			renderCommandQueueState->ActiveShader->SetMat4(glGetUniformLocation(shaderProgramID, "ViewProjectionMatrix"), viewProjectionMatrix);
		}
	};
};