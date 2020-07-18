#include "Game.h"

SpriteRenderer *spriteRenderer;

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

const glm::vec2 INITIAL_BALL_VELOCITY(60.0f, -150.0f);
const float BALL_RADIUS = 12.5f;

GameObject *Player;
BallObject* Ball;

bool checkCollisions(GameObject& one, GameObject& two);

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
	(void)ResourceManager::LoadTexture("./Resources/Textures/ball.png", true, "ball");
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
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));
}

void Game::ProcessInput(float dt)
{
	if (this->m_State == GameState::GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		// move playerboard
		if (m_Keys[GLFW_KEY_A] || m_Keys[GLFW_KEY_LEFT])
		{
			if (Player->GetPosition() >= 0.0f) {
				Player->UpdateVelocity(-velocity);
				if (Ball->m_Stuck) {
					Ball->UpdateVelocity(-velocity);
				}
			}
		}
		if (m_Keys[GLFW_KEY_D] || m_Keys[GLFW_KEY_RIGHT])
		{
			if (Player->GetPosition() <= this->m_Width - Player->GetSize()) {
				Player->UpdateVelocity(velocity);
				if (Ball->m_Stuck) {
					Ball->UpdateVelocity(velocity);
				}
			}
		}
		if (m_Keys[GLFW_KEY_SPACE])
		{
			Ball->m_Stuck = false;
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
		Ball->Draw(*spriteRenderer);
	}
}

void Game::Update(float dt)
{
	Ball->Move(dt, this->m_Width);
	DoCollisions();
}

void Game::DoCollisions()
{
	for (GameObject& box : Levels[level].m_Bricks) {
		if (!box.IsDestroyed()) {
			if (checkCollisions(*Ball, box)) {
				if (!box.IsSolid()) {
					box.Destroy();
				}
			}
		}
	}
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

bool checkCollisions(GameObject& one, GameObject& two) {
	bool collisionX = one.m_Position.x + one.m_Size.x >= two.m_Position.x && two.m_Position.x + two.m_Size.x >= one.m_Position.x;
	bool collisionY = one.m_Position.y + one.m_Size.y >= two.m_Position.y && two.m_Position.y + two.m_Size.y >= one.m_Position.y;

	return collisionX && collisionY;
}

bool checkCollisions(BallObject& one, GameObject& two) {
	glm::vec2 center(one.m_Position + one.m_Radius);

	glm::vec2 aabb_half_extents(
		two.m_Size.x / 2, two.m_Size.y / 2
	);

	glm::vec2 aabb_center(
		two.m_Position.x + aabb_half_extents.x,
		two.m_Position.y + aabb_half_extents.y
	);

	glm::vec2 difference = center - aabb_center;

	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;
	difference = closest - center;

	return glm::length(difference) < one.m_Radius;
}
