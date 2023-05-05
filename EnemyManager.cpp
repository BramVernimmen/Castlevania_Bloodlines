#include "pch.h"
#include "EnemyManager.h"
#include "Player.h"
#include"Level.h"
#include "Sprite.h"
// include all enemy classes
#include "Enemy.h"
#include "Zombie.h"
#include "PeepingEye.h"
#include "VampireBat.h"
#include "FlamingSkull.h"
#include "FishMan.h"




EnemyManager::EnemyManager()
{
	m_CanPlayerTakeDamage = true;
}

EnemyManager::~EnemyManager()
{
	for (size_t levelIndex{ 0 }; levelIndex < m_pEnemies.size(); ++levelIndex)
	{
		for (size_t stageIndex{ 0 }; stageIndex < m_pEnemies[levelIndex].size(); ++stageIndex)
		{
			for (size_t enemyIndex{0}; enemyIndex < m_pEnemies[levelIndex][stageIndex].size(); ++enemyIndex)
			{
				delete m_pEnemies[levelIndex][stageIndex][enemyIndex];
				m_pEnemies[levelIndex][stageIndex][enemyIndex] = nullptr;

			}
		}
	}
}

void EnemyManager::Update(float elapsedSec, Player* pPlayer, Level* pCurrentLevel, int currentLevel, int currentStage)
{
	for (size_t enemyIndex{0}; enemyIndex < m_pEnemies[currentLevel][currentStage].size(); ++enemyIndex)
	{
		Enemy* pCurrentEnemy{ m_pEnemies[currentLevel][currentStage][enemyIndex] };
		pCurrentEnemy->Update(elapsedSec, pPlayer->GetPlayerPos(), pCurrentLevel->GetLayerBoundaries(Sprite::SpriteType::MidLayer).width);
		if (pCurrentLevel->IsLevelInTransition() == true)
		{
			pCurrentEnemy->GetVelocity().x = 0.f;
		}
		pCurrentEnemy->IsHit(pPlayer->GetPlayerAttackHitBox(), pPlayer->GetPlayerDamage());
		if (pCurrentEnemy->IsDead() == true)
		{
			pPlayer->AddScore(pCurrentEnemy->GetValue());
		}
		if (pCurrentEnemy->HitPlayer(pPlayer->GetPlayerHitBox()) == true && m_CanPlayerTakeDamage == true)
		{
			pPlayer->IsHit(pCurrentEnemy->GetPointOfContact(), pCurrentEnemy->GetDamage());
		}
		if (pCurrentEnemy->GetCollideWithStage() == true)
		{
			pCurrentEnemy->SetOnGround(pCurrentLevel->IsOnGround(pCurrentEnemy->GetHitBox(), pCurrentEnemy->GetVelocity()));
			pCurrentLevel->HandleCollision(pCurrentEnemy->GetHitBox(), pCurrentEnemy->GetVelocity());
		}
	}

}

void EnemyManager::Draw(int currentLevel, int currentStage) const
{
	for (size_t enemyIndex{0}; enemyIndex < m_pEnemies[currentLevel][currentStage].size(); ++enemyIndex)
	{
		m_pEnemies[currentLevel][currentStage][enemyIndex]->Draw();
	}
}

void EnemyManager::InitializeEnemies(TextureManager* pTextureManager, SoundManager* pSoundManager)
{
		//initialize level 1
		{
			std::vector<std::vector<Enemy*>> vectorLevel1;
			// Stage 1-1
			{
				std::vector<Enemy*> pEnemies1_1;
				// no enemies in 1_1, just and empty vector

				vectorLevel1.push_back(std::move(pEnemies1_1));
			}
			// Stage 1-2
			{
				std::vector<Enemy*> pEnemies1_2;
				{
					pEnemies1_2.push_back(new Zombie(true, pTextureManager, pSoundManager));
					pEnemies1_2.push_back(new Zombie(true, pTextureManager, pSoundManager));
					pEnemies1_2.push_back(new Zombie(true, pTextureManager, pSoundManager));
					pEnemies1_2.push_back(new Zombie(true, pTextureManager, pSoundManager));
				}

				vectorLevel1.push_back(std::move(pEnemies1_2));
			}
			// Stage 1-3
			{
				std::vector<Enemy*> pEnemies1_3;
				{
					pEnemies1_3.push_back(new PeepingEye(Point2f{ 250.f, 130.f }, pTextureManager, pSoundManager));
				}

				vectorLevel1.push_back(std::move(pEnemies1_3));
			}
			// Stage 1-4
			{
				std::vector<Enemy*> pEnemies1_4;
				{
					pEnemies1_4.push_back(new FlamingSkull(Point2f{ 355.f, 120.f }, pTextureManager, pSoundManager));
					pEnemies1_4.push_back(new FishMan(true, pTextureManager, pSoundManager));
					pEnemies1_4.push_back(new FishMan(true, pTextureManager, pSoundManager));
				}

				vectorLevel1.push_back(std::move(pEnemies1_4));
			}
			// Stage 1-5
			{
				std::vector<Enemy*> pEnemies1_5;
				{
					pEnemies1_5.push_back(new VampireBat(true, Point2f{ 300.f, 50.f }, pTextureManager, pSoundManager));
				}

				vectorLevel1.push_back(std::move(pEnemies1_5));
			}
			// Stage 1-6
			{
				std::vector<Enemy*> pEnemies1_6;

				vectorLevel1.push_back(std::move(pEnemies1_6));
			}
			// Stage 1-6
			{
				std::vector<Enemy*> pEnemies1_6;

				vectorLevel1.push_back(std::move(pEnemies1_6));
			}
			m_pEnemies.push_back(std::move(vectorLevel1));
		}
}



