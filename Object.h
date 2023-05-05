#pragma once
//class Sprite;
class Level;
#include "Vector2f.h"
#include <vector>
class DropManager;
class TextureManager;
class SoundManager;

class Object
{
public:
	Object(const Point2f& drawPos);
	Object(const Object& other) = delete;
	Object& operator=(const Object& rhs) = delete;
	Object(Object&& other) = delete;
	Object& operator=(Object&& rhs) = delete;
	virtual ~Object() = default;

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle) = 0;
	virtual void IsHit(const std::vector<Rectf>& playerAttackHitBox, const Rectf& playerHitBox, int playerLevel, DropManager* dropList, TextureManager* pTextureManager, SoundManager* pSoundManager);

protected:
	Point2f m_DrawPos;

};

