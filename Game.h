#pragma once
#include <vector>
#include "Level.h"
#include "MenuManager.h"
class Camera;
class Player;
class DropManager;
class LevelManager;
class EnemyManager;
class BossManager;
class ObjectManager;
class Hud;

class TextureManager;
class SoundManager;
class SoundEffect;

class Game final
{
public:
	enum class GameState
	{
		InGame,
		Paused,
		Transitioning, 
		InMenu
	};

	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

private:
	// DATA MEMBERS
	const Window m_Window;

	Camera* m_pCamera;
	Player* m_pPlayer;
	Hud* m_pHud;
	SoundEffect* m_pPauseSoundEffect;
	SoundEffect* m_pPlayerDeathSoundEffect;

	DropManager* m_pDropManager;
	LevelManager* m_pLevelManager;
	EnemyManager* m_pEnemyManager;
	BossManager* m_pBossManager;
	ObjectManager* m_pObjectManager;

	TextureManager* m_pTextureManager;
	SoundManager* m_pSoundManager;

	MenuManager* m_pMenuManager;

	GameState m_CurrentGameState;

	int 
		m_CurrentLevel,
		m_CurrentStage,
		m_NextStage;

	Level::TransitionType m_CurrentLevelTransitionType;
	Color4f m_FadeToBlackColor;

	float m_CamWidth;
	float m_CamScale;
	float m_CamShift;

	bool m_HasLevelStarted;

	MenuManager::MenuType m_CurrentMenuType;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
	void PrintInfo() const;

	void ResetLevelTransition();
	void ResetAfterDeath();

	void DrawHud() const;
	void DrawCurrentLevel() const;
	



	// for testing backgrounds
	Rectf m_testRectf{150.f, 50.f, 30.f, 50.f};
	void UpdateTestRect(float elapsedSec);
	void CheckTestRect();
};