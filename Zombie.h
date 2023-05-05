#pragma once
#include "Enemy.h"
//#include <vector>
//#include "Vector2f.h"
class Sprite;
class TextureManager;
class Zombie final : public Enemy
{
public:
	Zombie(bool repeatSpawning, TextureManager* pTextureManager, SoundManager* pSoundManager);
	virtual ~Zombie();
	Zombie(const Zombie& other) = delete;
	Zombie& operator=(const Zombie& rhs) = delete;
	Zombie(Zombie&& other) = delete;
	Zombie& operator=(Zombie&& rhs) = delete;

	virtual void Update(float elapsedSec, Point2f playerPos, float levelWidth) override;
	virtual void Draw()const override;


private:
	bool m_RepeatSpawn;
	bool m_CanSpawn;
	bool m_HasSpawned;
	bool m_HasFly;

	float m_OriginalHealth;

	Sprite* m_pSpriteMoving;
	Sprite* m_pSpriteDying;
	Sprite* m_pSpriteBloodParticles;
	Sprite* m_pSpriteFly;

	float m_FlyAngle;
	Point2f m_FlyPos;

	float m_RespawnTimer;
	float m_MaxRespawnTimer;

	float m_MovementScale;
	// need to use m_MovementSpeed...
};

