#pragma once
#include <vector>
class Enemy;
class Player;
class Level;
class TextureManager;
class SoundManager;

class EnemyManager final
{
public:
	EnemyManager();
	EnemyManager(const EnemyManager& other) = delete;
	EnemyManager& operator=(const EnemyManager& other) = delete;
	EnemyManager(EnemyManager&& other) = delete;
	EnemyManager& operator=(EnemyManager&& other) = delete;
	~EnemyManager();

	void Update(float elapsedSec, Player* pPlayer, Level* pCurrentLevel, int currentLevel, int currentStage);

	void Draw(int currentLevel, int currentStage) const;

	void InitializeEnemies(TextureManager* pTextureManager, SoundManager* pSoundManager);

private:
	std::vector<std::vector<std::vector<Enemy*>>> m_pEnemies;

	bool m_CanPlayerTakeDamage; // for debug purpose only

};

