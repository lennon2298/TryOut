#include "Game.h"

SpriteRenderer *spriteRenderer;

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

GameObject *Player;

Game::Game(unsigned int width, unsigned int height)
	:m_State(GameState::GAME_ACTIVE), m_Keys(), m_Width(width), m_Height(height), level(0)
{
}

Game::~Game()
{
}

void Game::Init()
{
	(void)ResourceManager::LoadShader("./Resources/Shaders/TryOutVert.glsl", "./Resources/Shaders/TryOutFrag.glsl", "TryOut");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width),static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("TryOut").Bind();
	ResourceManager::GetShader("TryOut").GetUniform1i("u_Texture", 0);
	ResourceManager::GetShader("TryOut").GetUniformMat4f("u_Projection", projection);

	spriteRenderer = new SpriteRenderer(ResourceManager::GetShader("TryOut"));

	(void)ResourceManager::LoadTexture("./Resources/Textures/container2.png", true, "pepe");
	(void)ResourceManager::LoadTexture("./Resources/Textures/paddle.png", true, "player");
	(void)ResourceManager::LoadTexture("./Resources/Textures/block.png", true, "block");
	(void)ResourceManager::LoadTexture("./Resources/Textures/block_solid.png", true, "block_solid");

	GameLevel one; one.Load("./Resources/levels/one.lvl", this->m_Width, this->m_Height / 2);
	GameLevel two; two.Load("./Resources/levels/two.lvl", this->m_Width, this->m_Height / 2);
	GameLevel three; three.Load("./Resources/levels/three.lvl", this->m_Width, this->m_Height / 2);
	GameLevel four; four.Load("./Resources/levels/four.lvl", this->m_Width, this->m_Height / 2);

	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->level = 0;

	glm::vec2 playerPos = glm::vec2( this->m_Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->m_Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player"));
}

void Game::ProcessInput(float dt)
{
	if (this->m_State == GameState::GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		// move playerboard
		if (m_Keys[GLFW_KEY_A] || m_Keys[GLFW_KEY_LEFT])
		{
			if (Player->GetPosition() >= 0.0f)
				Player->UpdateVelocity(-velocity);
		}
		if (m_Keys[GLFW_KEY_D] || m_Keys[GLFW_KEY_RIGHT])
		{
			if (Player->GetPosition() <= this->m_Width - Player->GetSize())
				Player->UpdateVelocity(velocity);
		}
	}
}

void Game::Render()
{
	if (this->m_State == GameState::GAME_ACTIVE)
	{
		// draw background
		spriteRenderer->DrawSprite(ResourceManager::GetTexture("pepe"),
			glm::vec2(0.0f, 0.0f), glm::vec2(this->m_Width, this->m_Height), 0.0f);
		// draw level
		this->Levels[this->level].Draw(*spriteRenderer);
		Player->Draw(*spriteRenderer);
	}
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
	m_Keys[key] = true;
}

void Game::SetKeyRelease(int key)
{
	m_Keys[key] = false;
}

GameState Game::GetState()
{
	return m_State;
}
