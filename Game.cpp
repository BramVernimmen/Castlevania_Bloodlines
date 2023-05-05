#include "pch.h"
#include "Game.h"
#include "Sprite.h"
#include "Camera.h"
#include "Object.h"
#include "Player.h"
#include "DropManager.h"
#include "LevelManager.h"
#include "EnemyManager.h"
#include "ObjectManager.h"
#include "BossManager.h"
#include "Hud.h"

#include "TextureManager.h"
#include "SoundManager.h"
#include "SoundEffect.h"

#include "Menu.h"

#include "utils.h"
#include <iostream>

Game::Game(const Window& window)
	:m_Window{ window }
	, m_CamWidth{320.f}
	, m_CurrentGameState{GameState::InMenu} // should be standard in menu
	, m_CurrentLevel{0}
	, m_CurrentStage{0}
	, m_CamShift{0.f}
	, m_CurrentLevelTransitionType{Level::TransitionType::none}
	, m_FadeToBlackColor{ Color4f{0.f,0.f,0.f,0.f} }
	, m_NextStage{0}
	, m_HasLevelStarted{false}
	, m_CurrentMenuType{MenuManager::MenuType::startScreen}
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	m_pTextureManager = new TextureManager();
	m_pSoundManager = new SoundManager();
	m_CamScale = m_Window.width / m_CamWidth;
	//m_pPlayer = new Player(Point2f{ 35.f, 100.f }, 3, 2 , m_pTextureManager);
	//m_pPlayer = new Player(Point2f{ 1200.f, 190.f });
	m_pLevelManager = new LevelManager();
	m_pEnemyManager = new EnemyManager();
	m_pBossManager = new BossManager();
	m_pObjectManager = new ObjectManager();
	m_pDropManager = new DropManager();

	m_pMenuManager = new MenuManager(m_pTextureManager, m_pSoundManager);
	m_pHud = new Hud(m_Window, m_CamScale, m_pTextureManager);
	m_pPauseSoundEffect = m_pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/Pause.mp3");
	m_pPlayerDeathSoundEffect = m_pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/PlayerDeath.mp3");
	m_pCamera = new Camera(m_CamWidth, 224.f, m_CamScale);

	m_pLevelManager->InitializeLevels(m_pTextureManager, m_pSoundManager);
	m_pEnemyManager->InitializeEnemies(m_pTextureManager, m_pSoundManager);
	m_pBossManager->InitializeBosses(m_pTextureManager, m_pSoundManager);
	m_pObjectManager->InitializeObjects(m_pTextureManager, m_pSoundManager);

	m_pCamera->SetLevelBoundaries(m_pLevelManager->GetBoundaries(m_CurrentLevel, m_CurrentStage, Sprite::SpriteType::MidLayer));
	PrintInfo();
}

void Game::Cleanup()
{
	delete m_pLevelManager;
	m_pLevelManager = nullptr;

	delete m_pEnemyManager;
	m_pEnemyManager = nullptr;

	delete m_pBossManager;
	m_pBossManager = nullptr;
	
	delete m_pObjectManager;
	m_pObjectManager = nullptr;
	
	delete m_pMenuManager;
	m_pMenuManager = nullptr;

	if (m_HasLevelStarted == true)
	{
		delete m_pPlayer;
		m_pPlayer = nullptr;
	}
	
	
	delete m_pDropManager;
	m_pDropManager = nullptr;

	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pTextureManager;
	m_pTextureManager = nullptr;	
	
	delete m_pSoundManager;
	m_pSoundManager = nullptr;

	delete m_pHud;
	m_pHud = nullptr;

}

