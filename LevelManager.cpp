#include "pch.h"
#include "Camera.h"
#include "Player.h"
#include "LevelManager.h"
#include "DropManager.h"
#include "ObjectManager.h"
#include "EnemyManager.h"
#include "BossManager.h"

#include "SoundStream.h"


LevelManager::LevelManager()
	: m_IsLevelInBossBattle{false}
	, m_HasStoppedMusic{false}
	, m_CanPlayLevelMusic{true}
{
	
}

LevelManager::~LevelManager()
{
	for (size_t levelIndex{0}; levelIndex < m_pLevels.size(); ++levelIndex)
	{
		for (size_t stageIndex{0}; stageIndex < m_pLevels[levelIndex].size(); ++stageIndex)
		{
			delete m_pLevels[levelIndex][stageIndex];
			m_pLevels[levelIndex][stageIndex] = nullptr;
		}
	}
}

void LevelManager::Update(float elapsedSec, Player* pPlayer, DropManager* pDropManager, EnemyManager* pEnemyManager, BossManager* pBossManager, ObjectManager* pObjectManager, int currentLevel, int currentStage, TextureManager* pTextureManager, SoundManager* pSoundManager)
{
	Level* pCurrentLevel{ m_pLevels[currentLevel][currentStage] };
	//m_pLevels[currentLevel][currentStage]->Update(elapsedSec, pPlayer, pDropManager, pTextureManager);
	pCurrentLevel->Update(elapsedSec, pPlayer);

	pEnemyManager->Update(elapsedSec, pPlayer, pCurrentLevel, currentLevel, currentStage);

	// update only when not in transition
	
	pBossManager->Update(elapsedSec, pPlayer, pCurrentLevel, currentLevel, currentStage);
	

	m_IsLevelInBossBattle = pBossManager->IsPlayerInBattle();

	m_CanPlayLevelMusic = !(m_IsLevelInBossBattle);

	pObjectManager->Update(elapsedSec, currentLevel, currentStage, pPlayer, pDropManager, pTextureManager, pSoundManager, m_IsLevelInBossBattle);

	if (pDropManager->Size() > 0)
	{
		pDropManager->Update(elapsedSec, pCurrentLevel);
		pDropManager->HitItem(pPlayer->GetPlayerHitBox(), pPlayer);
	}

	// check if current level soundstream is being played...

	SoundStream* pCurrentSoundStreamLevel{ m_pLevelBackgroundMusic[currentLevel] };
	if (m_CanPlayLevelMusic == true)
	{
		if (m_HasStoppedMusic == false)
		{
			if (pCurrentSoundStreamLevel->IsPlaying() == false)
			{
				pCurrentSoundStreamLevel->Play(true);
			}
		}
		else
		{
			pBossManager->StopBossMusic(currentLevel, currentStage);
			m_HasStoppedMusic = false;
		}
		
	}
	else
	{
		if (m_HasStoppedMusic == false)
		{
			if (pCurrentSoundStreamLevel->IsPlaying() == true)
			{
				pCurrentSoundStreamLevel->Stop();
			}
			m_HasStoppedMusic = true;
		}
		else
		{
			pBossManager->PlayBossMusic(currentLevel, currentStage);
		}
	}
	
}



bool LevelManager::CheckTransitionTriggered(int currentLevel, int currentStage)
{
	if (m_IsLevelInBossBattle == true)
	{
		// when in boss battle, we don't want to trigger any transitions, the player should be stuck
		return false;
	}
	return m_pLevels[currentLevel][currentStage]->IsLevelInTransition();
}

Level::TransitionType LevelManager::GetTransitionType(int currentLevel, int currentStage)
{
	return m_pLevels[currentLevel][currentStage]->GetTransitionType();
}

Point2f LevelManager::GetNewPlayerPosition(int currentLevel, int currentStage)
{
	return m_pLevels[currentLevel][currentStage]->GetTransitionNewPosition();
}

int LevelManager::GetNextStageIndex(int currentLevel, int currentStage)
{
	return m_pLevels[currentLevel][currentStage]->GetTransitionNextStage();
}

void LevelManager::ResetTransition(int currentLevel, int currentStage)
{
	m_pLevels[currentLevel][currentStage]->ResetTransition();
}



