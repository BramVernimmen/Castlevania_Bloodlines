#pragma once
#include "Vector2f.h"
#include "utils.h"
#include <vector>
class Sprite;
class SoundManager;
class SoundEffect;

class Enemy
{
public:
	enum class ActionState
	{
		idle,
		sleeping,
		waking,
		moving,
		attacking,
		dying,
		none
	};
	Enemy(SoundManager* pSoundManager);
	Enemy(const Enemy& other) = delete;
	Enemy& operator=(const Enemy& rhs) = delete;
	Enemy(Enemy&& other) = delete;
	Enemy& operator=(Enemy&& rhs) = delete;
	virtual ~Enemy() = default;

	virtual void Update(float elapsedSec, Point2f playerPos, float levelWidth) = 0;
	virtual void Draw() const = 0;
	//virtual void IsHit(Rectf playerAttackHitBox, float playerDamage);
	virtual void IsHit(std::vector<Rectf> playerAttackHitBox, float playerDamage);
	virtual bool IsDead();
	virtual bool HitPlayer(Rectf playerHitBox);
	virtual Rectf& GetHitBox();
	virtual Vector2f& GetVelocity();
	virtual int GetValue();
	virtual float GetPointOfContact();
	virtual float GetDamage();
	virtual bool GetCollideWithStage();
	virtual void SetOnGround(bool isOnGround);

protected:
	virtual void DrawSprite(Sprite* pSpriteToDraw) const;

	bool m_CanCollideWithStage;
	float m_Damage;
	float m_PointOfContact;
	int m_Value;
	Vector2f m_Velocity;
	Rectf m_HitBox;
	ActionState m_CurrentState;
	bool m_IsInvincible;
	float m_CurrentInvinsibleTimer;
	float m_MaxInvinsibleTimer;
	float m_Health;
	std::vector<Point2f> m_HitBoxVertices;
	bool m_HasGivenPoints;
	Point2f m_CenterPoint;
	bool m_IsLookingRight;
	float m_MovementSpeed;
	Point2f m_DrawPos;
	bool m_IsOnGround;

	SoundEffect* m_pHitSoundEffect;

private:

};

