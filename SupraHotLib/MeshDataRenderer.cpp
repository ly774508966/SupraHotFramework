#include "MeshDataRenderer.h"
#include "Shader.h"
#include "MeshData.h"
#include "Camera.h"

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

	void MeshDataRenderer::Render(Graphics::Camera* camera, Graphics::MeshData* meshData, Graphics::Shader* shader)
	{
		// Bind shader
		shader->Attach();

		// Bind uniforms
		GLuint shaderProgramID = shader->GetShaderID();

		Mat4 viewMatrix = *camera->GetViewMatrix();
		Mat4 projectionMatrix = *camera->GetProjectionMatrix();

		Transform modelTransform;
		modelTransform.SetScale(Vec3(0.05f, 0.05f, 0.05f));
		modelTransform.SetLocalRotation(Quat4(Vec3(0, 0, 1), 90) * Quat4(Vec3(0, 1, 0), 90));

		Mat4 modelMatrix = modelTransform.GetTransformation();
		Mat4 modelViewMatrix = viewMatrix * modelMatrix;
		Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

		// Set camera matrices
		shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelViewProjectionMatrix"), modelViewProjectionMatrix);
		shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelViewMatrix"), modelViewMatrix);
		shader->SetMat3(glGetUniformLocation(shaderProgramID, "NormalMatrix"), modelViewMatrix);

		// Bind GL buffers
		glBindVertexArray(meshData->VAOHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->IndexBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, meshData->VertexBufferHandle);

		uint64 offset = 0;

		if (meshData->HasPositionData)
		{
			uint32 vertexPositionAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexPosition");
			glEnableVertexAttribArray(vertexPositionAttrib);
			glVertexAttribPointer(vertexPositionAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			offset += meshData->VertexCount * 3 * sizeof(float);
		}

		if (meshData->HasNormalData)
		{
			uint32 vertexNormalAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexNormal");
			glEnableVertexAttribArray(vertexNormalAttrib);
			glVertexAttribPointer(vertexNormalAttrib, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(offset));
			offset += meshData->VertexCount * 3 * sizeof(float);
		}

		if (meshData->HasUVData)
		{
			uint32 vertexUVAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexUV");
			glEnableVertexAttribArray(vertexUVAttrib);
			glVertexAttribPointer(vertexUVAttrib, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(offset));
			offset += meshData->VertexCount * 2 * sizeof(float);
		}

		if (meshData->HasTangentData)
		{
			uint32 vertexTangentAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexTangent");
			glEnableVertexAttribArray(vertexTangentAttrib);
			glVertexAttribPointer(vertexTangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(offset));
			offset += meshData->VertexCount * 3 * sizeof(float);
		}

		if (meshData->HasBiTangentData)
		{
			uint32 vertexBiTangentAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexBiTangent");
			glEnableVertexAttribArray(vertexBiTangentAttrib);
			glVertexAttribPointer(vertexBiTangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(offset));
			offset += meshData->VertexCount * 3 * sizeof(float);
		}

		// Set textures

		if (meshData->MeshMaterial.GetAlbedoMap() != nullptr)
		{
			shader->SetTexture2D(glGetUniformLocation(shaderProgramID, "AlbedoTexture"), meshData->MeshMaterial.GetAlbedoMap(), GL_TEXTURE0);
		}

		// Layout
		// Vertex attributes are stored sequential & not interleaved
		// (vtx.x,vtx.y,vtx.z...... nml.x,nml.y,nml.z......, uv.s,uv.t,uv.s,uv.t.......)

		glDrawElements(GL_TRIANGLES, (meshData->FaceCount * 3), meshData->GlIndexType, nullptr);

		glBindVertexArray(0);

		shader->Detach();
	}

	MeshDataRenderer::~MeshDataRenderer()
	{
	}
};