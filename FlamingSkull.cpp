#include "pch.h"
#include "FlamingSkull.h"
#include "Sprite.h"

FlamingSkull::FlamingSkull(const Point2f& drawPos, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Enemy(pSoundManager)
	, m_pSpriteSpawning{ new Sprite("Resources/Sprites/Enemies/Flaming Skull/FlamingSkull_Spawning.png", 5, 1, 8, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteMoving{ new Sprite("Resources/Sprites/Enemies/Flaming Skull/FlamingSkull_Moving.png", 3, 1, 5, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_Angle{0.f}
	, m_AngleIncrement{0.05f}
{
	m_DrawPos = drawPos;
	m_Value = 500;		// need value
	m_Damage = 2.f;		// need damage
	m_CanCollideWithStage = false;
	m_CurrentState = ActionState::sleeping;
	m_Health = 5.f;	// need health
	m_MovementSpeed = 50.f;


	m_HitBox.left = m_DrawPos.x;
	m_HitBox.bottom = m_DrawPos.y;
	m_HitBox.width = m_pSpriteMoving->GetFrameWidth();
	m_HitBox.height = m_pSpriteMoving->GetFrameHeight();

	m_CenterPoint.x = m_HitBox.left + m_HitBox.width / 2.f;
	m_CenterPoint.y = m_HitBox.bottom + m_HitBox.height / 2.f;

	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom + m_HitBox.height });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom + m_HitBox.height });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom });
}

FlamingSkull::~FlamingSkull()
{
	delete m_pSpriteSpawning;
	m_pSpriteSpawning = nullptr;

	delete m_pSpriteMoving;
	m_pSpriteMoving = nullptr;
}

void FlamingSkull::Update(float elapsedSec, Point2f playerPos, float levelWidth)
{
	switch (m_CurrentState)
	{
	case Enemy::ActionState::sleeping:
		if (utils::GetDistance(playerPos, m_CenterPoint) <= 125.f)
		{
			m_CurrentState = ActionState::waking;
		}
		break;
	case Enemy::ActionState::waking:
		if (m_pSpriteSpawning->GetCurrentFrame() == m_pSpriteSpawning->GetTotalAmountOfFrames() - 1) // if final frame, make it move
		{
			m_CurrentState = ActionState::moving;
		}
		else
		{
			m_pSpriteSpawning->Update(elapsedSec);
		}
		break;
	case Enemy::ActionState::moving:
		if (playerPos.x <= m_CenterPoint.x)
		{
			m_IsLookingRight = false;
		}
		else
		{
			m_IsLookingRight = true;
		}

		m_Angle += m_AngleIncrement;
		m_CenterPoint.x += sinf(m_Angle / 2.f) * 2.f;
		m_CenterPoint.y += cosf(m_Angle) * 2.f;
		UpdatePos();
		m_pSpriteMoving->Update(elapsedSec);
		break;
	case Enemy::ActionState::dying:
		break;
	}	

	if (m_Health <= 0.f)
	{
		m_CurrentState = ActionState::dying;

	}

	if (m_IsInvincible == true)
	{
		m_CurrentInvinsibleTimer += elapsedSec;
		if (m_CurrentInvinsibleTimer >= m_MaxInvinsibleTimer)
		{
			m_CurrentInvinsibleTimer = 0.f;
			m_IsInvincible = false;
		}
	}
}

void FlamingSkull::Draw() const
{
	switch (m_CurrentState)
	{
	case Enemy::ActionState::waking:
		m_pSpriteSpawning->Draw();
		break;
	case Enemy::ActionState::moving:
		//if (m_IsLookingRight == false)
		//{
		//	m_pSpriteMoving->Draw();
		//}
		//else
		//{
		//	glPushMatrix();
		//	{
		//		glTranslatef(m_DrawPos.x + m_pSpriteMoving->GetFrameWidth() / 2, m_DrawPos.y, 0.f);
		//		glScalef(-1.f, 1.f, 1.f);
		//		glTranslatef(-m_DrawPos.x - m_pSpriteMoving->GetFrameWidth() / 2, -m_DrawPos.y, 0.f);
		//		m_pSpriteMoving->Draw();
		//	}
		//	glPopMatrix();
		//}
		DrawSprite(m_pSpriteMoving);
		break;
	}
}

void FlamingSkull::UpdatePos()
{
	m_DrawPos.x = m_CenterPoint.x - m_HitBox.width / 2.f;
	m_DrawPos.y = m_CenterPoint.y - m_HitBox.height / 2.f;

	m_pSpriteMoving->SetDrawPos(m_DrawPos);

	m_HitBox.left = m_DrawPos.x;
	m_HitBox.bottom = m_DrawPos.y;



	m_HitBoxVertices[0] = Point2f{ m_HitBox.left, m_HitBox.bottom };
	m_HitBoxVertices[1] = Point2f{ m_HitBox.left, m_HitBox.bottom + m_HitBox.height };
	m_HitBoxVertices[2] = Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom + m_HitBox.height };
	m_HitBoxVertices[3] = Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom };
}
