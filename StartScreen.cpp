#include "pch.h"
#include "StartScreen.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "SoundStream.h"
#include "SoundEffect.h"
#include "Sprite.h"

#include "utils.h"

#include <iostream>

StartScreen::StartScreen(TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Menu(pSoundManager)
	, m_CurrentPhase{StartScreenPhases::intro}
	//, m_ButtonFlicker{0.f}
	//, m_CountDownStarted{false}
	, m_AmountOfLives{3}
	, m_PlayerAmountOfLivesPosition{ Point2f{233.f, 156.f} } // 233.f
	, m_VolumePosition{ Point2f{232.f, 44.f} }
{
	m_BloodDropStartPositions.push_back(Point2f{ 50.f, 125.f });
	m_BloodDropStartPositions.push_back(Point2f{ 215.f, 119.f });
	m_BloodDropStartPositions.push_back(Point2f{ 125.f, 125.f });

	m_BloodDropEndPositions.push_back(Point2f{50.f, 25.f});
	m_BloodDropEndPositions.push_back(Point2f{215.f, 34.f});
	m_BloodDropEndPositions.push_back(Point2f{125.f, 56.f});

	m_BloodDropCurrentPositions.push_back(m_BloodDropStartPositions[0]);
	m_BloodDropCurrentPositions.push_back(m_BloodDropStartPositions[1]);
	m_BloodDropCurrentPositions.push_back(m_BloodDropStartPositions[2]);

	m_pTitleLogo = new Sprite("Resources/Sprites/Misc/TitleScreen/CastleVania_Logo.png", 1, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pPressStart = new Sprite("Resources/Sprites/Misc/TitleScreen/Start_Button.png", 1, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pOptionButton = new Sprite("Resources/Sprites/Misc/TitleScreen/Options.png", 1, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pPasswordButton = new Sprite("Resources/Sprites/Misc/TitleScreen/Password.png", 1, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pStartGameButton = new Sprite("Resources/Sprites/Misc/TitleScreen/Player_Start.png", 1, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pPointer = new Sprite("Resources/Sprites/Misc/TitleScreen/Pointer.png", 3, 1, 15, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	
	m_pOptionsMenuBackGround = new Sprite("Resources/Sprites/Misc/TitleScreen/OptionsMenu_Background.png", 1, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pOptionsMenuText = new Sprite("Resources/Sprites/Misc/TitleScreen/OptionsMenu_Text.png", 1, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pOptionNumbers = new Sprite("Resources/Sprites/Misc/TitleScreen/OptionsNumbers.png", 10, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);

	m_pIntroTitleSoundStream = pSoundManager->GetSoundStreamPointer("Resources/Sounds/Streams/Intro_TitleScreen_Theme.mp3");


	m_pBloodDropSprites.push_back(new Sprite("Resources/Sprites/Misc/TitleScreen/BloodDropping.png", 7, 1, 5, m_BloodDropCurrentPositions[0], Sprite::SpriteType::Other, pTextureManager));
	m_pBloodDropSprites.push_back(new Sprite("Resources/Sprites/Misc/TitleScreen/BloodDropping.png", 7, 1, 5, m_BloodDropCurrentPositions[1], Sprite::SpriteType::Other, pTextureManager));
	m_pBloodDropSprites.push_back(new Sprite("Resources/Sprites/Misc/TitleScreen/BloodDropping.png", 7, 1, 5, m_BloodDropCurrentPositions[2], Sprite::SpriteType::Other, pTextureManager));
}


StartScreen::~StartScreen()
{
	delete m_pTitleLogo;
	m_pTitleLogo = nullptr;

	delete m_pPressStart;
	m_pPressStart = nullptr;

	delete m_pOptionButton;
	m_pOptionButton = nullptr;

	delete m_pPasswordButton;
	m_pPasswordButton = nullptr;

	delete m_pStartGameButton;
	m_pStartGameButton = nullptr;

	delete m_pPointer;
	m_pPointer = nullptr;

	delete m_pOptionsMenuBackGround;
	m_pOptionsMenuBackGround = nullptr;

	delete m_pOptionsMenuText;
	m_pOptionsMenuText = nullptr;

	delete m_pOptionNumbers;
	m_pOptionNumbers = nullptr;


	for (size_t index{0}; index < m_pBloodDropSprites.size(); ++index)
	{
		delete m_pBloodDropSprites[index];
		m_pBloodDropSprites[index] = nullptr;
	}
}

void StartScreen::Draw(float camScale, SoundManager* pSoundManager, int playerLastScore, int playerHighScore) const
{
	float pixelBuffer{ 2.f };

	int globalSoundInPercent{ int(float(pSoundManager->GetGlobalValue() / 128.f) * 100)};
	//std::cout << pSoundManager->GetGlobalValue() << std::endl;
	glPushMatrix();
	{
		glScalef(camScale, camScale, 1.f);
		switch (m_CurrentPhase)
		{
		case StartScreen::StartScreenPhases::konamiLogo:
			break;
		case StartScreen::StartScreenPhases::story:
			break;
		case StartScreen::StartScreenPhases::intro:
		{
			//utils::SetColor(Color4f{ 1.f,1.f,0.f, 1.f });
			//utils::FillRect(0.f, 0.f, 320.f, 224.f);
			m_pTitleLogo->Draw();
			if (int(m_ButtonFlicker * 5) % 2 == 0)
			{
				m_pPressStart->Draw();
			}

			//m_pBloodDropSprite1->Draw();

			for (size_t index{ 0 }; index < m_pBloodDropSprites.size(); ++index)
			{
				if (size_t(m_ButtonFlicker * 2.f) > index)
				{
					m_pBloodDropSprites[index]->Draw();

				}
			}
			
		}
			break;
		case StartScreen::StartScreenPhases::mainMenu:
		{
			//utils::SetColor(Color4f{ 0.f,1.f,0.f, 1.f });
			//utils::FillRect(0.f, 0.f, 320.f, 224.f);
			m_pTitleLogo->Draw();
			m_pPointer->Draw();
			if (m_CountDownStarted == true)
			{
				if (m_CurrentSelectionIndex == 0)
				{
					if (int(m_ButtonFlicker * 10.f) % 2 == 0)
					{
						m_pStartGameButton->Draw();
					}
					m_pOptionButton->Draw();
					m_pPasswordButton->Draw();
				}
				else if (m_CurrentSelectionIndex == 1)
				{
					if (int(m_ButtonFlicker * 10.f) % 2 == 0)
					{
						m_pOptionButton->Draw();
					}
					m_pPasswordButton->Draw();
					m_pStartGameButton->Draw();
				}
				// add password menu later
			}
			else
			{
				m_pOptionButton->Draw();
				m_pPasswordButton->Draw();
				m_pStartGameButton->Draw();
			}
		}
			break;
		case StartScreen::StartScreenPhases::options:
		{

			m_pOptionsMenuBackGround->Draw();
			m_pOptionsMenuText->Draw();
			m_pPointer->Draw();



			// volume
			{
				std::string volumeString{};
				if (globalSoundInPercent < 10)
				{
					volumeString = "00" + std::to_string(globalSoundInPercent);
				}
				else if (globalSoundInPercent < 100)
				{
					volumeString = "0" + std::to_string(globalSoundInPercent);
				}
				else
				{
					volumeString = std::to_string(globalSoundInPercent);
				}

				for (size_t index{ 1 }; index < volumeString.size() + 1; ++index)
				{
					m_pOptionNumbers->SetDrawPos(Point2f{ m_VolumePosition.x - (m_pOptionNumbers->GetFrameWidth() * (index - 1)) - (pixelBuffer * (index - 1)), m_VolumePosition.y });
					char charValue{ volumeString[volumeString.size() - index] }; // we get the char of the index
					int correctValue{ int(charValue) - 48 };
					m_pOptionNumbers->SetCurrentFrame(correctValue);
					m_pOptionNumbers->Draw();
				}
			}

			// amount of lives
			{
				std::string livesString{ std::to_string(m_AmountOfLives)};
				for (size_t index{ 1 }; index < livesString.size() + 1; ++index)
				{
					m_pOptionNumbers->SetDrawPos(Point2f{ m_PlayerAmountOfLivesPosition.x - (m_pOptionNumbers->GetFrameWidth() * (index - 1)) - (pixelBuffer * (index - 1)), m_PlayerAmountOfLivesPosition.y });
					char charValue{ livesString[livesString.size() - index] }; // we get the char of the index
					int correctValue{ int(charValue) - 48 };
					m_pOptionNumbers->SetCurrentFrame(correctValue);
					m_pOptionNumbers->Draw();
				}
			}

		}
			break;
		case StartScreen::StartScreenPhases::characterSelection:
			break;
		}
	}
	glPopMatrix();
}

void StartScreen::Update(float elapsedSec, int playeramountOfContinues)
{
	float waitAmountOfSeconds{ 2.f }; 

	switch (m_CurrentPhase)
	{
	case StartScreen::StartScreenPhases::konamiLogo:
		break;
	case StartScreen::StartScreenPhases::story:
		break;
	case StartScreen::StartScreenPhases::intro:
		if (m_pIntroTitleSoundStream->IsPlaying() == false)
		{
			m_pIntroTitleSoundStream->Play(true);
		}
		m_ButtonFlicker += elapsedSec;

		for (size_t index{ 0 }; index < m_pBloodDropSprites.size(); ++index)
		{
			m_pBloodDropSprites[index]->SetDrawPos(m_BloodDropCurrentPositions[index]);

			if (m_pBloodDropSprites[index]->GetCurrentFrame() == 2 && m_BloodDropCurrentPositions[index].y > m_BloodDropEndPositions[index].y)
			{
				--m_BloodDropCurrentPositions[index].y;
				m_pBloodDropSprites[index]->SetDrawPos(m_BloodDropCurrentPositions[index]);
			}
			else
			{
				
				// update frames
				if (size_t(m_ButtonFlicker * 2.f) > index)
				{
					if (m_pBloodDropSprites[index]->GetCurrentFrame() == m_pBloodDropSprites[index]->GetTotalAmountOfFrames() )
					{
						m_BloodDropCurrentPositions[index] = m_BloodDropStartPositions[index];
					}
					m_pBloodDropSprites[index]->Update(elapsedSec);

				}
			}
		}

		
		break;
	case StartScreen::StartScreenPhases::mainMenu:
	{
		m_SelectionPositions.clear();
		m_SelectionPositions.push_back(Point2f{112.f, 71.f});
		m_SelectionPositions.push_back(Point2f{112.f, 39.f});
		m_pPointer->SetDrawPos(m_SelectionPositions[m_CurrentSelectionIndex]);
		m_pPointer->Update(elapsedSec);

		if (m_pIntroTitleSoundStream->IsPlaying() == true)
		{
			m_pIntroTitleSoundStream->Stop();
		}
		if (m_CountDownStarted == true)
		{
			m_ButtonFlicker += elapsedSec;
		}
		if (m_ButtonFlicker >= waitAmountOfSeconds) // wait amount of seconds before switching menu
		{
			if (m_CurrentSelectionIndex == 0)
			{
				m_CurrentPhase = StartScreenPhases::finished;
				m_CountDownStarted = false;
				m_ButtonFlicker = 0.f;
			}
			else if (m_CurrentSelectionIndex == 1)
			{
				m_CurrentPhase = StartScreenPhases::options;
				m_CountDownStarted = false;
				m_ButtonFlicker = 0.f;
			}
			m_CurrentSelectionIndex = 0;
		}
	}
		break;
	case StartScreen::StartScreenPhases::options:
		m_SelectionPositions.clear();
		m_SelectionPositions.push_back(Point2f{ 78.f, 154.f });
		m_SelectionPositions.push_back(Point2f{ 78.f, 41.f });
		m_pPointer->SetDrawPos(m_SelectionPositions[m_CurrentSelectionIndex]);
		m_pPointer->Update(elapsedSec);
		break;
	case StartScreen::StartScreenPhases::characterSelection:
		break;
	}
}

void StartScreen::ButtonInput(Menu::ButtonPressed pressedButton, SoundManager* pSoundManager)
{
	switch (m_CurrentPhase)
	{
	case StartScreen::StartScreenPhases::konamiLogo:
		// wait to implement
		break;
	case StartScreen::StartScreenPhases::story:
		// wait to implement
		break;
	case StartScreen::StartScreenPhases::intro:
	{
		if (pressedButton == Menu::ButtonPressed::start)
		{
			// play jingle
			m_pSelectedSoundEffect->Play(0);
			m_CurrentPhase = StartScreenPhases::mainMenu;
			m_ButtonFlicker = 0.f;
		}
	}
		break;
	case StartScreen::StartScreenPhases::mainMenu:
	{

		if (m_CountDownStarted == false)
		{
			if (pressedButton == Menu::ButtonPressed::start)
			{
				m_pSelectedSoundEffect->Play(0);
				m_CountDownStarted = true;
			}
			if (pressedButton == Menu::ButtonPressed::up)
			{
				m_pPointerMoveSoundEffect->Play(0);
				if (m_CurrentSelectionIndex == 0)
				{
					m_CurrentSelectionIndex = int(m_SelectionPositions.size()) - 1;
				}
				else
				{
					--m_CurrentSelectionIndex;
				}
			}

			if (pressedButton == Menu::ButtonPressed::down)
			{
				m_pPointerMoveSoundEffect->Play(0);
				if (m_CurrentSelectionIndex == int(m_SelectionPositions.size()) - 1)
				{
					m_CurrentSelectionIndex = 0;
				}
				else
				{
					++m_CurrentSelectionIndex;
				}
			}
		}
		

	}
		break;
	case StartScreen::StartScreenPhases::options:
	{
		if (pressedButton == Menu::ButtonPressed::start)
		{
			m_pSelectedSoundEffect->Play(0);
			m_CurrentPhase = StartScreenPhases::mainMenu;
			m_CurrentSelectionIndex = 0;
		}

		if (pressedButton == Menu::ButtonPressed::up)
		{
			m_pPointerMoveSoundEffect->Play(0);

			if (m_CurrentSelectionIndex == 0)
			{
				m_CurrentSelectionIndex = int(m_SelectionPositions.size()) - 1;
			}
			else
			{
				--m_CurrentSelectionIndex;
			}
		}

		if (pressedButton == Menu::ButtonPressed::down)
		{
			m_pPointerMoveSoundEffect->Play(0);

			if (m_CurrentSelectionIndex == int(m_SelectionPositions.size()) - 1)
			{
				m_CurrentSelectionIndex = 0;
			}
			else
			{
				++m_CurrentSelectionIndex;
			}
		}

		if (pressedButton == Menu::ButtonPressed::left)
		{
			if (m_CurrentSelectionIndex == 0)
			{
				if (m_AmountOfLives > 1)
				{
					--m_AmountOfLives;
				}
			}
			else if (m_CurrentSelectionIndex == 1)
			{
				pSoundManager->DecreaseVolume();
			}
		}
		if (pressedButton == Menu::ButtonPressed::right)
		{
			if (m_CurrentSelectionIndex == 0)
			{
				if (m_AmountOfLives < 9)
				{
					++m_AmountOfLives;
				}
			}
			else if (m_CurrentSelectionIndex == 1)
			{
				pSoundManager->IncreaseVolume();
			}
		}

	}
		break;
	case StartScreen::StartScreenPhases::characterSelection:
		// wait to implement
		break;
	}
}

bool StartScreen::HasFinished()
{
	if (m_CurrentPhase == StartScreenPhases::finished)
	{
		return true;
	}
	return false;
}

int StartScreen::GetAmountOfLives()
{
	return m_AmountOfLives;
}

void StartScreen::Reset()
{
	m_CurrentPhase = StartScreenPhases::intro;
}
