#pragma once
#include "Vector2f.h"
#include "utils.h"
#include <vector>
#include "SoundManager.h"
class Sprite;

class Boss
{
public:
	enum class ActionState
	{
		spawning,
		moving,
		idle,
		jumping,
		attacking,
		dying,
		despawning,
		none
	};
	Boss(SoundManager* pSoundManager, const std::string& streamPath);
	Boss(const Boss& other) = delete;
	Boss& operator=(const Boss& rhs) = delete;
	Boss(Boss&& other) = delete;
	Boss& operator=(Boss&& rhs) = delete;
	virtual ~Boss() = default;

	virtual void Update(float elapsedSec, Point2f playerPos) = 0;
	virtual void Draw() const = 0;
	virtual void IsHit(std::vector<Rectf> playerAttackHitBox, float playerDamage);
	virtual bool IsDead();
	virtual bool HitPlayer(Rectf playerHitBox);
	virtual Rectf& GetHitBox();
	virtual Vector2f& GetVelocity();
	virtual int GetValue();
	virtual float GetPointOfContact();
	virtual float GetDamage();
	virtual float GetCurrentHealth();
	virtual float GetMaximumHealth();

	virtual ActionState GetCurrentState();
	virtual bool HasGivenPoints();

	void PlayBossMusic();
	void StopBossMusic();

protected:
	virtual void DrawSprite(Sprite * pSpriteToDraw) const;

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
	float m_MaximumHealth;
	std::vector<Point2f> m_HitBoxVertices;
	bool m_HasGivenPoints; // if true, boss has been defeated
	Point2f m_CenterPoint;
	bool m_IsLookingRight;
	float m_MovementSpeed;
	Point2f m_DrawPos;

	float m_VerticalTrigger;

private:
	SoundStream* m_pBossSoundStream;
	SoundEffect* m_pHitSoundEffect;

};

