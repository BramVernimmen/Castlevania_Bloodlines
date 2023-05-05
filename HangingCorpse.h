#pragma once
#include "Object.h"
class Sprite;
class TextureManager;
class HangingCorpse final : public Object
{
public:
	HangingCorpse(const Point2f& drawPos, bool spriteMoves, TextureManager* pTextureManager);
	~HangingCorpse();
	HangingCorpse(const HangingCorpse& other) = delete;
	HangingCorpse& operator=(const HangingCorpse& rhs) = delete;
	HangingCorpse(HangingCorpse&& other) = delete;
	HangingCorpse& operator=(HangingCorpse&& rhs) = delete;

	virtual void Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle) override;
	virtual void Draw() const override;


private:
	Sprite* m_pSprite;
	bool m_SpritesMoves;

};

