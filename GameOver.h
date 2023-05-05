#pragma once
#include "Menu.h"
class TextureManager;
class SoundManager;
class Sprite;

class GameOver final : public Menu
{
public:
	GameOver(TextureManager* pTextureManager, SoundManager* pSoundManager);
	GameOver(const GameOver& other) = delete;
	GameOver& operator=(const GameOver& other) = delete;
	GameOver(GameOver&& other) = delete;
	GameOver& operator=(GameOver&& other) = delete;
	virtual ~GameOver();

	virtual void Draw(float camScale, SoundManager* pSoundManager, int playerLastScore, int playerHighScore) const override;
	virtual void Update(float elapsedSec, int playeramountOfContinues) override;

	virtual void ButtonInput(Menu::ButtonPressed pressedButton, SoundManager* pSoundManager) override;

	virtual bool HasFinished() override;
	virtual bool ResetGame() override;

	virtual void Reset() override;

private:
	int m_PlayerAmountOfContinues;
	bool m_CanContinue;
	bool m_ResetGame;
	bool m_GameOverScrollFinished;

	Sprite* m_pPointer;

	Sprite* m_pTitle;
	Sprite* m_pScores;
	Sprite* m_pContinue;
	Sprite* m_pEnd;

	Point2f m_TitleBeginPosition;
	const Point2f m_TitleEndPosition;
	Point2f m_TitleCurrentPosition;

	Sprite* m_pNumbers;
	const Point2f m_ScorePosition;
	const Point2f m_HighScorePosition;
	const Point2f m_ContinuePosition;

	SoundEffect* m_pGameOverSoundEffect;
	bool m_HasSoundEffectPlayed;
};

