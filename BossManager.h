#pragma once
#include <vector>
#include "SoundManager.h"
class Boss;
class Player;
class Level;
class TextureManager;
class BossManager final
{
public:
	BossManager();
	BossManager(const BossManager& other) = delete;
	BossManager& operator=(const BossManager& other) = delete;
	BossManager(BossManager&& other) = delete;
	BossManager& operator=(BossManager&& other) = delete;
	~BossManager();

	void Update(float elapsedSec, Player* pPlayer, Level* pCurrentLevel, int currentLevel, int currentStage);

	void Draw(int currentLevel, int currentStage) const;

	void InitializeBosses(TextureManager* pTextureManager, SoundManager* pSoundManager);

	bool IsPlayerInBattle() const;

	float GetBossCurrentHealth();
	float GetBossMaximumHealth();


	void PlayBossMusic(int currentLevel, int currentStage);
	void StopBossMusic(int currentLevel, int currentStage);
	

private:
	std::vector<std::vector<std::vector<Boss*>>> m_pBosses;
	// expect stages to be empty

	bool m_CanPlayerTakeDamage; // for debug purpose only

	bool m_IsPlayerInBattle;
	float m_CurrentActiveBossHealth;
	float m_CurrentActiveBossHealthMaximum;

};

