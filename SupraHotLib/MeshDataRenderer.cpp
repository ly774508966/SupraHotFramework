#include <algorithm>

#include "MeshDataRenderer.h"
#include "Shader.h"
#include "MeshData.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshComponent.h"
#include "Entity.h"

#define BUFFEROFFSET(x) ( (char*) NULL + (x) )

namespace SupraHot
{
	MeshDataRenderer::MeshDataRenderer()
	{
	}

	MeshDataRenderer& MeshDataRenderer::GetInstance()
	{
		static MeshDataRenderer* instance(new MeshDataRenderer);
		return *instance;
	}

	void MeshDataRenderer::AddMeshComponent(MeshComponent* meshComponent)
	{
		if (std::find(MeshComponents.begin(), MeshComponents.end(), meshComponent) == MeshComponents.end())
		{
			MeshComponents.push_back(meshComponent);
		}
	}

	void MeshDataRenderer::RemoveMeshComponent(MeshComponent* meshComponent)
	{
		if (std::find(MeshComponents.begin(), MeshComponents.end(), meshComponent) != MeshComponents.end())
		{
			MeshComponents.erase(std::remove(MeshComponents.begin(), MeshComponents.end(), meshComponent), MeshComponents.end());
		}
	}

	void MeshDataRenderer::Render(Graphics::Camera* camera)
	{		
		Mat4 viewMatrix = *camera->GetViewMatrix();
		Mat4 projectionMatrix = *camera->GetProjectionMatrix();
		Mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

		// Do frustum culling
		CameraFrustum.Compute(viewProjectionMatrix);
		FrustumCulling();

		uint32 cullcount = 0;
		for (MeshComponent* meshComponent : MeshComponents)
		{
			if (meshComponent->GetMeshData().get() == nullptr || meshComponent->GetMaterial() == nullptr)
			{
				continue;
			}

			Graphics::Shader* shader = meshComponent->GetMaterial()->GetShader();
			
			if (shader == nullptr)
			{
				continue;
			}

			Graphics::MeshData* meshData = meshComponent->GetMeshData().get();

			// Bind shader
			shader->Attach();

			// Bind uniforms
			GLuint shaderProgramID = shader->GetShaderID();

			Mat4 modelMatrix = meshComponent->GetParent()->GetTransform().GetTransformation();
			Mat4 modelViewMatrix = viewMatrix * modelMatrix;
			Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

			Graphics::AABB& originalAABB = meshData->BoundingBox;
			Vec3 scale = *meshComponent->GetParent()->GetTransform().GetGlobalScale();

			// This could be placed and calculated somewhere else, so that we don't need to
			// calculate it every single frame!
			Vec3 scaledMaximum = originalAABB.GetMaximum() * scale;
			Vec3 scaledMinimum = originalAABB.GetMinimum() * scale;
			float min = std::min(scaledMinimum.x, std::min(scaledMinimum.y, scaledMinimum.z));
			float radius = std::max(scaledMaximum.x, std::max(scaledMaximum.y, scaledMaximum.z));
			radius = std::max(radius, std::abs(min));
			 
			bool intersect = CameraFrustum.IntersectsSphere(meshComponent->GetParent()->GetTransform().GetGlobalPosition(), radius);			


			// TODO: Fix frustum culling vs aabb.
			// float determinant; Quat4 worldRotation; Vec3 worldTranslation; Vec3 worldScale;
			// modelMatrix.Decomposite(determinant, worldScale, worldTranslation, worldRotation);
			// bool intersect = CameraFrustum.IntersectsAABB(originalAABB, worldTranslation, worldScale, worldRotation);

			if (!intersect)
			{ 
				cullcount++;
				continue;
			}

			// Set camera matrices
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelViewProjectionMatrix"), modelViewProjectionMatrix);
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelViewMatrix"), modelViewMatrix);
			shader->SetMat3(glGetUniformLocation(shaderProgramID, "NormalMatrix"), modelViewMatrix);

			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelMatrix"), modelMatrix);
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ProjectionMatrix"), projectionMatrix);
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ViewMatrix"), viewMatrix);

			// Bind GL buffers
			glBindVertexArray(meshData->VAOHandle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->IndexBufferHandle);
			glBindBuffer(GL_ARRAY_BUFFER, meshData->VertexBufferHandle);

			uint64 offset = 0;

			if (meshData->HasPositionData)
			{
				// TODO: make the location attribs "static" to a shader permutation.
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
			
			meshComponent->GetMaterial()->GetMaterialInputs()->Apply(shader);

			// Layout
			// Vertex attributes are stored sequential & not interleaved
			// (vtx.x,vtx.y,vtx.z...... nml.x,nml.y,nml.z......, uv.s,uv.t,uv.s,uv.t.......)
			// http://stackoverflow.com/questions/16380005/opengl-3-4-glvertexattribpointer-stride-and-offset-miscalculation

			glDrawElements(GL_TRIANGLES, (meshData->FaceCount * 3), meshData->GlIndexType, nullptr);

			// meshComponent->GetMaterial()->Unbind();

			glBindVertexArray(0);

			shader->Detach();
		}

		//printf("Culling %d objects \n", cullcount);

	}

	void MeshDataRenderer::FrustumCulling()
	{
	}

	void MeshDataRenderer::BucketSorting()
	{
	}
	
	MeshDataRenderer::~MeshDataRenderer()
	{
	}
};