#include "pch.h"
#include "BossManager.h"
#include "Player.h"
#include"Level.h"
#include "Sprite.h"
// include all boss classes
#include "Boss.h"
#include "HellHound.h"


#include <iostream>


BossManager::BossManager()
	: m_IsPlayerInBattle{false}
	, m_CurrentActiveBossHealth{0.f}
	, m_CurrentActiveBossHealthMaximum{0.f}
{
	m_CanPlayerTakeDamage = true;
}

BossManager::~BossManager()
{
	for (size_t levelIndex{ 0 }; levelIndex < m_pBosses.size(); ++levelIndex)
	{
		for (size_t stageIndex{ 0 }; stageIndex < m_pBosses[levelIndex].size(); ++stageIndex)
		{
			for (size_t enemyIndex{ 0 }; enemyIndex < m_pBosses[levelIndex][stageIndex].size(); ++enemyIndex)
			{
				delete m_pBosses[levelIndex][stageIndex][enemyIndex];
				m_pBosses[levelIndex][stageIndex][enemyIndex] = nullptr;

			}
		}
	}
}

void BossManager::Update(float elapsedSec, Player* pPlayer, Level* pCurrentLevel, int currentLevel, int currentStage)
{
	//for (int bossIndex{ 0 }; bossIndex < m_pBosses[currentLevel][currentStage].size(); ++bossIndex)
	//std::cout << m_pBosses[currentLevel][currentStage].size() << std::endl;
	for (size_t bossIndex{ 0 }; bossIndex < m_pBosses[currentLevel][currentStage].size(); ++bossIndex)
	{
		Boss* pCurrentBoss{ m_pBosses[currentLevel][currentStage][bossIndex] };

		pCurrentBoss->Update(elapsedSec, pPlayer->GetPlayerPos());
		if (pCurrentLevel->IsLevelInTransition() == true)
		{
			pCurrentBoss->GetVelocity().x = 0.f;
		}
		pCurrentBoss->IsHit(pPlayer->GetPlayerAttackHitBox(), pPlayer->GetPlayerDamage());
		if (pCurrentBoss->IsDead() == true)
		{
			pPlayer->AddScore(pCurrentBoss->GetValue());
		}
		if (pCurrentBoss->HitPlayer(pPlayer->GetPlayerHitBox()) == true && m_CanPlayerTakeDamage == true)
		{
			pPlayer->IsHit(pCurrentBoss->GetPointOfContact(), pCurrentBoss->GetDamage());
		}
		pCurrentLevel->HandleCollision(pCurrentBoss->GetHitBox(), pCurrentBoss->GetVelocity());

		if (pCurrentBoss->GetCurrentState() != Boss::ActionState::none && pCurrentBoss->HasGivenPoints())
		{
			m_IsPlayerInBattle = false;
		}
		else if (pCurrentBoss->GetCurrentState() != Boss::ActionState::none && pCurrentBoss->HasGivenPoints() == false)
		{
			m_IsPlayerInBattle = true;
			m_CurrentActiveBossHealth = pCurrentBoss->GetCurrentHealth();
			m_CurrentActiveBossHealthMaximum = pCurrentBoss->GetMaximumHealth();
		}
		else
		{
			m_IsPlayerInBattle = false;
		}
	}

}

void BossManager::Draw(int currentLevel, int currentStage) const
{
	for (size_t bossIndex{ 0 }; bossIndex < m_pBosses[currentLevel][currentStage].size(); ++bossIndex)
	{
		m_pBosses[currentLevel][currentStage][bossIndex]->Draw();
	}
}

void BossManager::InitializeBosses(TextureManager* pTextureManager, SoundManager* pSoundManager)
{
	//initialize level 1
	{
		std::vector<std::vector<Boss*>> vectorLevel1;
		// Stage 1-1
		{
			std::vector<Boss*> pBosses1_1;
			// no bosses in 1_1, just and empty vector

			vectorLevel1.push_back(std::move(pBosses1_1));
		}
		// Stage 1-2
		{
			std::vector<Boss*> pBosses1_2;
			// no bosses in 1_2, just and empty vector


			vectorLevel1.push_back(std::move(pBosses1_2));
		}
		// Stage 1-3
		{
			std::vector<Boss*> pBosses1_3;
			// no bosses in 1_3, just and empty vector


			vectorLevel1.push_back(std::move(pBosses1_3));
		}
		// Stage 1-4
		{
			std::vector<Boss*> pBosses1_4;
			// no bosses in 1_4, just and empty vector


			vectorLevel1.push_back(std::move(pBosses1_4));
		}
		// Stage 1-5
		{
			std::vector<Boss*> pBosses1_5;
			{
				pBosses1_5.push_back(new HellHound(879.f, pTextureManager, pSoundManager));
			}

			vectorLevel1.push_back(std::move(pBosses1_5));
		}
		// Stage 1-6-1
		{
			std::vector<Boss*> pBosses1_6;
			// no bosses in 1_6-1, just and empty vector

			vectorLevel1.push_back(std::move(pBosses1_6));
		}
		// Stage 1-6-2
		{
			std::vector<Boss*> pBosses1_6;
			// no bosses in 1_6-1, just and empty vector

			vectorLevel1.push_back(std::move(pBosses1_6));
		}
		m_pBosses.push_back(std::move(vectorLevel1));
	}
}

bool BossManager::IsPlayerInBattle() const
{
	return m_IsPlayerInBattle;
}

float BossManager::GetBossCurrentHealth()
{
	return m_CurrentActiveBossHealth;
}

float BossManager::GetBossMaximumHealth()
{
	return m_CurrentActiveBossHealthMaximum;
}


void BossManager::PlayBossMusic(int currentLevel, int currentStage)
{
	for (size_t bossIndex{ 0 }; bossIndex < m_pBosses[currentLevel][currentStage].size(); ++bossIndex)
	{
		return m_pBosses[currentLevel][currentStage][bossIndex]->PlayBossMusic();
	}
}

void BossManager::StopBossMusic(int currentLevel, int currentStage)
{
	for (size_t bossIndex{ 0 }; bossIndex < m_pBosses[currentLevel][currentStage].size(); ++bossIndex)
	{
		return m_pBosses[currentLevel][currentStage][bossIndex]->StopBossMusic();
	}
}
