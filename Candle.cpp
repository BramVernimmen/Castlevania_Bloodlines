#include "pch.h"
#include "Candle.h"
#include "Sprite.h"
#include "utils.h"
#include "SoundManager.h"
#include "SoundEffect.h"

Candle::Candle(const Point2f& drawPos, Type type, Drop::DropType dropType, int value, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Object{drawPos}
	, m_IsDestroyed{ false }
	//, m_DrawPos{ drawPos }
	, m_DropType{dropType}
	, m_DropValue{value}
{
	switch (type)
	{
	case Candle::Type::Outside:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Candles/Candle1_1.png", 3, 1, 24, drawPos, Sprite::SpriteType::Other, pTextureManager);
		break;
	case Candle::Type::InsideRand:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Candles/Candle2.png", 15, 1, 10, drawPos, Sprite::SpriteType::Other, pTextureManager);
		break;
	case Candle::Type::InsideWeapon:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Candles/Candle3.png", 15, 1, 12, drawPos, Sprite::SpriteType::Other, pTextureManager);
		break;
	}

	m_pDestroyedSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/CandleDestroy.mp3");
	m_SpriteHitBox.left = m_DrawPos.x;
	m_SpriteHitBox.bottom = m_DrawPos.y;
	m_SpriteHitBox.width = m_pSprite->GetFrameWidth();
	m_SpriteHitBox.height = m_pSprite->GetFrameHeight();
}

Candle::~Candle()
{
	delete m_pSprite;
	m_pSprite = nullptr;
}

void Candle::Draw() const
{
	if (m_IsDestroyed == false)
	{
		m_pSprite->Draw();
	}
}

void Candle::Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle)
{
	if (m_IsDestroyed == false)
	{
		m_pSprite->Update(elapsedSec);
	}
}

void Candle::IsHit(const std::vector<Rectf>& playerAttackHitBox, const Rectf& playerHitBox, int playerLevel, DropManager* dropList, TextureManager* pTextureManager, SoundManager* pSoundManager)
{
	for (size_t i{ 0 }; i < playerAttackHitBox.size(); ++i)
	{
		if (utils::IsOverlapping(playerAttackHitBox[i], m_SpriteHitBox) == true && m_IsDestroyed == false)
		{
			m_IsDestroyed = true;
			
			dropList->AddItem(m_DrawPos, m_DropType, m_DropValue, playerLevel, pTextureManager, pSoundManager);
			m_pDestroyedSoundEffect->Play(0);

		}
	}


}
