#include "pch.h"
#include "BreakableWall.h"
#include "Sprite.h"
#include "utils.h"

#include "SoundManager.h"
#include "SoundEffect.h"

BreakableWall::BreakableWall(const Point2f& bottomLeft, WallType typeOfWall, int amountOfBlocks, Drop::DropType dropType, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Object{ bottomLeft }
	, m_DropType{dropType}
	, m_HasDropped{false}
	, m_AmountOfBlocks{amountOfBlocks}
	
{
	m_pDestroyedSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/WallBreaking.mp3");

	switch (typeOfWall)
	{
	case BreakableWall::WallType::pillar:
		m_AmountOfBlocks = 3;
		for (int i{0}; i < m_AmountOfBlocks; ++i)
		{
			m_SpritesWall.push_back(new Sprite("Resources/Sprites/Objects/BreakableWall/BreakableWall_01.png", 3, 1, 1, bottomLeft, Sprite::SpriteType::Other, pTextureManager));
			m_DrawPosWall.push_back(Point2f{ bottomLeft.x, bottomLeft.y + m_SpritesWall[i]->GetFrameHeight() * i });
			m_SpritesWall[i]->SetDrawPos(m_DrawPosWall[i]);
			m_SpritesWall[i]->SetCurrentFrame(i);
			m_IsDestroyed.push_back(false);
			m_HitBoxes.push_back(Rectf{ m_DrawPosWall[i].x, m_DrawPosWall[i].y, m_SpritesWall[i]->GetFrameWidth(), m_SpritesWall[i]->GetFrameHeight() });
		}
		break;
	case BreakableWall::WallType::wall:
		if (m_AmountOfBlocks == 4)
		{
			for (int i{ 0 }; i < m_AmountOfBlocks; ++i)
			{
				m_SpritesWall.push_back(new Sprite("Resources/Sprites/Objects/BreakableWall/BreakableWall_02.png", m_AmountOfBlocks, 1, 1, bottomLeft, Sprite::SpriteType::Other, pTextureManager));
				m_DrawPosWall.push_back(Point2f{ bottomLeft.x, bottomLeft.y + m_SpritesWall[i]->GetFrameHeight() * i });
				m_SpritesWall[i]->SetDrawPos(m_DrawPosWall[i]);	
				m_IsDestroyed.push_back(false);
				m_HitBoxes.push_back(Rectf{ m_DrawPosWall[i].x, m_DrawPosWall[i].y, m_SpritesWall[i]->GetFrameWidth(), m_SpritesWall[i]->GetFrameHeight() });
			}
		}
		else if(m_AmountOfBlocks == 3)
		{
			for (int i{ 0 }; i < m_AmountOfBlocks; ++i)
			{
				m_SpritesWall.push_back(new Sprite("Resources/Sprites/Objects/BreakableWall/BreakableWall_03.png", m_AmountOfBlocks, 1, 1, bottomLeft, Sprite::SpriteType::Other, pTextureManager));
				m_DrawPosWall.push_back(Point2f{ bottomLeft.x, bottomLeft.y + m_SpritesWall[i]->GetFrameHeight() * i });
				m_SpritesWall[i]->SetDrawPos(m_DrawPosWall[i]);
				m_IsDestroyed.push_back(false);
				m_HitBoxes.push_back(Rectf{ m_DrawPosWall[i].x, m_DrawPosWall[i].y, m_SpritesWall[i]->GetFrameWidth(), m_SpritesWall[i]->GetFrameHeight() });
			}
		}
		
		break;
	}
}

BreakableWall::~BreakableWall()
{
	for (Sprite* ptr: m_SpritesWall)
	{
		delete ptr;
		ptr = nullptr;
	}
}

void BreakableWall::Draw() const
{
	for (int i{0}; i < m_AmountOfBlocks; ++i)
	{ 
		if (m_IsDestroyed[i] == false)
		{
			m_SpritesWall[i]->Draw();
			//utils::SetColor(Color4f{1.f, 0.f, 0.f, 1.f});
			//utils::FillRect(m_HitBoxes[i]);
		}
	}
}

void BreakableWall::Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle)
{
	// lots of glitches, so wall doesn't do this anymore
	/*for (int i{0}; i < m_AmountOfBlocks; ++i)
	{
		HandleCollision(actorHitBox, actorVelocity, i);
	}*/
}

void BreakableWall::IsHit(const std::vector<Rectf>& playerAttackHitBox, const Rectf& playerHitBox, int playerLevel, DropManager* dropList, TextureManager* pTextureManager, SoundManager* pSoundManager)
{
	for (int i{0}; i < m_AmountOfBlocks; ++i)
	{
		for (size_t j{0}; j < playerAttackHitBox.size(); ++j)
		{
			if (utils::IsOverlapping(playerAttackHitBox[j], m_HitBoxes[i]) == true && m_IsDestroyed[i] == false)
			{
				m_IsDestroyed[i] = true;
				m_pDestroyedSoundEffect->Play(0);
			}
		}
		
	}

	if (CheckAllDestroyed() == true && m_HasDropped == false)
	{
		m_HasDropped = true;
		int index{ m_AmountOfBlocks - 1 };
		dropList->AddItem(m_DrawPosWall[index], m_DropType, 0, playerLevel, pTextureManager, pSoundManager);
	}

}

void BreakableWall::HandleCollision(Rectf& actorHitBox, Vector2f& actorVelocity, int index) const
{
	std::vector < Point2f> hitPoints;
	hitPoints.push_back(Point2f{m_HitBoxes[index].left, m_HitBoxes[index].bottom});
	hitPoints.push_back(Point2f{m_HitBoxes[index].left, m_HitBoxes[index].bottom + m_HitBoxes[index].height});
	hitPoints.push_back(Point2f{m_HitBoxes[index].left + m_HitBoxes[index].width, m_HitBoxes[index].bottom + m_HitBoxes[index].height});
	hitPoints.push_back(Point2f{m_HitBoxes[index].left + m_HitBoxes[index].width, m_HitBoxes[index].bottom});

	// vertical collision
	Point2f p1{ actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom };
	Point2f p2{ actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom + actorHitBox.height };
	utils::HitInfo hitInfo{};

	if (utils::Raycast(hitPoints, p1, p2, hitInfo) == true)
	{
		actorVelocity.y = 0.f;
		actorHitBox.bottom = hitInfo.intersectPoint.y;
	}

	// horizontal
	Point2f p3{ actorHitBox.left + 1.f , actorHitBox.bottom + actorHitBox.height / 2 };
	Point2f p4{ actorHitBox.left + actorHitBox.width - 1.f, actorHitBox.bottom + actorHitBox.height / 2 };
	utils::HitInfo hitInfo2{};

	if (utils::Raycast(hitPoints, p3, p4, hitInfo2) == true)
	{
		if (actorVelocity.x > 0.f) // player is going right
		{
			actorHitBox.left = hitInfo2.intersectPoint.x - actorHitBox.width;

		}
		else // player is going left
		{
			actorHitBox.left = hitInfo2.intersectPoint.x;

		}
		actorVelocity.x = 0.f;

	}
}

bool BreakableWall::CheckAllDestroyed() const
{
	for (int i{0}; i < m_AmountOfBlocks; ++i)
	{
		if (m_IsDestroyed[i] == false)
		{
			return false;
		}
	}
	return true;
}
