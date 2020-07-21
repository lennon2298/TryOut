#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader shader)
	:m_QuadVAO(0)
{
	m_Shader = shader;
	InitRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	m_Shader.Bind();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	//m_Shader.GetUniform1i("u_Texture", 0);
	m_Shader.GetUniformMat4f("u_Model", model);
	m_Shader.GetUniform3f("u_SpriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(m_QuadVAO);
	glDisableVertexAttribArray(2);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::DrawInstanced(unsigned int instanced, glm::vec2& translationArray, glm::vec2 size, glm::vec3 color, float rotate)
{
	m_Shader.Bind();

	unsigned int instanceVBO;
	GLCall(glGenBuffers(1, &instanceVBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, instanceVBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * instanced, &translationArray[0], GL_STATIC_DRAW));
	//GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

	GLCall(glBindVertexArray(m_QuadVAO));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, instanceVBO));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glVertexAttribDivisor(2, 1));

	//m_Shader.GetUniform1i("u_Texture", 0);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_Shader.GetUniformMat4f("u_Model", model);
	m_Shader.GetUniform3f("u_SpriteColor", color);

	//glBindVertexArray(m_QuadVAO);
	GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, instanced));
	//glBindVertexArray(0);
}

void SpriteRenderer::InitRenderData()
{
	unsigned int VBO;
	float vertices[] = {
		// pos      // tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	GLCall(glGenVertexArrays(1, &m_QuadVAO));
	GLCall(glGenBuffers(1, &VBO));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GLCall(glBindVertexArray(m_QuadVAO));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));

	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	//GLCall(glBindVertexArray(0));
}
