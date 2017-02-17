#include "RenderMeshCmd.h"
#include "MeshData.h"
#include "RenderCommandQueue.h"
#include "Shader.h"
#include "Camera.h"
#include "MeshComponent.h"
#include "Entity.h"
#include "Frustum.h"
#include <algorithm>

namespace SupraHot
{
	namespace Graphics
	{
		RenderMeshCmd::RenderMeshCmd(SupraHot::MeshComponent* meshComponent)
		{
			this->MeshComponent = meshComponent;
		}

		RenderMeshCmd::~RenderMeshCmd()
		{
		}

		void RenderMeshCmd::Execute(RenderCommandQueueState* renderCommandQueueState)
		{
			renderCommandQueueState->ActiveMeshComponent = MeshComponent;

			MeshData* meshData = MeshComponent->GetMeshData().get();


			// - - - - - - - - - View frustum culling

			Graphics::AABB& originalAABB = meshData->BoundingBox;
			Vec3 scale = *MeshComponent->GetParent()->GetTransform().GetGlobalScale();

			// This could be placed and calculated somewhere else, so that we don't need to
			// calculate it every single frame!.
			// We can cache this by calculating it, when the scale of the meshcomponent's transform changes.
			Vec3 scaledMaximum = originalAABB.GetMaximum() * scale;
			Vec3 scaledMinimum = originalAABB.GetMinimum() * scale;
			float min = std::min(scaledMinimum.x, std::min(scaledMinimum.y, scaledMinimum.z));
			float radius = std::max(scaledMaximum.x, std::max(scaledMaximum.y, scaledMaximum.z));
			radius = std::max(radius, std::abs(min));

			bool intersect = renderCommandQueueState->ActiveFrustum->IntersectsSphere(MeshComponent->GetParent()->GetTransform().GetGlobalPosition(), radius);
			if (!intersect)
			{
				return;
			}


			// - - - - - - - - - End view frustum culling


			Shader* shader = renderCommandQueueState->ActiveShader;
			GLuint shaderProgramID = shader->GetShaderID();

			Mat4 modelMatrix = MeshComponent->GetParent()->GetTransform().GetTransformation();
			Mat4 modelViewMatrix = *renderCommandQueueState->ActiveCamera->GetViewMatrix() * modelMatrix;
			Mat4 modelViewProjectionMatrix = *renderCommandQueueState->ActiveCamera->GetViewProjectionMatrix() * modelMatrix;

			// Set camera matrices
			// this can also be cached on the shader object itself.
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelViewProjectionMatrix"), modelViewProjectionMatrix);
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelViewMatrix"), modelViewMatrix);
			shader->SetMat3(glGetUniformLocation(shaderProgramID, "NormalMatrix"), modelViewMatrix);
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelMatrix"), modelMatrix);

			// Bind GL buffers
			// Todo: next, sort also by meshData similarities. 
			// Just use the vao handle for that.
			glBindVertexArray(meshData->VAOHandle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->IndexBufferHandle);
			glBindBuffer(GL_ARRAY_BUFFER, meshData->VertexBufferHandle);

			uint64 offset = 0;

			if (meshData->HasPositionData)
			{
				int vertexPositionAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexPosition");
				if (vertexPositionAttrib > -1)
				{
					glEnableVertexAttribArray(vertexPositionAttrib);
					glVertexAttribPointer(vertexPositionAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
				}
				offset += meshData->VertexCount * 3 * sizeof(float);
			}

			if (meshData->HasNormalData)
			{
				int vertexNormalAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexNormal");
				if (vertexNormalAttrib > -1)
				{
					glEnableVertexAttribArray(vertexNormalAttrib);
					glVertexAttribPointer(vertexNormalAttrib, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(offset));
				}
				offset += meshData->VertexCount * 3 * sizeof(float);
			}

			if (meshData->HasUVData)
			{
				int vertexUVAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexUV");
				if (vertexUVAttrib > -1)
				{
					glEnableVertexAttribArray(vertexUVAttrib);
					glVertexAttribPointer(vertexUVAttrib, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(offset));
				}
				offset += meshData->VertexCount * 2 * sizeof(float);
			}

			if (meshData->HasTangentData)
			{
				int vertexTangentAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexTangent");
				if (vertexTangentAttrib > -1)
				{
					glEnableVertexAttribArray(vertexTangentAttrib);
					glVertexAttribPointer(vertexTangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(offset));
				}
				offset += meshData->VertexCount * 3 * sizeof(float);
			}

			if (meshData->HasBiTangentData)
			{
				int vertexBiTangentAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexBiTangent");
				if (vertexBiTangentAttrib > -1)
				{
					glEnableVertexAttribArray(vertexBiTangentAttrib);
					glVertexAttribPointer(vertexBiTangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(offset));
				}
				offset += meshData->VertexCount * 3 * sizeof(float);
			}

			// Layout
			// Vertex attributes are stored sequential & not interleaved
			// (vtx.x,vtx.y,vtx.z...... nml.x,nml.y,nml.z......, uv.s,uv.t,uv.s,uv.t.......)
			// http://stackoverflow.com/questions/16380005/opengl-3-4-glvertexattribpointer-stride-and-offset-miscalculation

			glDrawElements(GL_TRIANGLES, (meshData->FaceCount * 3), meshData->GlIndexType, nullptr);

			glBindVertexArray(0);
		}
	};
};
