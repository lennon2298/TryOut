#pragma once
#include <glm/geometric.hpp>

#include "SpriteRenderer.h"
#include "ErrorHandling.h"
#include "ResourceManager.h"

enum class GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
private:
	unsigned int m_Width, m_Height;
	bool m_keys[1024];
	GameState m_State;
public:
	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();

	void ProcessInput(float dt);
	void Render();
	void Update(float dt);

	void SetState(GameState state);
	void SetKeyPress(int key);
	void SetKeyRelease(int key);
	GameState GetState();

};
