#include <algorithm>

#include "MeshDataRenderer.h"
#include "Shader.h"
#include "MeshData.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshComponent.h"
#include "Entity.h"
#include "Sorting.h"
#include "BindShaderCmd.h"
#include "SetCameraMatricesCmd.h"
#include "BindMaterialCmd.h"
#include "RenderMeshCmd.h"
#include "UnbindShaderCmd.h"
#include "BindBRDFRenderTargetsCmd.h"
#include "ResetRenderCommandQueueStateCmd.h"
#include "ComputeFrustumCmd.h"
#include "FrameBufferObject.h"
#include "GBuffer.h"

#define BUFFEROFFSET(x) ( (char*) NULL + (x) )
#include "BindGBufferCmd.h"
#include "UnbindGBufferCmd.h"

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

	void MeshDataRenderer::Initialize(Graphics::Camera* camera, uint32 width, uint32 height)
	{
		this->Camera = camera;
		this->GBuffer = new Graphics::GBuffer(width, height);
		
		for (uint32 i = 0; i < RenderTargetQueueSize; ++i)
		{
			Graphics::Texture2DPtr renderTarget = std::make_shared<Graphics::Texture2D>(GL_RGBA, GL_RGBA8, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
			renderTarget->SetName("RenderTarget | " + std::to_string(i));
			renderTarget->Init(width, height);
			RenderTargetQueue.push_back(renderTarget);
		}
	}

	void MeshDataRenderer::Resize(uint32 width, uint32 height)
	{
		this->GBuffer->Resize(width, height);

		for (uint32 i = 0; i < RenderTargetQueueSize; ++i)
		{
			RenderTargetQueue[i]->Destroy();
			RenderTargetQueue[i]->Init(width, height);
		}
	}

	void MeshDataRenderer::AddMeshComponent(MeshComponent* meshComponent)
	{
		// Perform sorted insert

		// Check if mesh vector is empty or not
		if (std::find(MeshComponents.begin(), MeshComponents.end(), meshComponent) == MeshComponents.end())
		{
			if (MeshComponents.size() > 0)
			{

				// Sort first by Shader-BRDF type
				Graphics::Shader::BRDFType brdfType = meshComponent->GetMaterial()->GetShader()->GetBRDF();
				int maxBRDFIndex = Utils::Sorting::FindMaxBRDFIndex(MeshComponents, 0, static_cast<int>(MeshComponents.size()) - 1, brdfType);

				if (maxBRDFIndex == -1)
				{
					// Find spot to insert the shader.
					int insertionIndex = Utils::Sorting::FindBRDFInsertionIndex(MeshComponents, 0, static_cast<int>(MeshComponents.size()), brdfType);
					MeshComponents.insert(MeshComponents.begin() + insertionIndex + 1, meshComponent);
				}
				else
				{
					
					int minBRDFIndex = Utils::Sorting::FindMinBRDFIndex(MeshComponents, 0, maxBRDFIndex, brdfType);

					// Sort-in the mesh component
					// First find a valid place for the shader being used and then look for a spot for the material!
					// The shader uuid already contains the shader permutation index!
					uint64 shaderUUID = meshComponent->GetMaterial()->GetShader()->GetUUID();

					int maxShaderIndex = Utils::Sorting::FindMaxShaderIndex(MeshComponents, minBRDFIndex, maxBRDFIndex, shaderUUID);

					if (maxShaderIndex == -1)
					{
						// Find spot to insert the shader.
						int insertionIndex = Utils::Sorting::FindShaderInsertionIndex(MeshComponents, minBRDFIndex, maxBRDFIndex, shaderUUID);
						MeshComponents.insert(MeshComponents.begin() + insertionIndex + 1, meshComponent);
					}
					else
					{
						int minShaderIndex = Utils::Sorting::FindMinShaderIndex(MeshComponents, minBRDFIndex, maxShaderIndex, shaderUUID);

						// Now we need to sort in the mesh component into the sub array of [minShaderIndex, maxShaderIndex] by it's material UUID
						// #1 Check the max index of the given material uuid,
						// if the max index is -1, then we just insert the new material at the maxShaderIndex of the Shader
						// If it is > -1, then we insert it at the max material index
						uint64 materialUUID = meshComponent->GetMaterial()->GetUUID();
						int maxMaterialIndex = Utils::Sorting::FindMaxMaterialIndex(MeshComponents, minShaderIndex, maxShaderIndex, materialUUID);

						if (maxMaterialIndex == -1)
						{
							MeshComponents.insert(MeshComponents.begin() + maxShaderIndex + 1, meshComponent);
						}
						else
						{
							MeshComponents.insert(MeshComponents.begin() + maxMaterialIndex + 1, meshComponent);
						}
					}
				}
			}
			else
			{
				MeshComponents.push_back(meshComponent);
			}
		}


		RebuildRenderCommandQueue();
	}

	void MeshDataRenderer::RemoveMeshComponent(MeshComponent* meshComponent)
	{
		// No bells, no whistles. Just remove the component and rebuild the CmdQueue
		if (std::find(MeshComponents.begin(), MeshComponents.end(), meshComponent) != MeshComponents.end())
		{
			MeshComponents.erase(std::remove(MeshComponents.begin(), MeshComponents.end(), meshComponent), MeshComponents.end());
			RebuildRenderCommandQueue();
		}
	}

	void MeshDataRenderer::RebuildRenderCommandQueue()
	{
		// Clear queue if it is not empty 
		if (RenderCommandQueue.Size() > 0)
		{
			ClearRenderCommandQueue();
		}

		// Inject here the gbuffer binding for the selected BRDF.
		// TODO: When we gonna use the cmd q like this, we also need to sort by BRDF-type!

		//RenderCommandQueue.AddCommand(new Graphics::BindGBufferCmd(GBuffer));

		MeshComponent* lastComponent = nullptr;

		for (size_t i = 0, l = MeshComponents.size(); i < l; ++i)
		{
			MeshComponent* currentMeshComponent = MeshComponents[i];

			if (lastComponent == nullptr)
			{
				RenderCommandQueue.AddCommand(new Graphics::BindBRDFRenderTargetsCmd(currentMeshComponent->GetMaterial()->GetShader()->GetBRDF()));
				RenderCommandQueue.AddCommand(new Graphics::BindShaderCmd(currentMeshComponent->GetMaterial()->GetShader()));
				RenderCommandQueue.AddCommand(new Graphics::SetCameraMatricesCmd(this->Camera));
				RenderCommandQueue.AddCommand(new Graphics::ComputeFrustumCmd(&this->CameraFrustum));
				RenderCommandQueue.AddCommand(new Graphics::BindMaterialCmd(currentMeshComponent->GetMaterial()));
				RenderCommandQueue.AddCommand(new Graphics::RenderMeshCmd(currentMeshComponent));
			}
			else
			{
				if (lastComponent->GetMaterial()->GetShader()->GetBRDF() != currentMeshComponent->GetMaterial()->GetShader()->GetBRDF())
				{
					// RenderCommandQueue.AddCommand(new Graphics::UnbindBRDFRenderTargetsCmd());
					RenderCommandQueue.AddCommand(new Graphics::UnbindShaderCmd());
					RenderCommandQueue.AddCommand(new Graphics::BindShaderCmd(currentMeshComponent->GetMaterial()->GetShader()));
					RenderCommandQueue.AddCommand(new Graphics::SetCameraMatricesCmd(this->Camera));
					RenderCommandQueue.AddCommand(new Graphics::BindMaterialCmd(currentMeshComponent->GetMaterial()));
					RenderCommandQueue.AddCommand(new Graphics::RenderMeshCmd(currentMeshComponent));
				}
				else if (lastComponent->GetMaterial()->GetShader() != currentMeshComponent->GetMaterial()->GetShader())
				{
					RenderCommandQueue.AddCommand(new Graphics::UnbindShaderCmd());
					RenderCommandQueue.AddCommand(new Graphics::BindShaderCmd(currentMeshComponent->GetMaterial()->GetShader()));
					RenderCommandQueue.AddCommand(new Graphics::SetCameraMatricesCmd(this->Camera));
					RenderCommandQueue.AddCommand(new Graphics::BindMaterialCmd(currentMeshComponent->GetMaterial()));
					RenderCommandQueue.AddCommand(new Graphics::RenderMeshCmd(currentMeshComponent));
				}
				else if (lastComponent->GetMaterial() != currentMeshComponent->GetMaterial())
				{
					RenderCommandQueue.AddCommand(new Graphics::BindMaterialCmd(currentMeshComponent->GetMaterial()));
					RenderCommandQueue.AddCommand(new Graphics::RenderMeshCmd(currentMeshComponent));
				}
				else
				{
					RenderCommandQueue.AddCommand(new Graphics::RenderMeshCmd(currentMeshComponent));
				}
			}

			lastComponent = currentMeshComponent;
		}

		//RenderCommandQueue.AddCommand(new Graphics::UnbindGBufferCmd(GBuffer));

		RenderCommandQueue.AddCommand(new Graphics::ResetRenderCommandQueueStateCmd());

		// SHF_PRINTF("RenderCommandQueue size = %llu \n", RenderCommandQueue.Size());
	}

	void MeshDataRenderer::ClearRenderCommandQueue()
	{
		RenderCommandQueue.Clear();
	}

	// Old methods
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

	// New method.
	void MeshDataRenderer::ExecuteRenderCommandQueue(Graphics::FrameBufferObject* framebuffer)
	{
		RenderCommandQueue.Execute();
		// framebuffer->SetReadSource( GBuffer->GetPositionsRT().get() );
	}

	std::vector<MeshComponent*>& MeshDataRenderer::GetMeshComponents()
	{
		return MeshComponents;
	}

	MeshDataRenderer::~MeshDataRenderer()
	{
		RenderCommandQueue.Clear();
		RenderTargetQueue.clear();
		delete GBuffer;		
	}
};