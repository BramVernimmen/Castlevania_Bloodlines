#include "pch.h"
#include "GameOver.h"
#include "TextureManager.h"
#include "SoundEffect.h"
#include "Sprite.h"
#include "SoundManager.h"

#include "utils.h"

GameOver::GameOver(TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Menu(pSoundManager)
	, m_PlayerAmountOfContinues{0}
	, m_CanContinue{false}
	, m_ResetGame{false}
	, m_GameOverScrollFinished{false}
	, m_TitleBeginPosition{ Point2f{0.f, 0.f} }
	, m_TitleEndPosition{Point2f{0.f, 170.f}}
	, m_ScorePosition{ Point2f{225.f, 134.f} } // 134
	, m_HighScorePosition{ Point2f{225.f, 118.f} }
	, m_ContinuePosition{ Point2f{215.f, 94.f} }
	, m_HasSoundEffectPlayed{false}
{
	m_pPointer = new Sprite("Resources/Sprites/Misc/TitleScreen/Pointer.png", 3, 1, 15, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pTitle = new Sprite("Resources/Sprites/Misc/TitleScreen/GameOver_Title.png", 1, 1, 1, m_TitleBeginPosition, Sprite::SpriteType::Other, pTextureManager);
	m_TitleBeginPosition.y = 112.f - m_pTitle->GetFrameHeight() / 2.f;
	m_pScores = new Sprite("Resources/Sprites/Misc/TitleScreen/GameOver_Scores.png", 1, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pContinue = new Sprite("Resources/Sprites/Misc/TitleScreen/GameOver_Continue.png", 1, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pEnd = new Sprite("Resources/Sprites/Misc/TitleScreen/GameOver_End.png", 1, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pNumbers = new Sprite("Resources/Sprites/Misc/TitleScreen/GameOver_Numbers.png", 10, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_TitleCurrentPosition = m_TitleBeginPosition;

	m_pGameOverSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/PlayerGameOver.mp3");

}

GameOver::~GameOver()
{
	delete m_pPointer;
	m_pPointer = nullptr;

	delete m_pTitle;
	m_pTitle = nullptr;

	delete m_pScores;
	m_pScores = nullptr;

	delete m_pContinue;
	m_pContinue = nullptr;

	delete m_pEnd;
	m_pEnd = nullptr;

	delete m_pNumbers;
	m_pNumbers = nullptr;
}

void GameOver::Draw(float camScale, SoundManager* pSoundManager, int playerLastScore, int playerHighScore) const
{
	float pixelBuffer{ 2.f };

	glPushMatrix();
	{
		glScalef(camScale, camScale, 1.f);
		//utils::SetColor(Color4f{ 1.f,0.f,0.f, 1.f });
		//utils::FillRect(0.f, 0.f, 320.f, 224.f);

		if (m_TitleCurrentPosition.y < m_TitleEndPosition.y)
		{
			m_pTitle->Draw();
		}
		else
		{
			m_pTitle->Draw();
			m_pScores->Draw();
			if (m_CountDownStarted == true)
			{
				if (m_PlayerAmountOfContinues > 0)
				{
					if (m_CurrentSelectionIndex == 0)
					{
						if (int(m_ButtonFlicker * 10.f) % 2 == 0)
						{
							m_pContinue->Draw();
							// continues
							{
								m_pNumbers->SetDrawPos(Point2f{ m_ContinuePosition.x - m_pNumbers->GetFrameWidth(), m_ContinuePosition.y });
								m_pNumbers->SetCurrentFrame(m_PlayerAmountOfContinues);
								m_pNumbers->Draw();
							}
						}
						m_pEnd->Draw();
					}
					else if (m_CurrentSelectionIndex == 1)
					{
						if (int(m_ButtonFlicker * 10.f) % 2 == 0)
						{
							m_pEnd->Draw();
						}
						m_pContinue->Draw();
						// continues
						{
							m_pNumbers->SetDrawPos(Point2f{ m_ContinuePosition.x - m_pNumbers->GetFrameWidth(), m_ContinuePosition.y });
							m_pNumbers->SetCurrentFrame(m_PlayerAmountOfContinues);
							m_pNumbers->Draw();
						}
					}
				}
				else
				{
					if (m_CurrentSelectionIndex == 0)
					{
						if (int(m_ButtonFlicker * 10.f) % 2 == 0)
						{
							m_pEnd->Draw();
						}
					}
				}
				// add password menu later
			}
			else
			{
				if (m_PlayerAmountOfContinues > 0)
				{
					m_pContinue->Draw();
					// continues
					{
						m_pNumbers->SetDrawPos(Point2f{ m_ContinuePosition.x - m_pNumbers->GetFrameWidth(), m_ContinuePosition.y });
						m_pNumbers->SetCurrentFrame(m_PlayerAmountOfContinues);
						m_pNumbers->Draw();
					}
				}
				m_pEnd->Draw();
			}
			//m_pContinue->Draw();
			//m_pEnd->Draw();
			m_pPointer->Draw();

			// score
			{
				std::string scoreString{ std::to_string(playerLastScore) };
				for (size_t index{ 1 }; index < scoreString.size() + 1; ++index)
				{
					m_pNumbers->SetDrawPos(Point2f{ m_ScorePosition.x - (m_pNumbers->GetFrameWidth() * (index - 1)) - (pixelBuffer * (index - 1)), m_ScorePosition.y });
					char charValue{ scoreString[scoreString.size() - index] }; // we get the char of the index
					int correctValue{ int(charValue) - 48 };
					m_pNumbers->SetCurrentFrame(correctValue);
					m_pNumbers->Draw();
				}
			}

			// highscore
			{
				std::string highscoreString{ std::to_string(playerHighScore) };
				for (size_t index{ 1 }; index < highscoreString.size() + 1; ++index)
				{
					m_pNumbers->SetDrawPos(Point2f{ m_HighScorePosition.x - (m_pNumbers->GetFrameWidth() * (index - 1)) - (pixelBuffer * (index - 1)), m_HighScorePosition.y });
					char charValue{ highscoreString[highscoreString.size() - index] }; // we get the char of the index
					int correctValue{ int(charValue) - 48 };
					m_pNumbers->SetCurrentFrame(correctValue);
					m_pNumbers->Draw();
				}
			}
		}
	}
	glPopMatrix();
}

void GameOver::Update(float elapsedSec, int playeramountOfContinues)
{
	if (m_HasSoundEffectPlayed == false)
	{
		m_pGameOverSoundEffect->Play(0);
		m_HasSoundEffectPlayed = true;
	}
	m_PlayerAmountOfContinues = playeramountOfContinues;
	float waitAmountOfSeconds{ 2.f };

	if (m_TitleCurrentPosition.y < m_TitleEndPosition.y)
	{
		m_TitleCurrentPosition.y += 1.f;
		m_pTitle->SetDrawPos(m_TitleCurrentPosition);
	}
	else
	{
		m_GameOverScrollFinished = true;
	}
	
	m_SelectionPositions.clear();
	if (playeramountOfContinues > 0)
	{
		m_SelectionPositions.push_back(Point2f{ 112.f, 92.f });
		m_SelectionPositions.push_back(Point2f{ 112.f, 75.f });
	}
	else
	{
		m_SelectionPositions.push_back(Point2f{ 112.f, 75.f });
	}
	
	m_pPointer->SetDrawPos(m_SelectionPositions[m_CurrentSelectionIndex]);
	m_pPointer->Update(elapsedSec);

	if (m_CountDownStarted == true)
	{
		m_ButtonFlicker += elapsedSec;
	}
	if (m_ButtonFlicker >= waitAmountOfSeconds) // wait amount of seconds before switching menu
	{

		if (m_PlayerAmountOfContinues > 0)
		{
			if (m_CurrentSelectionIndex == 0)
			{
				//m_CurrentPhase = StartScreenPhases::finished;
				// continue
				m_CanContinue = true;
			}
			else if (m_CurrentSelectionIndex == 1)
			{
				//m_CurrentPhase = StartScreenPhases::options;
				// end game
				m_ResetGame = true;
			}
		}
		else
		{
			m_ResetGame = true;
			// end game
		}
		m_CountDownStarted = false;
		m_ButtonFlicker = 0.f;
		m_CurrentSelectionIndex = 0;
	}
}

void GameOver::ButtonInput(Menu::ButtonPressed pressedButton, SoundManager* pSoundManager)
{
	if (m_CountDownStarted == false && m_GameOverScrollFinished == true)
	{
		if (pressedButton == Menu::ButtonPressed::start)
		{
			m_CountDownStarted = true;
			m_pSelectedSoundEffect->Play(0);
		}

		if (pressedButton == Menu::ButtonPressed::up)
		{
			if (m_CurrentSelectionIndex == 0)
			{
				m_CurrentSelectionIndex = int(m_SelectionPositions.size()) - 1;
			}
			else
			{
				--m_CurrentSelectionIndex;
			}
			m_pPointerMoveSoundEffect->Play(0);
		}

		if (pressedButton == Menu::ButtonPressed::down)
		{
			if (m_CurrentSelectionIndex == int(m_SelectionPositions.size()) - 1)
			{
				m_CurrentSelectionIndex = 0;
			}
			else
			{
				++m_CurrentSelectionIndex;
			}
			m_pPointerMoveSoundEffect->Play(0);
		}
	}
}

bool GameOver::HasFinished()
{
	return m_CanContinue;
}

bool GameOver::ResetGame()
{
	return m_ResetGame;
}

void GameOver::Reset()
{
	m_CanContinue = false;
	m_ResetGame = false;
	m_GameOverScrollFinished = false;
	m_PlayerAmountOfContinues = 0;
	m_TitleCurrentPosition = m_TitleBeginPosition;
	m_pTitle->SetDrawPos(m_TitleBeginPosition);
	m_HasSoundEffectPlayed = false;

}
