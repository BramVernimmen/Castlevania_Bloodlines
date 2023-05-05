#pragma once
#include "Enemy.h"
class TextureManager;
class Sprite;
class FishMan final : public Enemy
{
public:
	FishMan(bool repeatSpawning, TextureManager* pTextureManager, SoundManager* pSoundManager);
	virtual ~FishMan();
	FishMan(const FishMan& other) = delete;
	FishMan& operator=(const FishMan& rhs) = delete;
	FishMan(FishMan&& other) = delete;
	FishMan& operator=(FishMan&& rhs) = delete;

	virtual void Update(float elapsedSec, Point2f playerPos, float levelWidth) override;
	virtual void Draw() const override;

private:
	void UpdatePos();

	bool m_HasAttacked;
	float m_OriginalHealth;

	bool m_RepeatSpawn;
	bool m_CanSpawn;
	bool m_HasSpawned;

	float m_RespawnTimer;
	float m_MaxRespawnTimer;

	float m_JumpSpeed;

	Sprite* m_pSpriteMoving;
	Sprite* m_pSpriteSpawning;
	Sprite* m_pSpriteDying;
	Sprite* m_pSpriteAttacking;

	Vector2f m_Acceleration;

	bool m_IsWalking;

	float m_WalkWaitTimer;
	float m_WalkMaxTimer;

	float m_CurrentTimer;
	float m_AttackMaxTimer;

};

