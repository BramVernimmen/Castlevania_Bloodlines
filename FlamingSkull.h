#pragma once
#include "Enemy.h"
class Sprite;
class TextureManager;
class FlamingSkull final : public Enemy
{
public:
	FlamingSkull(const Point2f& drawPos, TextureManager* pTextureManager, SoundManager* pSoundManager);
	virtual ~FlamingSkull();
	FlamingSkull(const FlamingSkull& other) = delete;
	FlamingSkull& operator=(const FlamingSkull& rhs) = delete;
	FlamingSkull(FlamingSkull&& other) = delete;
	FlamingSkull& operator=(FlamingSkull&& rhs) = delete;

	virtual void Update(float elapsedSec, Point2f playerPos, float levelWidth) override;
	virtual void Draw() const override;

private:
	void UpdatePos();
	Sprite* m_pSpriteSpawning;
	Sprite* m_pSpriteMoving;

	float m_Angle;
	float m_AngleIncrement;
};

