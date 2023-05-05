#include "pch.h"
#include "ObjectManager.h"
#include "Player.h"
#include "DropManager.h"

// include all objects classes
#include "Object.h"
#include "Candle.h"
#include "Raven.h"
#include "RandomSkull.h"
#include "Door.h"
#include "BreakableWall.h"
#include "InvisibleWall.h"
#include "HangingCorpse.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	for (size_t levelIndex{ 0 }; levelIndex < m_pObjects.size(); ++levelIndex)
	{
		for (size_t stageIndex{ 0 }; stageIndex < m_pObjects[levelIndex].size(); ++stageIndex)
		{
			for (size_t objectIndex{ 0 }; objectIndex < m_pObjects[levelIndex][stageIndex].size(); ++objectIndex)
			{
				delete m_pObjects[levelIndex][stageIndex][objectIndex];
				m_pObjects[levelIndex][stageIndex][objectIndex] = nullptr;

			}
		}
	}
}

void ObjectManager::Update(float elapsedSec, int currentLevel, int currentStage, Player* pPlayer, DropManager* pDropManager, TextureManager* pTextureManager, SoundManager* pSoundManager, bool isPlayerInBattle)
{
	for (size_t objectIndex{0}; objectIndex < m_pObjects[currentLevel][currentStage].size(); ++objectIndex)
	{
		Object* pCurrentObject{m_pObjects[currentLevel][currentStage][objectIndex]};
		pCurrentObject->Update(elapsedSec, pPlayer->GetPlayerHitBox(), pPlayer->GetPlayerVelocity(), isPlayerInBattle);
		pCurrentObject->IsHit(pPlayer->GetPlayerAttackHitBox(), pPlayer->GetPlayerHitBox(), pPlayer->GetPlayerLevel(), pDropManager, pTextureManager, pSoundManager);
	}
}

void ObjectManager::Draw(int currentLevel, int currentStage) const
{
	for (size_t objectIndex{ 0 }; objectIndex < m_pObjects[currentLevel][currentStage].size(); ++objectIndex)
	{
		m_pObjects[currentLevel][currentStage][objectIndex]->Draw();
	}
}

