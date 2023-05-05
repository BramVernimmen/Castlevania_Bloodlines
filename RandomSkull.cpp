#include "pch.h"
#include "RandomSkull.h"
#include "Sprite.h"
#include "TextureManager.h"

RandomSkull::RandomSkull(const Point2f& drawPos, TextureManager* pTextureManager)
	: Object{ drawPos }
	, m_Angle{0.f}
	, m_pSprite{new Sprite("Resources/Sprites/Objects/RandomSkull/RandomSkull_01.png", 11, 1, 8, drawPos, Sprite::SpriteType::Other, pTextureManager)}
{
}

RandomSkull::~RandomSkull()
{
	delete m_pSprite;
	m_pSprite = nullptr;
}

void RandomSkull::Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle)
{
	m_pSprite->Update(elapsedSec);
	m_Angle += 0.5f;
	m_DrawPos.x += cosf(m_Angle) / 5.f;
	m_DrawPos.y += sinf(m_Angle) / 5.f;
	m_pSprite->SetDrawPos(m_DrawPos);
}

void RandomSkull::Draw() const
{
	m_pSprite->Draw();
}
