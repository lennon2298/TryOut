#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

class GameLevel {
public:
	std::vector<GameObject> m_Bricks;

	GameLevel() {};

	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	void Draw(SpriteRenderer &renderer);
	bool isCompleted();

private:
	void Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
};
