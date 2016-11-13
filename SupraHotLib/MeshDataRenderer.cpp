#include "MeshDataRenderer.h"
#include "Shader.h"
#include "MeshData.h"
#include "Camera.h"

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

		Mat4 modelMatrix = {};
		Mat4 modelViewMatrix = viewMatrix * modelMatrix;
		Mat4 modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;

		// Set camera matrices
		shader->SetMat4(glGetUniformLocation(shaderProgramID, "ModelViewProjectionMatrix"), modelViewProjectionMatrix);

		// Bind GL buffers
		glBindVertexArray(meshData->VAOHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData->IndexBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, meshData->VertexBufferHandle);

		uint32 vertexPositionAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexPosition");
		uint32 vertexNormalAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexNormal");
		uint32 vertexUVAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexUV");
		uint32 vertexTangentAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexTangent");
		uint32 vertexBiTangentAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexBiTangent");

		glEnableVertexAttribArray(vertexPositionAttrib);
		glVertexAttribPointer(vertexPositionAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		// Layout
		// Vertex attributes are stored progressively not interleaved.
		// First come ALL positions, then ALL normals, etc.
		// And not: Vtx, Vtx, Vtx, Nml, Nml, Nml, vtx, vtx, vtx...

		glDrawElements(GL_TRIANGLES, (meshData->FaceCount * 3), meshData->GlIndexType, nullptr);

		glBindVertexArray(0);

		shader->Detach();
	}

	MeshDataRenderer::~MeshDataRenderer()
	{
	}
};