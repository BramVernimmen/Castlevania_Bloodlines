#pragma once
#include "Object.h"
class Sprite;
class TextureManager;
class RandomSkull final : public Object
{
public:
	RandomSkull(const Point2f& drawPos, TextureManager* pTextureManager);
	~RandomSkull();
	RandomSkull(const RandomSkull& other) = delete;
	RandomSkull& operator=(const RandomSkull& rhs) = delete;
	RandomSkull(RandomSkull&& other) = delete;
	RandomSkull& operator=(RandomSkull&& rhs) = delete;

	virtual void Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle) override;
	virtual void Draw() const override;


private:
	Sprite* m_pSprite;
	//Point2f m_DrawPos;
	float m_Angle;

};

