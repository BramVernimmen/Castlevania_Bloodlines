#pragma once
#include "Object.h"
class Sprite;
class Player;
class Level;
class TextureManager;
class SoundManager;
class SoundEffect;
#include "DropManager.h"


class Candle final : public Object
{

public:
	enum class Type 
	{
		Outside = 1,
		InsideRand = 2, 
		InsideWeapon = 3
	};
	Candle(const Point2f& drawPos, Type type, Drop::DropType dropType, int value, TextureManager* pTextureManager, SoundManager* pSoundManager);
	virtual ~Candle();
	Candle(const Candle& other) = delete;
	Candle& operator=(const Candle& rhs) = delete;
	Candle(Candle&& other) = delete;
	Candle& operator=(Candle&& rhs) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle) override;
	virtual void IsHit( const std::vector<Rectf>& playerAttackHitBox, const Rectf& playerHitBox, int playerLevel, DropManager* dropList, TextureManager* pTextureManager, SoundManager* pSoundManager) override;

private:
	//Point2f m_DrawPos;
	bool m_IsDestroyed;
	Rectf m_SpriteHitBox;
	Drop::DropType m_DropType;
	int m_DropValue;
	Sprite* m_pSprite;
	SoundEffect* m_pDestroyedSoundEffect;
};

