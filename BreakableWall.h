#pragma once
#include "Object.h"
#include "DropManager.h"
#include <vector>
class Sprite;
class TextureManager;

class SoundManager;
class SoundEffect;

class BreakableWall final : public Object
{
public:
	enum class WallType
	{
		pillar,
		wall
	};
	BreakableWall(const Point2f& bottomLeft, WallType typeOfWall, int amountOfBlocks, Drop::DropType dropType, TextureManager* pTextureManager, SoundManager* pSoundManager);
	virtual ~BreakableWall();
	BreakableWall(const BreakableWall& other) = delete;
	BreakableWall& operator=(const BreakableWall& rhs) = delete;
	BreakableWall(BreakableWall&& other) = delete;
	BreakableWall& operator=(BreakableWall&& rhs) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle) override;
	virtual void IsHit(const std::vector<Rectf>& playerAttackHitBox, const Rectf& playerHitBox, int playerLevel, DropManager* dropList, TextureManager* pTextureManager, SoundManager* pSoundManager) override;

private:
	void HandleCollision(Rectf& actorHitBox, Vector2f& actorVelocity, int index) const;
	bool CheckAllDestroyed() const;
	std::vector<Point2f> m_DrawPosWall;
	std::vector<Sprite*> m_SpritesWall;
	std::vector<bool> m_IsDestroyed;
	std::vector<Rectf> m_HitBoxes;
	int m_AmountOfBlocks; 
	Drop::DropType m_DropType;
	bool m_HasDropped;
	SoundEffect* m_pDestroyedSoundEffect;
};