void ObjectManager::InitializeObjects(TextureManager* pTextureManager, SoundManager* pSoundManager)
{
	//initialize level 1
	{
		std::vector<std::vector<Object*>> vectorLevel1;
		// Stage 1-1
		{
			std::vector<Object*> pObjects1_1;
			{
				pObjects1_1.push_back(new Candle(Point2f{ 165.f, 59.f }, Candle::Type::Outside, Drop::DropType::coatOfArms, 0, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Candle(Point2f{ 205.f, 89.f }, Candle::Type::Outside, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Candle(Point2f{ 300.f, 59.f }, Candle::Type::Outside, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Candle(Point2f{ 425.f, 129.f }, Candle::Type::Outside, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Candle(Point2f{ 550.f, 94.f }, Candle::Type::Outside, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Candle(Point2f{ 675.f, 74.f }, Candle::Type::Outside, Drop::DropType::coatOfArms, 0, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Candle(Point2f{ 740.f, 104.f }, Candle::Type::Outside, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Candle(Point2f{ 825.f, 104.f }, Candle::Type::Outside, Drop::DropType::bag, 200, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Candle(Point2f{ 960.f, 79.f }, Candle::Type::Outside, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));

				pObjects1_1.push_back(new Raven(Point2f{ 220.f, 163.f }, true, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Raven(Point2f{ 475.f, 163.f }, false, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Raven(Point2f{ 775.f, 55.f }, false, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Raven(Point2f{ 790.f, 55.f }, false, pTextureManager, pSoundManager));
				pObjects1_1.push_back(new Raven(Point2f{ 807.f, 55.f }, false, pTextureManager, pSoundManager));

				pObjects1_1.push_back(new RandomSkull(Point2f{ 70.f, 150.f }, pTextureManager));
			}

			vectorLevel1.push_back(std::move(pObjects1_1));
		}
		// Stage 1-2
		{
			std::vector<Object*> pObjects1_2;
			{
				pObjects1_2.push_back(new Candle(Point2f{ 84.f, 42.f }, Candle::Type::InsideRand, Drop::DropType::specialCoatOfArms, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 163.f, 79.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 275.f, 79.f }, Candle::Type::InsideWeapon, Drop::DropType::boomerang, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 355.f, 42.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 575.f, 144.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 635.f, 164.f }, Candle::Type::InsideRand, Drop::DropType::orb, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 728.f, 139.f }, Candle::Type::InsideRand, Drop::DropType::specialCoatOfArms, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 852.f, 104.f }, Candle::Type::InsideWeapon, Drop::DropType::battleAxe, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 540.f, 42.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 680.f, 42.f }, Candle::Type::InsideRand, Drop::DropType::bag, 500, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 931.f, 42.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 1140.f, 159.f }, Candle::Type::InsideRand, Drop::DropType::specialCoatOfArms, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Candle(Point2f{ 1230.f, 159.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_2.push_back(new Door(Point2f{ 1290.f, 129.f }, pTextureManager, pSoundManager)); // trigger at 1290.f, 160.f
			}

			vectorLevel1.push_back(std::move(pObjects1_2));
		}
		// Stage 1-3
		{
			std::vector<Object*> pObjects1_3;
			{
				pObjects1_3.push_back(new Door(Point2f{ -5.f, 129.f }, pTextureManager, pSoundManager));
				pObjects1_3.push_back(new Candle(Point2f{ 58.f, 160.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_3.push_back(new Candle(Point2f{ 190.f, 139.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_3.push_back(new Candle(Point2f{ 30.f, 69.f }, Candle::Type::InsideRand, Drop::DropType::bag, 500, pTextureManager, pSoundManager));
				pObjects1_3.push_back(new Candle(Point2f{ 443.f, 53.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_3.push_back(new Candle(Point2f{ 415.f, 149.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_3.push_back(new Candle(Point2f{ 635.f, 160.f }, Candle::Type::InsideRand, Drop::DropType::bag, 100, pTextureManager, pSoundManager));
				pObjects1_3.push_back(new Door(Point2f{ 698.f, 129.f }, pTextureManager, pSoundManager));
				pObjects1_3.push_back(new BreakableWall(Point2f{ 242.f, 17.f }, BreakableWall::WallType::pillar, 3, Drop::DropType::spellBook, pTextureManager, pSoundManager));
			}

			vectorLevel1.push_back(std::move(pObjects1_3));
		}
		// Stage 1-4
		{
			std::vector<Object*> pObjects1_4;
			{
				pObjects1_4.push_back(new Candle(Point2f{ 170.f, 135.f }, Candle::Type::InsideRand, Drop::DropType::specialCoatOfArms, 0, pTextureManager, pSoundManager));
				pObjects1_4.push_back(new Candle(Point2f{ 267.f, 109.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_4.push_back(new Candle(Point2f{ 365.f, 97.f }, Candle::Type::InsideRand, Drop::DropType::mirrorOfThruth, 0, pTextureManager, pSoundManager));
				pObjects1_4.push_back(new Candle(Point2f{ 470.f, 97.f }, Candle::Type::InsideRand, Drop::DropType::bag, 1000, pTextureManager, pSoundManager));
				pObjects1_4.push_back(new Candle(Point2f{ 620.f, 109.f }, Candle::Type::InsideWeapon, Drop::DropType::battleAxe, 0, pTextureManager, pSoundManager));
				pObjects1_4.push_back(new Candle(Point2f{ 495.f, 155.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
			}

			vectorLevel1.push_back(std::move(pObjects1_4));
		}
		// Stage 1-5
		{
			std::vector<Object*> pObjects1_5;
			{
				pObjects1_5.push_back(new Door(Point2f{ -5.f, 129.f }, pTextureManager, pSoundManager));
				pObjects1_5.push_back(new Candle(Point2f{ 85.f, 159.f }, Candle::Type::InsideRand, Drop::DropType::specialCoatOfArms, 0, pTextureManager, pSoundManager));
				pObjects1_5.push_back(new Candle(Point2f{ 170.f, 140.f }, Candle::Type::InsideRand, Drop::DropType::blueJewel, 0, pTextureManager, pSoundManager));
				pObjects1_5.push_back(new Candle(Point2f{ 255.f, 112.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_5.push_back(new Candle(Point2f{ 343.f, 81.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_5.push_back(new Candle(Point2f{ 383.f, 81.f }, Candle::Type::InsideWeapon, Drop::DropType::sacredWater, 0, pTextureManager, pSoundManager));
				pObjects1_5.push_back(new Candle(Point2f{ 470.f, 43.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_5.push_back(new Candle(Point2f{ 510.f, 43.f }, Candle::Type::InsideRand, Drop::DropType::coatOfArms, 0, pTextureManager, pSoundManager));
				pObjects1_5.push_back(new Candle(Point2f{ 600.f, 81.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_5.push_back(new Candle(Point2f{ 640.f, 81.f }, Candle::Type::InsideWeapon, Drop::DropType::boomerang, 0, pTextureManager, pSoundManager));
				pObjects1_5.push_back(new InvisibleWall(Point2f{ 720.f, 0.f }, Point2f{ 720.f, 224.f }));
				pObjects1_5.push_back(new InvisibleWall(Point2f{ 1040.f, 0.f }, Point2f{ 1040.f, 224.f }));
			}

			vectorLevel1.push_back(std::move(pObjects1_5));
		}
		// Stage 1-6-1
		{
			std::vector<Object*> pObjects1_6;
			{
				pObjects1_6.push_back(new Candle(Point2f{ 89.f, 42.f }, Candle::Type::InsideRand, Drop::DropType::bag, 200, pTextureManager, pSoundManager));
				pObjects1_6.push_back(new Candle(Point2f{ 50.f, 151.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_6.push_back(new Candle(Point2f{ 215.f, 151.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_6.push_back(new Candle(Point2f{ 262.f, 58.f }, Candle::Type::InsideWeapon, Drop::DropType::battleAxe, 0, pTextureManager, pSoundManager));
				pObjects1_6.push_back(new BreakableWall(Point2f{ 288.f, 112.f }, BreakableWall::WallType::wall, 4, Drop::DropType::muttonChop, pTextureManager, pSoundManager));
			}

			vectorLevel1.push_back(std::move(pObjects1_6));
		}
		// Stage 1-6-2
		{
			std::vector<Object*> pObjects1_6;
			{
				pObjects1_6.push_back(new HangingCorpse(Point2f{100.f, 144.f}, true, pTextureManager));
				pObjects1_6.push_back(new HangingCorpse(Point2f{175.f, 144.f}, false, pTextureManager));
				pObjects1_6.push_back(new HangingCorpse(Point2f{350.f, 144.f}, true, pTextureManager));
				pObjects1_6.push_back(new HangingCorpse(Point2f{515.f, 144.f}, true, pTextureManager));
				pObjects1_6.push_back(new Candle(Point2f{ 153.f, 42.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_6.push_back(new Candle(Point2f{ 283.f, 72.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_6.push_back(new Candle(Point2f{ 412.f, 42.f }, Candle::Type::InsideRand, Drop::DropType::bag, 500, pTextureManager, pSoundManager));
				pObjects1_6.push_back(new Candle(Point2f{ 586.f, 102.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_6.push_back(new Candle(Point2f{ 847.f, 137.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
				pObjects1_6.push_back(new Candle(Point2f{ 997.f, 137.f }, Candle::Type::InsideRand, Drop::DropType::redJewel, 0, pTextureManager, pSoundManager));
			}

			vectorLevel1.push_back(std::move(pObjects1_6));
		}
		m_pObjects.push_back(std::move(vectorLevel1));
	}
}