void LevelManager::DrawLevelLayers(int level, int stage, Sprite::SpriteType typeOfLayer) const
{
	m_pLevels[level][stage]->DrawLayers(typeOfLayer);
}

void LevelManager::DrawCurrentLevel(int currentLevel, int currentStage, Camera* pCamera, Player* pPlayer, DropManager* pDropManager, EnemyManager* pEnemyManager, BossManager* pBossManager, ObjectManager* pObjectManager) const
{
	// background layer
	glPushMatrix();
	{
		pCamera->TransformParallax(GetBoundaries(currentLevel, currentStage, Sprite::SpriteType::BackLayer), false); // need to add a way to get y-axis parallax for 1-1
		DrawLevelLayers(currentLevel, currentStage, Sprite::SpriteType::BackLayer);
	}
	glPopMatrix();

	// mid layer -> all actions (objects + enemies)
	glPushMatrix();
	{
		pCamera->TransformMid();
		DrawLevelLayers(currentLevel, currentStage, Sprite::SpriteType::MidLayer);

		if (pDropManager->Size() > 0)
		{
			pDropManager->Draw();
		}

		pObjectManager->Draw(currentLevel, currentStage);

		pEnemyManager->Draw(currentLevel, currentStage);

		pBossManager->Draw(currentLevel, currentStage);

		pPlayer->Draw();

	}
	glPopMatrix();

	// front layer 
	glPushMatrix();
	{
		pCamera->TransformParallax(GetBoundaries(currentLevel, currentStage, Sprite::SpriteType::FrontLayer), false);
		DrawLevelLayers(currentLevel, currentStage, Sprite::SpriteType::FrontLayer);
	}
	glPopMatrix();
}

void LevelManager::DrawDoorTransition(int currentLevel, int currentStage, Camera* pCamera, Player* pPlayer, DropManager* pDropManager, EnemyManager* pEnemyManager, ObjectManager* pObjectManager, float camShift, int nextStage) const
{
	
	// draw background of current stage
	glPushMatrix();
	{
		glTranslatef(-camShift, 0.f, 0.f);
		pCamera->TransformParallax(GetBoundaries(currentLevel, currentStage, Sprite::SpriteType::BackLayer), false); // need to add a way to get y-axis parallax for 1-1
		DrawLevelLayers(currentLevel, currentStage, Sprite::SpriteType::BackLayer);
	}
	glPopMatrix();

	// draw background of next stage
	glPushMatrix();
	{
		pCamera->TransformParallax(GetBoundaries(currentLevel, nextStage, Sprite::SpriteType::BackLayer), false); // need to add a way to get y-axis parallax for 1-1
		DrawLevelLayers(currentLevel, nextStage, Sprite::SpriteType::BackLayer);
	}
	glPopMatrix();

	// draw mid layer of next stage first -> door is opening on the current stage
	// so we want the current stage to be layed over the next stage
	glPushMatrix();
	{
		glTranslatef(-camShift, 0.f, 0.f);
		pCamera->TransformMid();
		glTranslatef(GetBoundaries(currentLevel, currentStage, Sprite::SpriteType::MidLayer).width, 0.f, 0.f);
		// we translate the next stage to be right next to the current stage
		DrawLevelLayers(currentLevel, nextStage, Sprite::SpriteType::MidLayer);

		pObjectManager->Draw(currentLevel, nextStage);

		pEnemyManager->Draw(currentLevel, nextStage);
	}
	glPopMatrix();

	// draw mid layer of current stage
	glPushMatrix();
	{
		glTranslatef(-camShift, 0.f, 0.f);
		pCamera->TransformMid();
		DrawLevelLayers(currentLevel, currentStage, Sprite::SpriteType::MidLayer);

		if (pDropManager->Size() > 0)
		{
			pDropManager->Draw();
		}
		pObjectManager->Draw(currentLevel, currentStage);

		pEnemyManager->Draw(currentLevel, currentStage);

		pPlayer->Draw();
	}
	glPopMatrix();


	// draw front layer of current stage
	glPushMatrix();
	{
		glTranslatef(-camShift, 0.f, 0.f);
		pCamera->TransformParallax(GetBoundaries(currentLevel, currentStage, Sprite::SpriteType::FrontLayer), false);
		DrawLevelLayers(currentLevel, currentStage, Sprite::SpriteType::FrontLayer);
	}
	glPopMatrix();

	// draw front layer of next stage
	glPushMatrix();
	{
		glTranslatef(-camShift, 0.f, 0.f);
		pCamera->TransformParallax(GetBoundaries(currentLevel, nextStage, Sprite::SpriteType::FrontLayer), false);
		DrawLevelLayers(currentLevel, nextStage, Sprite::SpriteType::FrontLayer);
	}
	glPopMatrix();

	
}