void Game::Update(float elapsedSec)
{

	if (m_CurrentGameState == GameState::InMenu)
	{
		if (m_HasLevelStarted == false)
		{
			m_CurrentMenuType = MenuManager::MenuType::startScreen;
			m_pMenuManager->Update(elapsedSec, m_CurrentMenuType, 0);

		}
		else if (m_HasLevelStarted == true && (m_pPlayer->GetAmountOfLives() - 1) <= 0)
		{
			m_CurrentMenuType = MenuManager::MenuType::gameOver;
			m_pMenuManager->SetCanPlayLevel();
			m_pMenuManager->Update(elapsedSec, m_CurrentMenuType, m_pPlayer->GetAmountOfContinues());
		}
		//else
		//{
		//	m_CurrentMenuType = MenuManager::MenuType::startLevel;
		//}

		if (m_pMenuManager->CanPlayLevel() == true && m_HasLevelStarted == false) // m_CurrentMenuType == MenuManager::MenuType::startLevel && 
		{
			m_pPlayer = new Player(Point2f{ 35.f, 100.f }, m_pMenuManager->GetAmountOfPlayerLives(), 2, m_pTextureManager, m_pSoundManager);
			m_pCamera->SetLevelBoundaries(m_pLevelManager->GetBoundaries(m_CurrentLevel, m_CurrentStage, Sprite::SpriteType::MidLayer));

			m_HasLevelStarted = true;
			m_CurrentGameState = GameState::InGame;
			// initialize everything using the managers var
			// m_HasLevelStarted is used so we don't do this multiple times
		}
		else if (m_pMenuManager->CanPlayLevel() == true && m_HasLevelStarted == true)
		{
			m_pPlayer->ResetScore();
			m_pPlayer->GetAmountOfContinues() -= 1;
			ResetAfterDeath();
			m_pMenuManager->Reset();
			
			
			m_CurrentGameState = GameState::InGame;

			//std::cout << "player continue" << std::endl;

		}
		else if (m_pMenuManager->ResetGame() == true && m_HasLevelStarted == true)
		{
			m_HasLevelStarted = false;
			m_pMenuManager->Reset();

			//std::cout << "reset all" << std::endl;
			delete m_pPlayer;
			m_CurrentLevel = 0;
			m_CurrentStage = 0;
			// reset game over screen
			// delete all 
			// reinitialize all
			delete m_pLevelManager;
			delete m_pEnemyManager;
			delete m_pBossManager;
			delete m_pObjectManager;
			delete m_pDropManager;

			m_pLevelManager = new LevelManager();
			m_pEnemyManager = new EnemyManager();
			m_pBossManager = new BossManager();
			m_pObjectManager = new ObjectManager();
			m_pDropManager = new DropManager();

			m_pLevelManager->InitializeLevels(m_pTextureManager, m_pSoundManager);
			m_pEnemyManager->InitializeEnemies(m_pTextureManager, m_pSoundManager);
			m_pBossManager->InitializeBosses(m_pTextureManager, m_pSoundManager);
			m_pObjectManager->InitializeObjects(m_pTextureManager, m_pSoundManager);
		}


	}

	if (m_CurrentGameState == GameState::InGame)
	{
		if (m_CurrentLevelTransitionType == Level::TransitionType::fadeToBlack && m_FadeToBlackColor.a > 0.f)
		{
			m_FadeToBlackColor.a -= elapsedSec;
		}
	
		m_pLevelManager->Update(elapsedSec, m_pPlayer, m_pDropManager, m_pEnemyManager, m_pBossManager, m_pObjectManager, m_CurrentLevel, m_CurrentStage, m_pTextureManager, m_pSoundManager);

		m_pPlayer->Update(elapsedSec);

		if (m_pLevelManager->IsLevelInBattle() == false)
		{
			m_pCamera->Update(m_pPlayer->GetPlayerHitBox());
		}
		else
		{
			// get vertical trigger pos
			// if cam isn't there yet, move by a pixel
			// repeat until campos x value is the same as trigger value
			// move player aswell?
			// could also not move player, when player is fully past x value (calculate using player width)
		}

		
		if (m_pLevelManager->CheckTransitionTriggered(m_CurrentLevel, m_CurrentStage))
		{
			m_CurrentLevelTransitionType = m_pLevelManager->GetTransitionType(m_CurrentLevel, m_CurrentStage);
			m_NextStage = m_pLevelManager->GetNextStageIndex(m_CurrentLevel, m_CurrentStage);
			m_CurrentGameState = GameState::Transitioning;
		}

		if (m_pPlayer->IsPlayerDead() == true)
		{
			// stop all level sounds
			m_pLevelManager->StopLevelMusic(m_CurrentLevel, m_CurrentStage, m_pBossManager);
			m_pPlayerDeathSoundEffect->Play(0);
			m_CurrentLevelTransitionType = Level::TransitionType::fadeToBlack;
			m_CurrentGameState = GameState::Transitioning;
		}

		// update tests...
		{
			//m_pCamera->Update(m_testRectf);
			//collision test
			//CheckTestRect();
			// movement test
			//UpdateTestRect(elapsedSec);
		}
	}

	if (m_CurrentGameState == GameState::Transitioning)
	{
		switch (m_CurrentLevelTransitionType)
		{
		case Level::TransitionType::fadeToBlack:
			{
				m_FadeToBlackColor.a += elapsedSec;

				if (m_pPlayer->IsPlayerDead() == false)
				{
					if (m_FadeToBlackColor.a >= 1.f)
					{
						ResetLevelTransition();
					}

					if (m_pPlayer->MovePlayerFadeToBlack(elapsedSec) == false)
					{
						m_pLevelManager->Update(elapsedSec, m_pPlayer, m_pDropManager, m_pEnemyManager, m_pBossManager, m_pObjectManager, m_CurrentLevel, m_CurrentStage, m_pTextureManager, m_pSoundManager);
					}

				}
				else
				{
					if (m_FadeToBlackColor.a >= 3.f)
					{
						if ((m_pPlayer->GetAmountOfLives()-1) == 0)
						{
							m_CurrentGameState = GameState::InMenu;
						}
						else
						{
							ResetAfterDeath();

						}
					}
				}
			}
			break;
		case Level::TransitionType::door:
			{
				// if m_CamShift <= (m_CamWidth * m_CamScale) / 2.5f) => the door hasn't opened yet, we wait
				if (m_CamShift <= (m_CamWidth * m_CamScale) / 2.5f)
				{

					m_pPlayer->MakePlayerWait(elapsedSec);
				}
				else
				{
					float playerNewPos{ m_pLevelManager->GetNewPlayerPosition(m_CurrentLevel, m_CurrentStage).x 
										+ m_pLevelManager->GetBoundaries(m_CurrentLevel, m_CurrentStage, Sprite::SpriteType::MidLayer).width };
					m_pPlayer->MovePlayerThroughDoor(elapsedSec, playerNewPos);
				}


				m_pLevelManager->Update(elapsedSec, m_pPlayer, m_pDropManager, m_pEnemyManager, m_pBossManager, m_pObjectManager, m_CurrentLevel, m_CurrentStage, m_pTextureManager, m_pSoundManager);
				m_pLevelManager->Update(elapsedSec, m_pPlayer, m_pDropManager, m_pEnemyManager, m_pBossManager, m_pObjectManager, m_CurrentLevel, m_NextStage, m_pTextureManager, m_pSoundManager);


				if (m_CamShift < m_CamWidth * m_CamScale) // we need to shift the levels shown cam width * the scale of the map
				{
					float increment{ m_CamScale * 1.5f}; // triple the m_CamScale for a smooth transition * 3.f 
					m_CamShift += increment;
				}

				m_pCamera->Update(m_pPlayer->GetPlayerHitBox());
				
				if (m_CamShift >= m_CamWidth * m_CamScale ) // when m_CamShift has fully shifted by a camwidth, we are in the next level
				{
					ResetLevelTransition();
				}

			}
			break;
		}
	}

	
}

