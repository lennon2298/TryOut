#include "Game.h"

SpriteRenderer *spriteRenderer;

Game::Game(unsigned int width, unsigned int height)
	:m_State(GameState::GAME_ACTIVE), m_keys(), m_Width(width), m_Height(height)
{
}

Game::~Game()
{
}

void Game::Init()
{
	(void)ResourceManager::LoadShader("./resources/shaders/TryOutVert.glsl", "./resources/shaders/TryOutFrag.glsl", "TryOut");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->m_Width),static_cast<float>(this->m_Width), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("TryOut").Bind();
	ResourceManager::GetShader("TryOut").GetUniform1i("u_Texture", 0);
	ResourceManager::GetShader("TryOut").GetUniformMat4f("u_Projection", projection);

	spriteRenderer = new SpriteRenderer(ResourceManager::GetShader("TryOut"));

	(void)ResourceManager::LoadTexture("./resources/textures/pepe.png", true, "pepe");
}

void Game::ProcessInput(float dt)
{
}

void Game::Render()
{
	spriteRenderer->DrawSprite(ResourceManager::GetTexture("pepe"),
		glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Game::Update(float dt)
{
}

void Game::SetState(GameState state)
{
	m_State = state;
}

void Game::SetKeyPress(int key)
{
}

void Game::SetKeyRelease(int key)
{
}

GameState Game::GetState()
{
	return m_State;
}
