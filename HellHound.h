#pragma once
#include "Boss.h"
class SoundManager;
class TextureManager;
class Sprite;
class HellHound final : public Boss
{
public:
	HellHound(float verticalTrigger, TextureManager* pTextureManager, SoundManager* pSoundManager);
	virtual ~HellHound();
	HellHound(const HellHound& other) = delete;
	HellHound& operator=(const HellHound& rhs) = delete;
	HellHound(HellHound&& other) = delete;
	HellHound& operator=(HellHound&& rhs) = delete;

	virtual void Update(float elapsedSec, Point2f playerPos) override;
	virtual void Draw()const override;


private:
	void UpdatePosition();
	Sprite* m_pSpriteMoving;
	Sprite* m_pSpriteDying;
	Sprite* m_pSpriteDespawning;
	Sprite* m_pSpriteIdle;
	Sprite* m_pSpriteAttackingFire;
	Sprite* m_pSpriteSmallFlame;
	Sprite* m_pSpriteBigFlame;

	std::vector<Point2f> m_FlamePositions;
	int m_AmountOfFlames;

	float m_RightWallPosition;
	float m_DespawnTimer;
	float m_OriginalDespawnTimer;

	float m_IdleWaitTimer;

	float m_AttackingTimer;
	float m_OriginalAttackingTimer;
	float m_Angle;

	float m_AngleCosine;
	float m_AngleCosineScale;

};