void Game::Draw() const
{
	// only draw when ingame or paused
	switch (m_CurrentGameState)
	{
	case Game::GameState::InGame:
	{
		DrawCurrentLevel();
		if (m_FadeToBlackColor.a >= 0.f)
		{
			utils::SetColor(m_FadeToBlackColor);
			utils::FillRect(0.f, 0.f, m_Window.width, m_Window.height);
		}
	}
		break;
	case Game::GameState::Paused:
	{
		DrawCurrentLevel();
		utils::SetColor(Color4f{ 0.f,0.f,0.f, 0.7f });
		utils::FillRect(0.f,0.f,m_Window.width, m_Window.height);
		m_pHud->DrawPause(m_pSoundManager);
	}
		break;
	case Game::GameState::Transitioning:
	{
		switch (m_CurrentLevelTransitionType)
		{
		case Level::TransitionType::fadeToBlack:
			DrawCurrentLevel();
			utils::SetColor(m_FadeToBlackColor);
			utils::FillRect(0.f, 0.f, m_Window.width, m_Window.height);
			break;
		case Level::TransitionType::door:
			ClearBackground();
			m_pLevelManager->DrawDoorTransition(m_CurrentLevel, m_CurrentStage, m_pCamera, m_pPlayer, m_pDropManager, m_pEnemyManager, m_pObjectManager, m_CamShift, m_NextStage);
			m_pHud->Draw(m_pPlayer, m_pLevelManager->GetCurrentLevel(m_CurrentLevel, m_CurrentStage), m_pBossManager, m_CurrentLevel);
			//DrawHud();
			break;
		}
	}
		break;
	case Game::GameState::InMenu:
	{
		ClearBackground();
		if (m_HasLevelStarted == true)
		{
			m_pMenuManager->Draw(m_CurrentMenuType, m_CamScale, m_pSoundManager, m_pPlayer->GetCurrentScore(), m_pPlayer->GetHighScore());
		}
		else
		{
			m_pMenuManager->Draw(m_CurrentMenuType, m_CamScale, m_pSoundManager,0, 0);
		}
	}
		break;
	}
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	switch ( e.keysym.sym )
	{
	case SDLK_RETURN:
		if (m_CurrentGameState != GameState::Transitioning && m_CurrentGameState != GameState::InMenu)
		{
			if (m_CurrentGameState == GameState::InGame)
			{
				m_CurrentGameState = GameState::Paused;

				m_pSoundManager->PauseAll();
				m_pPauseSoundEffect->Play(0);
			}
			else
			{
				m_CurrentGameState = GameState::InGame;
				m_pSoundManager->ResumeAll();
				m_pPauseSoundEffect->Play(0);

			}
		}
		if (m_CurrentGameState == GameState::InMenu)
		{
			m_pMenuManager->ButtonInput(Menu::ButtonPressed::start, m_CurrentMenuType, m_pSoundManager);
			//std::cout << "start" << std::endl;
		}
		break;
	case SDLK_KP_ENTER:
		if (m_CurrentGameState != GameState::Transitioning && m_CurrentGameState != GameState::InMenu)
		{
			if (m_CurrentGameState == GameState::InGame)
			{
				m_CurrentGameState = GameState::Paused;
				m_pSoundManager->PauseAll();
			}
			else
			{
				m_CurrentGameState = GameState::InGame;
				m_pSoundManager->ResumeAll();
			}
		}
		if (m_CurrentGameState == GameState::InMenu)
		{
			m_pMenuManager->ButtonInput(Menu::ButtonPressed::start, m_CurrentMenuType, m_pSoundManager);
			//std::cout << "start" << std::endl;

		}
		break;	
	case SDLK_i:
		PrintInfo();
		break;
	case SDLK_o:
		m_pSoundManager->DecreaseVolume();
		break;
	case SDLK_p:
		m_pSoundManager->IncreaseVolume();
		break;
	case SDLK_k:
		m_pSoundManager->MuteAndUnMute();
		break;
	case SDLK_a:
		if (m_CurrentGameState == GameState::InMenu)
		{
			m_pMenuManager->ButtonInput(Menu::ButtonPressed::start, m_CurrentMenuType, m_pSoundManager);
			//std::cout << "start" << std::endl;

		}
		break;
	case SDLK_UP:
		if (m_CurrentGameState == GameState::InMenu)
		{
			m_pMenuManager->ButtonInput(Menu::ButtonPressed::up, m_CurrentMenuType, m_pSoundManager);
			//std::cout << "up" << std::endl;

		}
		break;
	case SDLK_DOWN:
		if (m_CurrentGameState == GameState::InMenu)
		{
			m_pMenuManager->ButtonInput(Menu::ButtonPressed::down, m_CurrentMenuType, m_pSoundManager);
			//std::cout << "down" << std::endl;
		}
		break;
	case SDLK_LEFT:
		if (m_CurrentGameState == GameState::InMenu)
		{
			m_pMenuManager->ButtonInput(Menu::ButtonPressed::left, m_CurrentMenuType, m_pSoundManager);
			//std::cout << "left" << std::endl;
		}
		if (m_CurrentGameState == GameState::Paused)
		{
			m_pSoundManager->DecreaseVolume();
		}
		break;
	case SDLK_RIGHT:
		if (m_CurrentGameState == GameState::InMenu)
		{
			m_pMenuManager->ButtonInput(Menu::ButtonPressed::right, m_CurrentMenuType, m_pSoundManager);
			//std::cout << "right" << std::endl;
		}
		if (m_CurrentGameState == GameState::Paused)
		{
			m_pSoundManager->IncreaseVolume();
		}
		break;
	}

}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::ResetLevelTransition()
{
	m_pLevelManager->ResetTransition(m_CurrentLevel, m_CurrentStage);
	m_pPlayer->SetPos(m_pLevelManager->GetNewPlayerPosition(m_CurrentLevel, m_CurrentStage), m_NextStage);
	m_CurrentStage = m_NextStage;
	m_pCamera->SetLevelBoundaries(m_pLevelManager->GetBoundaries(m_CurrentLevel, m_CurrentStage, Sprite::SpriteType::MidLayer));
	m_pCamera->SnapCamToPlayer(m_pPlayer->GetPlayerHitBox());
	m_CurrentGameState = GameState::InGame;
	m_CamShift = 0.f; // only used in door transition, placed here for convenience
}

