#include "GameLevel.h"

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
	m_Bricks.clear();

	unsigned int tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;
	if (fstream)
	{
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode)
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->Init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (GameObject& tile : this->m_Bricks)
		if (!tile.IsDestroyed())
			tile.Draw(renderer);
}

void GameLevel::DrawInstanced(SpriteRenderer& renderer)
{
	glActiveTexture(GL_TEXTURE0);
	ResourceManager::GetTexture("block_solid").Bind();
	glm::vec2 size(unit_width - 1.0, unit_height);
	renderer.DrawInstanced(std::size(translationsSolid), translationsSolid[0], size, glm::vec3(0.8f, 0.8f, 0.7f));
	//std::cout << std::size(translationsSolid) << std::endl;
	//std::cout << iterator << std::endl;
	//glActiveTexture(GL_TEXTURE0);
	//ResourceManager::GetTexture("block").Bind();
	//renderer.DrawInstanced(oneCount, *translationsOne, size, glm::vec3(0.2f, 0.6f, 1.0f));
	//renderer.DrawInstanced(oneCount, *translationsTwo, size, glm::vec3(0.0f, 0.7f, 0.0f));
	//renderer.DrawInstanced(oneCount, *translationsThree, size, glm::vec3(0.8f, 0.8f, 0.4f));
	//renderer.DrawInstanced(oneCount, *translationsFour, size, glm::vec3(1.0f, 0.5f, 0.0f));
}

bool GameLevel::isCompleted()
{
	for (GameObject& tile : this->m_Bricks)
		if (!tile.IsSolid() && !tile.IsDestroyed())
			return false;
	return true;
}

void GameLevel::Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();
	unit_width = levelWidth / static_cast<float>(width);
	unit_height = levelHeight / height;

	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			if (tileData[y][x] == 1)
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width - 1.0, unit_height);
				GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"), iterator, glm::vec3(0.8f, 0.8f, 0.7f));
				obj.SetSolid();
				this->m_Bricks.push_back(obj);
				translationsSolid.push_back(glm::vec4(pos, 0, 0));
				//solidCount++;
				//iterator++;
			}
			else if(tileData[y][x] > 1) {
				glm::vec2 pos(unit_width * x, unit_height * y);

				glm::vec3 color = glm::vec3(1.0f); // original: white
				if (tileData[y][x] == 2) {
					color = glm::vec3(0.2f, 0.6f, 1.0f);
					translationsSolid.push_back(glm::vec4(pos, 0, 2));
				}
				else if (tileData[y][x] == 3) {
					color = glm::vec3(0.0f, 0.7f, 0.0f);
					translationsSolid.push_back(glm::vec4(pos, 0, 3));
				}
				else if (tileData[y][x] == 4) {
					color = glm::vec3(0.8f, 0.8f, 0.4f);
					translationsSolid.push_back(glm::vec4(pos, 0, 4));
				}
				else if (tileData[y][x] == 5) {
					color = glm::vec3(1.0f, 0.5f, 0.0f);
					translationsSolid.push_back(glm::vec4(pos, 0, 5));
				}

				glm::vec2 size(unit_width - 1, unit_height);
				GameObject obj(pos, size, ResourceManager::GetTexture("block"), iterator, color);
				//iterator++;
				this->m_Bricks.push_back(obj);
			}
		}
	}
}
