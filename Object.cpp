#include "pch.h"
#include "Object.h"

Object::Object(const Point2f& drawPos)
	: m_DrawPos{drawPos}
{

}



void Object::IsHit(const std::vector<Rectf>& playerAttackHitBox, const Rectf& playerHitBox, int playerLevel, DropManager* dropList, TextureManager* pTextureManager, SoundManager* pSoundManager)
{
	// do nothing
}