void Game::ResetAfterDeath()
{
	// resetting all levels, objects, enemies, bosses, drops
	delete m_pLevelManager;
	delete m_pEnemyManager;
	delete m_pBossManager;
	delete m_pObjectManager;
	delete m_pDropManager;

	m_pLevelManager = new LevelManager();
	m_pEnemyManager = new EnemyManager();
	m_pBossManager = new BossManager();
	m_pObjectManager = new ObjectManager();
	m_pDropManager = new DropManager();

	m_pLevelManager->InitializeLevels(m_pTextureManager, m_pSoundManager);
	m_pEnemyManager->InitializeEnemies(m_pTextureManager, m_pSoundManager);
	m_pBossManager->InitializeBosses(m_pTextureManager, m_pSoundManager);
	m_pObjectManager->InitializeObjects(m_pTextureManager, m_pSoundManager);



	// reset player
	if ((m_pPlayer->GetAmountOfLives() -1) == 0)
	{
		m_pPlayer->ResetPlayerAfterDeath(m_pMenuManager->GetAmountOfPlayerLives());
	}
	else
	{
		m_pPlayer->ResetPlayerAfterDeath(m_pPlayer->GetAmountOfLives() - 1);

	}
	// reset current stage
	m_CurrentStage = m_pPlayer->GetRespawnLevelIndex();
	// reset cam
	m_pCamera->SetLevelBoundaries(m_pLevelManager->GetBoundaries(m_CurrentLevel, m_CurrentStage, Sprite::SpriteType::MidLayer));
	m_pCamera->SnapCamToPlayer(m_pPlayer->GetPlayerHitBox());
	// switch gamestate
	m_CurrentGameState = GameState::InGame;


}