Rectf LevelManager::GetBoundaries(int level, int stage, Sprite::SpriteType typeOfLayer) const
{
	return m_pLevels[level][stage]->GetLayerBoundaries(typeOfLayer);
}

Level* LevelManager::GetCurrentLevel(int currentLevel, int currentStage)
{
	return m_pLevels[currentLevel][currentStage];
}

void LevelManager::StopLevelMusic(int currentLevel, int currentStage, BossManager* pBossManager)
{
	SoundStream* pCurrentSoundStreamLevel{ m_pLevelBackgroundMusic[currentLevel] };
	if (m_CanPlayLevelMusic == true)
	{
		//if (m_HasStoppedMusic == false)
		//{
		//	if (pCurrentSoundStreamLevel->IsPlaying() == false)
		//	{
		//		pCurrentSoundStreamLevel->Play(true);
		//	}
		//}
		//else
		//{
		//	pBossManager->StopBossMusic(currentLevel, currentStage);
		//	m_HasStoppedMusic = false;
		//}
		pCurrentSoundStreamLevel->Stop();
		m_HasStoppedMusic = false;
	}
	else
	{
		pBossManager->StopBossMusic(currentLevel, currentStage);
		m_CanPlayLevelMusic = true;
		m_HasStoppedMusic = false;

		//if (m_HasStoppedMusic == false)
		//{
		//	if (pCurrentSoundStreamLevel->IsPlaying() == true)
		//	{
		//		pCurrentSoundStreamLevel->Stop();
		//	}
		//	m_HasStoppedMusic = true;
		//}
		//else
		//{
		//	pBossManager->PlayBossMusic(currentLevel, currentStage);
		//}
	}
}



bool LevelManager::IsLevelInBattle() const
{
	return m_IsLevelInBossBattle;
}

