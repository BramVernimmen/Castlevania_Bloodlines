#pragma once
//#include "DropManager.h"
class Player;
class Level;
#include "Vector2f.h"
//class DropManager;
class Sprite;
class TextureManager;
class SoundManager;

class SoundEffect;

class Drop final
{
public:

	enum class DropType
	{
		redJewel,
		blueJewel,
		bag,
		muttonChop,
		mirrorOfThruth,
		oneUp,
		orb,
		coatOfArms,
		specialCoatOfArms,
		spellBook,
		sacredWater,
		battleAxe,
		boomerang

	
	};

	explicit Drop(const Point2f& pos, Drop::DropType type, int value, int playerLevel, TextureManager* pTextureManager, SoundManager* pSoundManager);
	~Drop();
	Drop(const Drop& other) = delete;
	Drop& operator=(const Drop& rhs) = delete;
	Drop(Drop&& other) = delete;
	Drop& operator=(Drop&& rhs) = delete;

	void Update( float elapsedSec, const Level* level);
	void Draw( ) const;
	bool IsHit(const Rectf& playerHitBox,  Player* player);
	bool m_IsDestroyed;

private:
	Point2f m_DrawPos;
	Rectf m_SpriteHitBox;
	float m_Timer;
	DropType m_Type;
	int m_Value;
	int m_PlayerLevel;

	Vector2f m_Velocity;
	Vector2f m_Acceleration;

	Sprite* m_pSprite;
	SoundEffect* m_pPickupSoundEffect;
};


