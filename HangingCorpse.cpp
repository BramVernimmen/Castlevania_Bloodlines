#include "pch.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "HangingCorpse.h"


HangingCorpse::HangingCorpse(const Point2f& drawPos, bool spriteMoves, TextureManager* pTextureManager)
	: Object{ drawPos }
	, m_SpritesMoves{spriteMoves}
{
	m_pSprite = new Sprite("Resources/Sprites/Objects/HangingCorpse/HangingCorpse.png", 4, 1, 4, drawPos, Sprite::SpriteType::Other, pTextureManager);
	if (spriteMoves == false)
	{
		m_pSprite->SetCurrentFrame(1);
	}
	else
	{
		m_pSprite->SetCurrentFrame(rand() % m_pSprite->GetTotalAmountOfFrames());
	}
}

HangingCorpse::~HangingCorpse()
{
	delete m_pSprite;
	m_pSprite = nullptr;
}

void HangingCorpse::Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle)
{
	if (m_SpritesMoves == true)
	{
		m_pSprite->Update(elapsedSec);
	}
}

void HangingCorpse::Draw() const
{
	m_pSprite->Draw();
}