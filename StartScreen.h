#pragma once
#include "Menu.h"
class TextureManager;
class SoundManager;
class Sprite;
class SoundStream;
class StartScreen final : public Menu
{
public:
	enum class StartScreenPhases
	{
		konamiLogo,
		story,
		intro,
		mainMenu,
		options,
		characterSelection,
		finished
		// password
	};

	StartScreen(TextureManager* pTextureManager, SoundManager* pSoundManager);
	StartScreen(const StartScreen& other) = delete;
	StartScreen& operator=(const StartScreen& other) = delete;
	StartScreen(StartScreen&& other) = delete;
	StartScreen& operator=(StartScreen&& other) = delete;
	virtual ~StartScreen();

	virtual void Draw(float camScale, SoundManager* pSoundManager, int playerLastScore, int playerHighScore) const override;
	virtual void Update(float elapsedSec, int playeramountOfContinues) override;

	virtual void ButtonInput(Menu::ButtonPressed pressedButton, SoundManager* pSoundManager) override;

	virtual bool HasFinished() override;

	virtual int GetAmountOfLives() override;
	
	virtual void Reset() override;

private:
	StartScreenPhases m_CurrentPhase;
	
	Sprite* m_pPointer;

	Sprite* m_pTitleLogo;
	Sprite* m_pPressStart;
	SoundStream* m_pIntroTitleSoundStream;

	Sprite* m_pOptionButton;
	Sprite* m_pPasswordButton;
	Sprite* m_pStartGameButton;

	Sprite* m_pOptionsMenuBackGround;
	Sprite* m_pOptionsMenuText;
	Sprite* m_pOptionNumbers;
	const Point2f m_PlayerAmountOfLivesPosition;
	const Point2f m_VolumePosition;
	int m_AmountOfLives;

	std::vector<Point2f> m_BloodDropStartPositions;
	std::vector<Point2f> m_BloodDropCurrentPositions;
	std::vector<Point2f> m_BloodDropEndPositions;
	std::vector<Sprite*> m_pBloodDropSprites;


};

