#include "pch.h"
#include "MenuManager.h"

#include "StartScreen.h"
#include "GameOver.h"

MenuManager::MenuManager(TextureManager* pTextureManager, SoundManager* pSoundManager)
	: m_CanPlayLevel{false}
{
	m_pStartScreen = new StartScreen(pTextureManager, pSoundManager);
	m_pGameOver = new GameOver(pTextureManager, pSoundManager);
}

MenuManager::~MenuManager()
{
	delete m_pStartScreen;
	m_pStartScreen = nullptr;


	delete m_pGameOver;
	m_pGameOver = nullptr;
}

void MenuManager::Update(float elapsedSec, MenuType currentMenuType, int playeramountOfContinues)
{
	switch (currentMenuType)
	{
	case MenuManager::MenuType::startScreen:
		m_pStartScreen->Update(elapsedSec, playeramountOfContinues);
		if (m_pStartScreen->HasFinished() == true)
		{
			m_CanPlayLevel = true;
		}
		break;
	case MenuManager::MenuType::startLevel:

		break;
	case MenuManager::MenuType::gameOver:
		m_pGameOver->Update(elapsedSec, playeramountOfContinues);
		if (m_pGameOver->HasFinished() == true)
		{
			m_CanPlayLevel = true;
		}
		break;
	}
}

void MenuManager::Draw(MenuType currentMenuType, float camScale, SoundManager* pSoundManager, int playerLastScore, int playerHighScore) const
{
	switch (currentMenuType)
	{
	case MenuManager::MenuType::startScreen:
		m_pStartScreen->Draw(camScale, pSoundManager, playerLastScore, playerHighScore);
		break;
	case MenuManager::MenuType::startLevel:
		break;
	case MenuManager::MenuType::gameOver:
		m_pGameOver->Draw(camScale, pSoundManager, playerLastScore, playerHighScore);
		break;
	}
}

bool MenuManager::CanPlayLevel()
{
	return m_CanPlayLevel;
}

void MenuManager::SetCanPlayLevel()
{
	m_CanPlayLevel = false;
}

bool MenuManager::ResetGame()
{
	return m_pGameOver->ResetGame();
}

int MenuManager::GetAmountOfPlayerLives()
{
	return m_pStartScreen->GetAmountOfLives();
}

void MenuManager::Reset()
{
	m_pStartScreen->Reset();
	m_pGameOver->Reset();
}

void MenuManager::ButtonInput(Menu::ButtonPressed pressedButton, MenuType currentMenuType, SoundManager* pSoundManager)
{
	switch (currentMenuType)
	{
	case MenuManager::MenuType::startScreen:
		m_pStartScreen->ButtonInput(pressedButton, pSoundManager);
		break;
	case MenuManager::MenuType::startLevel:
		break;
	case MenuManager::MenuType::gameOver:
		m_pGameOver->ButtonInput(pressedButton, pSoundManager);
		break;
	}
}
