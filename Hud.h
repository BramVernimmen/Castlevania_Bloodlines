#pragma once
class Sprite;
class TextureManager;
class SoundManager;
class Player;
class Level;
class BossManager;
class Hud final
{
public:
	Hud(const Window& window, float camScale, TextureManager* pTextureManager);
	Hud(const Hud& other) = delete;
	Hud& operator=(const Hud& rhs) = delete;
	Hud(Hud&& other) = delete;
	Hud& operator=(Hud&& rhs) = delete;
	~Hud();


	void Draw(Player* pPlayer, Level* pCurrentLevel, BossManager* pBossManager, int currentLevelIndex) const;
	void DrawPause(SoundManager* pSoundManager) const;
private:
	const Color4f m_HudColor;
	const Rectf m_HudRect;
	const float m_CamScale;
	Sprite* m_pHudElementsNonChangable;
	

	Sprite* m_pSubWeaponBoomerang;
	Sprite* m_pSubWeaponBattleAxe;
	Sprite* m_pSubWeaponSacredWater;

	Sprite* m_pHealthBar;

	Sprite* m_pHudNumbers;
	const int m_AmountOfHealthBars;
	const Point2f m_PlayerHealthBarPosition;
	const Point2f m_EnemyHealthBarPosition;
	const Point2f m_ScorePosition;
	const Point2f m_GemsPosition;
	const Point2f m_CurrentLevelPosition;
	const Point2f m_CurrentStagePosition;
	const Point2f m_PlayerAmountOfLivesPosition;

	Sprite* m_pPause;
	Sprite* m_pNumbers;
	Sprite* m_pVolume;

	const Point2f m_VolumePosition;
};

