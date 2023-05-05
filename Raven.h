#pragma once
#include "Object.h"
#include <vector>
class TextureManager;
class SoundManager;
class SoundEffect;

class Sprite;
class Raven final : public Object
{

public:
	enum class ActionState
	{
		idle,
		moving,
		dying
	};
	Raven(const Point2f& drawPos, bool isLookingRight, TextureManager* pTextureManager, SoundManager* pSoundManager);
	virtual ~Raven();	
	Raven(const Raven& other) = delete;
	Raven& operator=(const Raven& rhs) = delete;
	Raven(Raven&& other) = delete;
	Raven& operator=(Raven&& rhs) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle) override;
	virtual void IsHit(const std::vector<Rectf>& playerAttackHitBox, const Rectf& playerHitBox, int playerLevel, DropManager* dropList, TextureManager* pTextureManager, SoundManager* pSoundManager) override;

private:
	//Point2f m_DrawPos;
	ActionState m_CurrentState;

	std::vector<Point2f> m_FeatherPos;
	float m_featherAngle;
	float m_featherAngleIncrement;
	float m_MovementSpeed;

	Sprite* m_pSpriteIdle;
	Sprite* m_pSpriteMoving;
	Sprite* m_pSpriteDying;
	Sprite* m_pSpriteFeather;
	

	bool m_IsLookingRight;
	bool m_IsMovingRight;

	float m_EatingTimer;
	float m_RandomEatingTimer;

	SoundEffect* m_pFlyAwaySoundeffect;
};