void Game::DrawCurrentLevel() const
{
	ClearBackground();
	m_pLevelManager->DrawCurrentLevel(m_CurrentLevel, m_CurrentStage, m_pCamera, m_pPlayer, m_pDropManager, m_pEnemyManager, m_pBossManager, m_pObjectManager);
	m_pHud->Draw(m_pPlayer, m_pLevelManager->GetCurrentLevel(m_CurrentLevel, m_CurrentStage), m_pBossManager, m_CurrentLevel);
	//DrawHud();
}

void Game::DrawHud() const
{
	// placeholder for the HUD
	// having a placeholder makes it look a little cleaner
	m_pHud->Draw(m_pPlayer, m_pLevelManager->GetCurrentLevel(m_CurrentLevel, m_CurrentStage), m_pBossManager, m_CurrentLevel);
	//utils::SetColor(Color4f{33/255.f, 0.f, 33/255.f, 1.f});
	//utils::FillRect(Rectf{ 0.f, m_Window.height - (32.f * m_CamScale), m_Window.width, 32.f * m_CamScale});
}

void Game::PrintInfo() const
{
	std::cout << "------INFO------------------------------------------------------------------------" << std::endl;
	std::cout << "Move with the arrow keys." << std::endl;
	std::cout << "In menu, use 'enter' to start." << std::endl;
	std::cout << "Attack with the 'a' key." << std::endl;
	std::cout << "Jump with the 's' key." << std::endl;
	std::cout << "Throw subweapon with the 'd' key. (only works for the Axe and Boomerang at the moment)" << std::endl;
	std::cout << "Mute/unmute all sounds using 'k'." << std::endl;
	std::cout << "Decrease volume using 'o'." << std::endl;
	std::cout << "Increase volume using 'p'." << std::endl;
	std::cout << "Press 'enter' to pause the game." << std::endl;
	std::cout << "When paused, you can use the arrow keys to in/decrease volume."  << std::endl;
	std::cout << "-----------------------------------------------------------------------------------" << std::endl << std::endl << std::endl;
}

