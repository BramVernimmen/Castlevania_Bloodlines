#pragma once
#include "Menu.h"
class TextureManager;
class SoundManager;
class MenuManager final
{
public:
	enum class MenuType
	{
		startScreen,
		startLevel,
		gameOver
	};



	MenuManager(TextureManager* pTextureManager, SoundManager* pSoundManager);
	MenuManager(const MenuManager& other) = delete;
	MenuManager& operator=(const MenuManager& other) = delete;
	MenuManager(MenuManager&& other) = delete;
	MenuManager& operator=(MenuManager&& other) = delete;
	~MenuManager();

	void Update(float elapsedSec, MenuType currentMenuType, int playeramountOfContinues);
	void Draw(MenuType currentMenuType, float camScale, SoundManager* pSoundManager, int playerLastScore, int playerHighScore) const;

	bool CanPlayLevel();
	void SetCanPlayLevel();

	bool ResetGame();

	int GetAmountOfPlayerLives();

	void Reset();

	void ButtonInput(Menu::ButtonPressed pressedButton, MenuType currentMenuType, SoundManager* pSoundManager);

private:
	bool m_CanPlayLevel;
	
	Menu* m_pStartScreen;
	Menu* m_pGameOver;



	// menu base class
	// derived classes:
	// start screen
	// start level
	// game over?
	// 
	// in start screen
	//		enum:
	//		konami
	//		story
	//		intro (press start)
	//		main menu
	//		(holding a value of where the arrow is, which title is selected, standard 0, after pressing, reset to 0)
	//		options  --> hold vars in manager, use for initializing
	//		character select  -> case animation is finished -> set a variable saying finished
	//		finished 
	//  
	// when finished -> change currentMenuType
	// 
	//	in start level
	//		enum:
	//		map intro
	//		level 1 intro
	//		
	//	in game over
	//		if continues left = 0 -> only end option, else draw continue and end options
	//
};