void LevelManager::InitializeLevels(TextureManager* pTextureManager, SoundManager* pSoundManager)
{
	// Can be optimised by reading it from files...

	//initialize level 1
	{
		// background music
		m_pLevelBackgroundMusic.push_back(pSoundManager->GetSoundStreamPointer("Resources/Sounds/Streams/Level_1_Background_Music.mp3"));

		// all stages
		std::vector<Level*> vectorLevel1;
		// Stage 1-1
		{
			// initialize level sprites
			std::vector<Sprite*> pSprites1_1;
			{
				pSprites1_1.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-1/Stage1-1_MidLayer.png", 1, 3, 5, Point2f{ 0.f, 0.f }, Sprite::SpriteType::MidLayer, pTextureManager));
				pSprites1_1.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-1/Stage1-1_BackLayer.png", 1, 1, 1, Point2f{ 0.f, 15.f }, Sprite::SpriteType::BackLayer, pTextureManager));
				pSprites1_1.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-1/Stage1-1_FrontLayer.png", 1, 3, 6, Point2f{ 0.f, 0.f }, Sprite::SpriteType::FrontLayer, pTextureManager));
				pSprites1_1.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-1/Stage1-1_BackLayer_Sky1.png", 1, 1, 1, Point2f{ 0.f, 270.f }, Sprite::SpriteType::Cloud, 25.f, pTextureManager));
				pSprites1_1.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-1/Stage1-1_BackLayer_Sky2.png", 1, 1, 1, Point2f{ 0.f, 246.f }, Sprite::SpriteType::Cloud, 30.f, pTextureManager));
				pSprites1_1.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-1/Stage1-1_BackLayer_Sky3.png", 1, 1, 1, Point2f{ 0.f, 233.f }, Sprite::SpriteType::Cloud, 35.f, pTextureManager));
			}

			// initialize the SVG files for the normal ground and all platforms
			std::vector<std::string> SvgVector1_1;
			{
				SvgVector1_1.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-1/SVG/Stage1-1_MidLayer.svg");
				SvgVector1_1.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-1/SVG/Stage1-1_Platforms.svg");
			}

			// initialize the SVG files for the stairs
			std::vector<std::string> SvgStairVector1_1;
			{
				SvgStairVector1_1.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-1/SVG/Stage1-1_Stairs_Right.svg");
				SvgStairVector1_1.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-1/SVG/Stage1-1_Stairs_Left.svg");
			}
			// initialize wich way the stairs are facing
			std::vector<bool> SvgStairVectorDirection1_1;  // true if looking Right when up, false if left
			{
				SvgStairVectorDirection1_1.push_back(true);
				SvgStairVectorDirection1_1.push_back(false);
			}




			Level::Transition transition1_1;
			{
				std::vector<Point2f> triggerPos1_1;
				{
					triggerPos1_1.push_back(Point2f{ 1010.f, 64.f });
				}

				std::vector<Level::TransitionType> transitionType1_1;
				{
					transitionType1_1.push_back(Level::TransitionType::fadeToBlack);
				}

				std::vector<Point2f> newPos1_1;
				{
					newPos1_1.push_back(Point2f{ 20.f, 20.f });
				}

				// transition to stage makes it easier to give a stage to transition to
				std::vector<int> transitionToStage1_1;
				{
					transitionToStage1_1.push_back(1);
				}
				transition1_1.transitionTriggerPositions = triggerPos1_1;
				transition1_1.transitionTypes = transitionType1_1;
				transition1_1.newPosition = newPos1_1;
				transition1_1.nextStageIndex = transitionToStage1_1;

			}

			// add all to the stagesvector
			vectorLevel1.push_back(new Level(std::move(pSprites1_1), SvgVector1_1, SvgStairVector1_1, SvgStairVectorDirection1_1, transition1_1, 1));
		}
		// Stage 1-2
		{
			std::vector<Sprite*> pSprites1_2;
			{
				pSprites1_2.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-2/Stage1-2_MidLayer.png", 1, 1, 1, Point2f{ 0.f, 0.f }, Sprite::SpriteType::MidLayer, pTextureManager));
				pSprites1_2.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-2/Stage1-2_BackLayer.png", 1, 1, 1, Point2f{ 0.f, 95.f }, Sprite::SpriteType::BackLayer, pTextureManager));
				pSprites1_2.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-2/Stage1-2_BackLayer_Sky3.png", 1, 1, 1, Point2f{ 0.f, 130.f }, Sprite::SpriteType::Cloud, 35.f, pTextureManager));
				pSprites1_2.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-2/Stage1-2_BackLayer_Sky2.png", 1, 1, 1, Point2f{ 0.f, 141.f }, Sprite::SpriteType::Cloud, 30.f, pTextureManager));
				pSprites1_2.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-2/Stage1-2_BackLayer_Sky1.png", 1, 1, 1, Point2f{ 0.f, 165.f }, Sprite::SpriteType::Cloud, 25.f, pTextureManager));
			}

			std::vector<std::string> SvgVector1_2;
			std::vector<std::string> SvgStairVector1_2;
			std::vector<bool> SvgStairVectorDirection1_2;
			{
				SvgVector1_2.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-2/SVG/Stage1-2_MidLayer.svg");
				SvgVector1_2.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-2/SVG/Stage1-2_Platforms.svg");

				SvgStairVector1_2.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-2/SVG/Stage1-2_Stairs_Right.svg");
				SvgStairVector1_2.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-2/SVG/Stage1-2_Stairs_Left.svg");
	
				SvgStairVectorDirection1_2.push_back(true);
				SvgStairVectorDirection1_2.push_back(false);
			}


			Level::Transition transition1_2;
			{

				std::vector<Point2f> transitionPos1_2;
				std::vector<Level::TransitionType> transitionType1_2;
				std::vector<Point2f> transitionToPos1_2;
				std::vector<int> transitionToStage1_2;
				{
					transitionPos1_2.push_back(Point2f{ 1290.f, 160.f });

					transitionType1_2.push_back(Level::TransitionType::door);

					transitionToPos1_2.push_back(Point2f{ 20.f, 129.f });

					transitionToStage1_2.push_back(2);


					transition1_2.newPosition = transitionToPos1_2;
					transition1_2.nextStageIndex = transitionToStage1_2;
					transition1_2.transitionTypes = transitionType1_2;
					transition1_2.transitionTriggerPositions = transitionPos1_2;
				}
			}
			



			vectorLevel1.push_back(new Level(std::move(pSprites1_2), SvgVector1_2, SvgStairVector1_2, SvgStairVectorDirection1_2, transition1_2, 2)); // temp end pos
		}
		// Stage 1-3
		{
			std::vector<Sprite*> pSprites1_3;
			{
				pSprites1_3.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-3/Stage1-3_MidLayer.png", 1, 1, 1, Point2f{ 0.f, 0.f }, Sprite::SpriteType::MidLayer, pTextureManager));
			}

			std::vector<std::string> SvgVector1_3;
			std::vector<std::string> SvgStairVector1_3;
			std::vector<bool> SvgStairVectorDirection1_3;
			{
				SvgVector1_3.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-3/SVG/Stage1-3_MidLayer.svg");
				SvgVector1_3.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-3/SVG/Stage1-3_Platforms.svg");

				SvgStairVector1_3.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-3/SVG/Stage1-3_Stairs_Left.svg");

				SvgStairVectorDirection1_3.push_back(false);
			}


			Level::Transition transition1_3;
			{

			std::vector<Point2f> transitionPos1_3;
			std::vector<Level::TransitionType> transitionType1_3;
			std::vector<Point2f> transitionToPos1_3;
			std::vector<int> transitionToStage1_3;
				{
					transitionPos1_3.push_back(Point2f{ 55.f, 3.f });
					transitionPos1_3.push_back(Point2f{ 395.f, 3.f });
					transitionPos1_3.push_back(Point2f{ 698.f, 160.f });

					transitionType1_3.push_back(Level::TransitionType::fadeToBlack);
					transitionType1_3.push_back(Level::TransitionType::fadeToBlack);
					transitionType1_3.push_back(Level::TransitionType::door);

					transitionToPos1_3.push_back(Point2f{ 65.f, 165.f });
					transitionToPos1_3.push_back(Point2f{ 415.f, 165.f });
					transitionToPos1_3.push_back(Point2f{ 20.f, 129.f });

					transitionToStage1_3.push_back(3);
					transitionToStage1_3.push_back(3);
					transitionToStage1_3.push_back(4);

					transition1_3.newPosition = transitionToPos1_3;
					transition1_3.nextStageIndex = transitionToStage1_3;
					transition1_3.transitionTypes = transitionType1_3;
					transition1_3.transitionTriggerPositions = transitionPos1_3;
				}
			}
			

			vectorLevel1.push_back(new Level(std::move(pSprites1_3), SvgVector1_3, SvgStairVector1_3, SvgStairVectorDirection1_3, transition1_3, 3));
		}
		// Stage 1-4
		{
			std::vector<Sprite*> pSprites1_4;
			{
				pSprites1_4.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-4/Stage1-4_MidLayer.png", 1, 16, 30, Point2f{ 0.f, 0.f }, Sprite::SpriteType::MidLayer, pTextureManager));
				pSprites1_4.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-4/Stage1-4_BackLayer.png", 1, 1, 1, Point2f{ 0.f, 67.f }, Sprite::SpriteType::BackLayer, pTextureManager));
			}

			std::vector<std::string> SvgVector1_4;
			std::vector<std::string> SvgStairVector1_4;
			std::vector<bool> SvgStairVectorDirection1_4;
			{
				SvgVector1_4.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-4/SVG/Stage1-4_MidLayer.svg"); // just bottom of the screen, will be treated as a kill plane
				SvgVector1_4.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-4/SVG/Stage1-4_Platforms.svg");

				SvgStairVector1_4.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-4/SVG/Stage1-4_Stairs_Left.svg");

				SvgStairVectorDirection1_4.push_back(false);
			}


			Level::Transition transition1_4;
			{
				std::vector<Point2f> transitionPos1_4;
				std::vector<Level::TransitionType> transitionType1_4;
				std::vector<Point2f> transitionToPos1_4;
				std::vector<int> transitionToStage1_4;
				{
					transitionPos1_4.push_back(Point2f{ 65.f, 220.f }); 
					transitionPos1_4.push_back(Point2f{ 415.f, 220.f });

					transitionType1_4.push_back(Level::TransitionType::fadeToBlack);
					transitionType1_4.push_back(Level::TransitionType::fadeToBlack);

					transitionToPos1_4.push_back(Point2f{ 35.f, 8.f });
					transitionToPos1_4.push_back(Point2f{ 385.f, 10.f });

					transitionToStage1_4.push_back(2);
					transitionToStage1_4.push_back(2);

					transition1_4.newPosition = transitionToPos1_4;
					transition1_4.nextStageIndex = transitionToStage1_4;
					transition1_4.transitionTypes = transitionType1_4;
					transition1_4.transitionTriggerPositions = transitionPos1_4;
				}
			}


			vectorLevel1.push_back(new Level(std::move(pSprites1_4), SvgVector1_4, SvgStairVector1_4, SvgStairVectorDirection1_4, transition1_4, 4)); 
		}
		// Stage 1-5
		{
			std::vector<Sprite*> pSprites1_5;
			{
				pSprites1_5.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-5/Stage1-5_MidLayer.png", 1, 1, 1, Point2f{ 0.f, 0.f }, Sprite::SpriteType::MidLayer, pTextureManager));
				pSprites1_5.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-5/Stage1-5_BackLayer.png", 1, 1, 1, Point2f{ 0.f, 95.f }, Sprite::SpriteType::BackLayer, pTextureManager));
				// reuse the clouds from 1-2, this way we don't have to copy those files, wasting extra memory
				pSprites1_5.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-2/Stage1-2_BackLayer_Sky3.png", 1, 1, 1, Point2f{ 0.f, 130.f }, Sprite::SpriteType::Cloud, 35.f, pTextureManager));
				pSprites1_5.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-2/Stage1-2_BackLayer_Sky2.png", 1, 1, 1, Point2f{ 0.f, 141.f }, Sprite::SpriteType::Cloud, 30.f, pTextureManager));
				pSprites1_5.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-2/Stage1-2_BackLayer_Sky1.png", 1, 1, 1, Point2f{ 0.f, 165.f }, Sprite::SpriteType::Cloud, 25.f, pTextureManager));
			}


			std::vector<std::string> SvgVector1_5;
			std::vector<std::string> SvgStairVector1_5;
			std::vector<bool> SvgStairVectorDirection1_5;
			{
				SvgVector1_5.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-5/SVG/Stage1-5_MidLayer.svg"); // just bottom of the screen, will be treated as a kill plane
				SvgVector1_5.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-5/SVG/Stage1-5_Platforms.svg");

				// there are no stairs in 1-5, use empty vectors
			}


			Level::Transition transition1_5;
			{
				std::vector<Point2f> transitionPos1_5;
				std::vector<Level::TransitionType> transitionType1_5;
				std::vector<Point2f> transitionToPos1_5;
				std::vector<int> transitionToStage1_5;
				{
					transitionPos1_5.push_back(Point2f{ 1040.f, 35.f }); // currently out of bounds for testing

					transitionType1_5.push_back(Level::TransitionType::fadeToBlack);

					transitionToPos1_5.push_back(Point2f{ 8.f, 20.f });

					transitionToStage1_5.push_back(5);

					transition1_5.newPosition = transitionToPos1_5;
					transition1_5.nextStageIndex = transitionToStage1_5;
					transition1_5.transitionTypes = transitionType1_5;
					transition1_5.transitionTriggerPositions = transitionPos1_5;
				}
			}


			vectorLevel1.push_back(new Level(std::move(pSprites1_5), SvgVector1_5, SvgStairVector1_5, SvgStairVectorDirection1_5, transition1_5, 5)); // temp end pos

		}
		// Stage 1-6-1
		{
			std::vector<Sprite*> pSprites1_6;
			{
				pSprites1_6.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-6/Stage1-6_1_MidLayer.png", 1, 1, 1, Point2f{ 0.f, 0.f }, Sprite::SpriteType::MidLayer, pTextureManager));
			}

			std::vector<std::string> SvgVector1_6;
			std::vector<std::string> SvgStairVector1_6;
			std::vector<bool> SvgStairVectorDirection1_6;
			{
				SvgVector1_6.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-6/SVG_1-6-1/Stage1-6-1_MidLayer.svg");
				SvgVector1_6.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-6/SVG_1-6-1/Stage1-6-1_Platforms.svg");
				//SvgVector1_6.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-3/SVG/Stage1-3_Platforms.svg");

				SvgStairVector1_6.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-6/SVG_1-6-1/Stage1-6-1_Stairs_Left.svg");

				SvgStairVectorDirection1_6.push_back(false);
			}


			Level::Transition transition1_6;
			{

				std::vector<Point2f> transitionPos1_6;
				std::vector<Level::TransitionType> transitionType1_6;
				std::vector<Point2f> transitionToPos1_6;
				std::vector<int> transitionToStage1_6;
				{
					transitionPos1_6.push_back(Point2f{ 106.f, 220.f });
					
					transitionType1_6.push_back(Level::TransitionType::fadeToBlack);
					
					transitionToPos1_6.push_back(Point2f{ 50.f, 10.f });
					
					transitionToStage1_6.push_back(6);

					transition1_6.newPosition = transitionToPos1_6;
					transition1_6.nextStageIndex = transitionToStage1_6;
					transition1_6.transitionTypes = transitionType1_6;
					transition1_6.transitionTriggerPositions = transitionPos1_6;
				}
			}


			vectorLevel1.push_back(new Level(std::move(pSprites1_6), SvgVector1_6, SvgStairVector1_6, SvgStairVectorDirection1_6, transition1_6, 6));
		}
		// Stage 1-6-2
		{
			std::vector<Sprite*> pSprites1_6;
			{
				pSprites1_6.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-6/Stage1-6_2_MidLayer.png", 1, 1, 1, Point2f{ 0.f, 0.f }, Sprite::SpriteType::MidLayer, pTextureManager));
				pSprites1_6.push_back(new Sprite("Resources/Sprites/Levels/Stage 1/Stage 1-6/Stage1-6_BackLayer.png", 1, 1, 1, Point2f{ 0.f, 0.f }, Sprite::SpriteType::BackLayer, pTextureManager));
			}

			std::vector<std::string> SvgVector1_6;
			std::vector<std::string> SvgStairVector1_6;
			std::vector<bool> SvgStairVectorDirection1_6;
			{
				SvgVector1_6.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-6/SVG_1-6-2/Stage1-6-2_MidLayer.svg");
				SvgVector1_6.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-6/SVG_1-6-2/Stage1-6-2_Platforms.svg");

				SvgStairVector1_6.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-6/SVG_1-6-2/Stage1-6-2_Stairs_Right.svg");
				SvgStairVector1_6.push_back("Resources/Sprites/Levels/Stage 1/Stage 1-6/SVG_1-6-2/Stage1-6-2_Stairs_Left.svg");

				SvgStairVectorDirection1_6.push_back(true);
				SvgStairVectorDirection1_6.push_back(false);
			}


			Level::Transition transition1_6;
			{

				std::vector<Point2f> transitionPos1_6;
				std::vector<Level::TransitionType> transitionType1_6;
				std::vector<Point2f> transitionToPos1_6;
				std::vector<int> transitionToStage1_6;
				{
					transitionPos1_6.push_back(Point2f{ 70.f, 10.f });

					transitionType1_6.push_back(Level::TransitionType::fadeToBlack);

					transitionToPos1_6.push_back(Point2f{ 110.f, 170.f });

					transitionToStage1_6.push_back(5);


					transition1_6.newPosition = transitionToPos1_6;
					transition1_6.nextStageIndex = transitionToStage1_6;
					transition1_6.transitionTypes = transitionType1_6;
					transition1_6.transitionTriggerPositions = transitionPos1_6;
				}
			}




			vectorLevel1.push_back(new Level(std::move(pSprites1_6), SvgVector1_6, SvgStairVector1_6, SvgStairVectorDirection1_6, transition1_6, 6)); // temp end pos
		}
		m_pLevels.push_back(vectorLevel1);
	}
}