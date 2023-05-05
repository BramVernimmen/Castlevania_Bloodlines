#pragma once
#include <vector>
class SoundManager;
class SoundEffect;

class Menu
{
public:

	enum class ButtonPressed
	{
		up,
		down,
		left,
		right,
		start
	};

	Menu(SoundManager* pSoundManager);
	Menu(const Menu& other) = delete;
	Menu& operator=(const Menu& other) = delete;
	Menu(Menu&& other) = delete;
	Menu& operator=(Menu&& other) = delete;
	virtual ~Menu() = default;

	virtual void Draw(float camScale, SoundManager* pSoundManager, int playerLastScore, int playerHighScore) const = 0;
	virtual void Update(float elapsedSec, int playeramountOfContinues) = 0;

	virtual void ButtonInput(Menu::ButtonPressed pressedButton, SoundManager* pSoundManager) = 0;

	virtual bool HasFinished() = 0;
	virtual bool ResetGame();

	virtual int GetAmountOfLives();

	virtual void Reset() = 0;
protected:
	int m_CurrentSelectionIndex;
	std::vector<Point2f> m_SelectionPositions;
	float m_ButtonFlicker;
	bool m_CountDownStarted;

	SoundEffect* m_pPointerMoveSoundEffect;
	SoundEffect* m_pSelectedSoundEffect;
};

