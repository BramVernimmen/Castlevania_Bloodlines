#pragma once
#include "Object.h"

class Sprite;
class TextureManager;

class SoundManager;
class SoundEffect;

class Door final : public Object
{
public:
	Door(const Point2f& drawPos, TextureManager* pTextureManager, SoundManager* pSoundManager);
	virtual ~Door();
	Door(const Door& other) = delete;
	Door& operator=(const Door& rhs) = delete;
	Door(Door&& other) = delete;
	Door& operator=(Door&& rhs) = delete;

	virtual void Draw()const override;
	virtual void Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle) override;
	virtual void IsHit(const std::vector<Rectf>& playerAttackHitBox, const Rectf& playerHitBox, int playerLevel, DropManager* dropList, TextureManager* pTextureManager, SoundManager* pSoundManager) override;

private:
	//Point2f m_DrawPos;
	Point2f m_TriggerPos;
	Sprite* m_pSpriteDoor;
	float m_CurrentTimer;
	float m_MaxTimer;
	float m_MaxOpenTimer;
	int m_CurrentFrame;
	bool m_FrameUp;
	bool m_CanOpen;

	bool m_PlayedSoundOnce;
	SoundEffect* m_pOpenCloseSoundEffect;
};

