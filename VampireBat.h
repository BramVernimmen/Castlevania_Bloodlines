#pragma once
#include "Enemy.h"
//#include <vector>
//#include "Vector2f.h"

class Sprite;
class TextureManager;
class VampireBat final : public Enemy
{
public:
	VampireBat(bool canRepeat, Point2f drawPos, TextureManager* pTextureManager, SoundManager* pSoundManager);
	virtual ~VampireBat();
	VampireBat(const VampireBat& other) = delete;
	VampireBat& operator=(const VampireBat& rhs) = delete;
	VampireBat(VampireBat&& other) = delete;
	VampireBat& operator=(VampireBat&& rhs) = delete;

	virtual void Update(float elapsedSec, Point2f playerPos, float levelWidth) override;
	virtual void Draw()const override;
	


private:
	void UpdatePos();

	bool m_RepeatSpawn;
	bool m_CanSpawn;
	bool m_HasSpawned;

	float m_OriginalHealth;

	Sprite* m_pSpriteMoving;
	Sprite* m_pSpriteDying;
	Sprite* m_pSpriteIdle;

	//float m_Angle;  will see if needed

	float m_RespawnTimer;
	float m_MaxRespawnTimer;

	float m_Angle;
	float m_AngleIncrement;
};

