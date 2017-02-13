#include <algorithm>

#include "MeshDataRenderer.h"
#include "Shader.h"
#include "MeshData.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshComponent.h"
#include "Entity.h"

#define BUFFEROFFSET(x) ( (char*) NULL + (x) )
#include "Sorting.h"

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
		// Perform sorted insert

		// Sort-in the mesh component
		// First find a valid place for the shader being used and then look for a spot for the material!
		if (std::find(MeshComponents.begin(), MeshComponents.end(), meshComponent) == MeshComponents.end())
		{
			if (MeshComponents.size() > 0)
			{
				uint64 shaderUUID = meshComponent->GetMaterial()->GetShader()->GetUUID();

				int maxIndex = Utils::Sorting::FindMaxShaderIndex(MeshComponents, shaderUUID);

				if (maxIndex == -1)
				{
					// Find spot to insert the shader.
					int insertionIndex = Utils::Sorting::FindShaderInsertionIndex(MeshComponents, shaderUUID);
					MeshComponents.insert(MeshComponents.begin() + insertionIndex, meshComponent);					
				}
				else
				{
					int minIndex = Utils::Sorting::FindMinShaderIndex(MeshComponents, 0, maxIndex, shaderUUID);

					// Now we need to sort in the mesh component into the sub array of [minIndex, maxIndex] by it's material UUID
					// #1 Check the max index of the given material uuid,
					// if the max index is -1, then we just insert the new material at the maxIndex of the Shader
					// If it is > -1, then we insert it at the max material index

					uint64 materialUUID = meshComponent->GetMaterial()->GetUUID(); 
					int maxMaterialIndex = Utils::Sorting::FindMaxMaterialIndex(MeshComponents, minIndex, maxIndex, materialUUID);

					if (maxMaterialIndex == -1)
					{
						MeshComponents.insert(MeshComponents.begin() + maxIndex, meshComponent);
					}
					else
					{
						MeshComponents.insert(MeshComponents.begin() + maxMaterialIndex, meshComponent);
					}
				}
			}
			else
			{
				MeshComponents.push_back(meshComponent);
			}
		}



		// TODO: Now that we know the position, where to insert the mesh, we can directly push it into our render command queue
		// #1 for the start, we can just rebuild it first
		// and later on we can do some smart things, to directly insert it.


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

	void MeshDataRenderer::RenderMain(Graphics::Camera* camera)
	{
		// Cache matrices
		Mat4 viewMatrix = *camera->GetViewMatrix();
		Mat4 projectionMatrix = *camera->GetProjectionMatrix();
		Mat4 viewProjectionMatrix = *camera->GetViewProjectionMatrix();
		Vec3 cameraPosition = camera->Position;

		// Compute frustum.
		CameraFrustum.Compute(viewProjectionMatrix);

		// Lists for sorting afterwards
		std::vector<MeshComponent*> transparentMeshes;
		std::vector<MeshComponent*> opaqueMeshes;

		// Frustum culling.
		for (MeshComponent* meshComponent : MeshComponents)
		{
			if (meshComponent->GetMeshData().get() == nullptr || meshComponent->GetMaterial() == nullptr)
			{
				continue;
			}

			Graphics::MeshData* meshData = meshComponent->GetMeshData().get();
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
			if (!intersect)
			{
				continue;
			}

			// Sort by meshes (opaque vs. transparent)
			if (meshComponent->GetMaterial()->Transparent)
			{
				transparentMeshes.push_back(meshComponent);
			}
			else
			{
				opaqueMeshes.push_back(meshComponent);
			}
		}

		// Sort transparent meshes by depth
		if (transparentMeshes.size() > 1)
		{
			Utils::Sorting::BubbleSortZ(transparentMeshes, camera);
		}

		// Separate meshes by their shader descriptions
		std::unordered_map<std::string, std::vector<MeshComponent*>> opaqueMeshHashmap;
		for (MeshComponent* meshComponent : opaqueMeshes)
		{
			// We need both the shader name, which can be MeshDefaultShader [ShaderPermutationIndex]
			// But since multiple materials can use the same shader, we need to mix in the material file path,
			// so that it it unique!
			std::string shaderMaterialHash = meshComponent->GetMaterial()->GetShader()->GetName() + meshComponent->GetMaterial()->GetMaterialInputs()->GetMaterialFilePath();
			opaqueMeshHashmap[shaderMaterialHash].push_back(meshComponent);
		}

		// Sort meshes
		typedef std::unordered_map<std::string, std::vector<MeshComponent*>>::iterator it_type;
		for (it_type iterator = opaqueMeshHashmap.begin(); iterator != opaqueMeshHashmap.end(); ++iterator)
		{
			std::vector<MeshComponent*>& opaqueMeshVector = iterator->second;

			// No need to sort them, because the shader name already contains it's shaderindex, which forms a unique identifier

			/*if (opaqueMeshVector.size() > 2)
			{
			Utils::Sorting::BubbleSortShaderIndex(opaqueMeshVector);
			}*/

			// Render opaque meshes as they arrive.
			RenderOpaque(opaqueMeshVector, camera);
		}

		// Render transparent meshes
		RenderTransparency(transparentMeshes, camera);
	}

	void MeshDataRenderer::RenderTransparency(std::vector<MeshComponent*>& transparentMeshcomponents, Graphics::Camera* camera)
	{
		Mat4 viewMatrix = *camera->GetViewMatrix();
		Mat4 projectionMatrix = *camera->GetProjectionMatrix();
		Mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (MeshComponent* meshComponent : transparentMeshcomponents)
		{
			Graphics::Shader* shader = meshComponent->GetMaterial()->GetShader();
			Graphics::MeshData* meshData = meshComponent->GetMeshData().get();

			// Bind shader
			shader->Attach();

			// Bind uniforms
			GLuint shaderProgramID = shader->GetShaderID();

			Mat4 modelMatrix = meshComponent->GetParent()->GetTransform().GetTransformation();
			Mat4 modelViewMatrix = viewMatrix * modelMatrix;
			Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

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

			glBindVertexArray(0);

			shader->Detach();
		}

		glDisable(GL_BLEND);
	}

	void MeshDataRenderer::RenderOpaque(std::vector<MeshComponent*>& opaqueMeshComponents, Graphics::Camera* camera)
	{
		Mat4 viewMatrix = *camera->GetViewMatrix();
		Mat4 projectionMatrix = *camera->GetProjectionMatrix();
		Mat4 viewProjectionMatrix = *camera->GetViewProjectionMatrix();

		Graphics::Shader* shader = opaqueMeshComponents.at(0)->GetMaterial()->GetShader();
		shader->Attach();
		GLuint shaderProgramID = shader->GetShaderID();

		shader->SetMat4(glGetUniformLocation(shaderProgramID, "ProjectionMatrix"), projectionMatrix);
		shader->SetMat4(glGetUniformLocation(shaderProgramID, "ViewMatrix"), viewMatrix);

		opaqueMeshComponents.at(0)->GetMaterial()->GetMaterialInputs()->Apply(shader);

		for (MeshComponent* meshComponent : opaqueMeshComponents)
		{
			Graphics::MeshData* meshData = meshComponent->GetMeshData().get();

			Mat4 modelMatrix = meshComponent->GetParent()->GetTransform().GetTransformation();
			Mat4 modelViewMatrix = viewMatrix * modelMatrix;
			Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

			// Set camera matrices
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelViewProjectionMatrix"), modelViewProjectionMatrix);
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelViewMatrix"), modelViewMatrix);
			shader->SetMat3(glGetUniformLocation(shaderProgramID, "NormalMatrix"), modelViewMatrix);
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelMatrix"), modelMatrix);

			// Bind GL buffers
			// Todo: next, sort also by meshData similarities.
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

		shader->Detach();
	}

	std::vector<MeshComponent*>& MeshDataRenderer::GetMeshComponents()
	{
		return MeshComponents;
	}

	MeshDataRenderer::~MeshDataRenderer()
	{
	}
};