void Game::UpdateTestRect(float elapsedSec)
{
	// player movement (temp test)
	// Check keyboard state
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		m_testRectf.left += 2.f;
		//std::cout << "Right arrow key is down\n";
	}
	if (pStates[SDL_SCANCODE_UP])
	{
		m_testRectf.bottom += 2.f;
		//std::cout << "Left and up arrow keys are down\n";
	}
	if (pStates[SDL_SCANCODE_LEFT])
	{
		m_testRectf.left -= 2.f;
		//std::cout << "Left and up arrow keys are down\n";
	}
	if (pStates[SDL_SCANCODE_DOWN])
	{
		m_testRectf.bottom -= 2.f;
		//std::cout << "Left and up arrow keys are down\n";
	}
}

void Game::CheckTestRect()
{
	// rough test
	// every hitbox should be a vector, later there will be a struggle with polygons
	std::vector<Point2f> points{ 
		Point2f{m_testRectf.left, m_testRectf.bottom}, 
		Point2f{m_testRectf.left, m_testRectf.bottom + m_testRectf.height},
		Point2f{m_testRectf.left + m_testRectf.width, m_testRectf.bottom},
		Point2f{m_testRectf.left + m_testRectf.width, m_testRectf.bottom + m_testRectf.height } };
	for (size_t i{0}; i < points.size(); ++i)
	{
		if (utils::IsPointInRect(points[i], m_pPlayer->GetPlayerHitBox()))
		{
			m_pPlayer->IsHit(points[i].x, 2);
		}
	}
	

	// draw test rect 
	//utils::SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });
	//utils::DrawRect(m_testRectf, 2.f);
}
