#include "pch.h"
#include "Drop.h"
#include "Sprite.h"
#include "utils.h"
#include "Level.h"
#include "Player.h"
#include "SoundManager.h"
#include "SoundEffect.h"

#include <iostream>

Drop::Drop(const Point2f& pos, Drop::DropType type, int value, int playerLevel, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: m_Type{ type }
	, m_DrawPos{pos}
	, m_IsDestroyed{false}
	, m_Timer{}
	, m_Velocity{ Vector2f{0.f, 0.f} }
	, m_Acceleration{ Vector2f{0.f, -981.f} }
	, m_Value{value}
	, m_PlayerLevel{playerLevel}
	, m_pPickupSoundEffect{nullptr}
{
	switch (m_Type)
	{
	case Drop::DropType::redJewel:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/RedJewel.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/GemPickup.mp3");;
		break;
	case Drop::DropType::blueJewel:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/BlueJewel.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/GemPickup.mp3");;
		break;
	case Drop::DropType::bag:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/Bag_01.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/BagPickup.mp3");;
		break;
	case Drop::DropType::muttonChop:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/Mutton_01.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/MuttonPickup.mp3");;
		break;
	case Drop::DropType::mirrorOfThruth:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/MirrorOfThruth.png", 2, 1, 5, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/MirrorPickup.mp3");;
		break;
	case Drop::DropType::oneUp:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/1Up.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/BagPickup.mp3");;
		break;
	case Drop::DropType::orb:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/Orb.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/BagPickup.mp3");;
		break;
	case Drop::DropType::coatOfArms:
		if (m_PlayerLevel >= 4) // the player will get a red jewel
		{
			m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/RedJewel.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
			m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/GemPickup.mp3");;
		}
		else // if the playerlevel is smaller than 4, the player will get a coat of arms
		{
			m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/CoatOfArms_01.png", 4, 1, 9, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
			m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/CoatOfArmsPickup.mp3");;
		}
		//m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/CoatOfArms_01.png", 4, 1, 9, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		break;
	case Drop::DropType::specialCoatOfArms:
		if (m_PlayerLevel >= 3) // the player will get a red jewel
		{
			m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/RedJewel.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
			m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/GemPickup.mp3");;
		}
		else // if the playerlevel is smaller than 3, the player will get a coat of arms
		{
			m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/CoatOfArms_01.png", 4, 1, 9, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
			m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/CoatOfArmsPickup.mp3");;
		}
		break;
	case Drop::DropType::spellBook:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/SpellBook.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/SpellBookPickup.mp3");;
		break;
	case Drop::DropType::sacredWater:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/SacredWater.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/SubWeaponPickup.mp3");;
		break;
	case Drop::DropType::battleAxe:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/BattleAxe.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/SubWeaponPickup.mp3");;
		break;
	case Drop::DropType::boomerang:
		m_pSprite = new Sprite("Resources/Sprites/Objects/Drops/Boomerang.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
		m_pPickupSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/SubWeaponPickup.mp3");;
		break;
	}


	m_SpriteHitBox.left = m_DrawPos.x;
	m_SpriteHitBox.bottom = m_DrawPos.y;
	m_SpriteHitBox.width = m_pSprite->GetFrameWidth();
	m_SpriteHitBox.height = m_pSprite->GetFrameHeight();
}

Drop::~Drop()
{
	delete m_pSprite;
	m_pSprite = nullptr;
}

void Drop::Update(float elapsedSec, const Level* level)
{
	if (m_IsDestroyed == false)
	{
		// update pos  (gravity)
		// check with level collision

		level->HandleCollision(m_SpriteHitBox, m_Velocity); 

		m_Velocity.y += m_Acceleration.y * elapsedSec;

		m_SpriteHitBox.left += m_Velocity.x * elapsedSec;
		m_SpriteHitBox.bottom += m_Velocity.y * elapsedSec;

		m_DrawPos.x = m_SpriteHitBox.left;
		m_DrawPos.y = m_SpriteHitBox.bottom;

		m_pSprite->SetDrawPos(m_DrawPos);
		m_Timer += elapsedSec;
		m_pSprite->Update(elapsedSec);
	}

	if (m_Timer >= 5.f)
	{
		m_IsDestroyed = true;
	}
}

void Drop::Draw() const
{
	if (m_IsDestroyed == false && m_Timer <= 3.f)
	{
		m_pSprite->Draw();
	}
	else if (m_IsDestroyed == false && m_Timer >= 3.f)
	{
		if (int(m_Timer * 10.f) % 2 == 0)
		{
			m_pSprite->Draw();
		}

	}
}

bool Drop::IsHit(const Rectf& playerHitBox , Player* player)
{
	if (utils::IsOverlapping(playerHitBox, m_SpriteHitBox) == true && m_IsDestroyed == false)
	{
		m_IsDestroyed = true;
		player->AddScore(m_Value);
		//std::cout << "Drop has been collected" << std::endl;
		m_pPickupSoundEffect->Play(0);
		switch (m_Type)
		{
		case Drop::DropType::redJewel:
			player->AddGems(1);
			break;
		case Drop::DropType::blueJewel:
			player->AddGems(2);
			break;
		case Drop::DropType::bag:
			break;
		case Drop::DropType::muttonChop:
			player->AddHealth(10);
			break;
		case Drop::DropType::mirrorOfThruth:
			break;
		case Drop::DropType::oneUp:
			break;
		case Drop::DropType::orb:
			player->SetInvincible();
			break;
		case Drop::DropType::coatOfArms:
			player->LevelUp();
			break;
		case Drop::DropType::specialCoatOfArms:
			if (m_PlayerLevel >= 3) // the player will get a red jewel
			{
				player->AddGems(1);
			}
			else // if the playerlevel is smaller than 3, the player will get a coat of arms
			{
				player->LevelUp();
			}
			break;
		case Drop::DropType::spellBook:
			player->LevelUp();
			break;
		case Drop::DropType::sacredWater:
			player->SetSubWeapon(Player::CurrentSubWeapon::sacredWater);
			break;
		case Drop::DropType::battleAxe:
			player->SetSubWeapon(Player::CurrentSubWeapon::battleAxe);
			break;
		case Drop::DropType::boomerang:
			player->SetSubWeapon(Player::CurrentSubWeapon::boomerang);
			break;
		}
		return true;
	}
	return false;
}
