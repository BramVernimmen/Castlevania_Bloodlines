#include "pch.h"
#include "Enemy.h"
#include "Sprite.h"

#include "SoundManager.h"
#include "SoundEffect.h"

Enemy::Enemy(SoundManager* pSoundManager)
	: m_Damage{0.f}
	, m_CanCollideWithStage{false}
	, m_PointOfContact{0.f}
	, m_Value{ 0 }
	, m_Velocity{ Vector2f{0.f, 0.f} }
	, m_HitBox{ Rectf{0.f, 0.f, 0.f, 0.f} }
	, m_CurrentState{ActionState::none}
	, m_IsInvincible{false}
	, m_CurrentInvinsibleTimer{ 0.f}
	, m_MaxInvinsibleTimer{0.7f}
	, m_Health{0.f}
	, m_HasGivenPoints{false}
	, m_CenterPoint{ Point2f{0.f, 0.f} }
	, m_IsLookingRight{false}
	, m_MovementSpeed{0.f}
	, m_DrawPos{ Point2f{0.f, 0.f} }
	, m_IsOnGround{false}
{
	m_pHitSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/EnemyHit.mp3");
}

void Enemy::IsHit(std::vector<Rectf> playerAttackHitBox, float playerDamage)
{
	if (m_CurrentState == ActionState::moving || m_CurrentState == ActionState::attacking || m_CurrentState == ActionState::idle)
	{
		for (size_t attackHitBoxIndex{0}; attackHitBoxIndex < playerAttackHitBox.size(); ++attackHitBoxIndex)
		{
			if (utils::IsOverlapping(m_HitBox, playerAttackHitBox[attackHitBoxIndex]) == true && m_IsInvincible == false)
			{
				m_Health -= playerDamage;
				m_IsInvincible = true;
				m_pHitSoundEffect->Play(0);
			}
		}
		
	}
	
}

bool Enemy::IsDead()
{
	if (m_CurrentState == ActionState::dying && m_HasGivenPoints == false)
	{
		m_HasGivenPoints = true;
		return true;
	}
	return false;
}

bool Enemy::HitPlayer(Rectf playerHitBox)
{
	if (m_CurrentState == ActionState::moving || m_CurrentState == ActionState::attacking)
	{
		for (size_t hitBoxVerticesIndex{ 0 }; hitBoxVerticesIndex < m_HitBoxVertices.size(); ++hitBoxVerticesIndex)
		{
			if (utils::IsPointInRect(m_HitBoxVertices[hitBoxVerticesIndex], playerHitBox))
			{
				m_PointOfContact = m_HitBoxVertices[hitBoxVerticesIndex].x;
				return true;
			}
		}
	}
	return false;
}

Rectf& Enemy::GetHitBox()
{
	return m_HitBox;
}

Vector2f& Enemy::GetVelocity()
{
	return m_Velocity;
}

int Enemy::GetValue()
{
	return m_Value;
}

float Enemy::GetPointOfContact()
{
	return m_PointOfContact;
}

float Enemy::GetDamage()
{
	return m_Damage;
}

bool Enemy::GetCollideWithStage()
{
	return m_CanCollideWithStage;
}

void Enemy::SetOnGround(bool isOnGround)
{
	m_IsOnGround = isOnGround;
}

void Enemy::DrawSprite(Sprite* pSpriteToDraw) const
{
	if (m_IsLookingRight == true)
	{
		glPushMatrix();
		{
			glTranslatef(m_DrawPos.x + pSpriteToDraw->GetFrameWidth() / 2, m_DrawPos.y, 0.f);
			glScalef(-1.f, 1.f, 1.f);
			glTranslatef(-m_DrawPos.x - pSpriteToDraw->GetFrameWidth() / 2, -m_DrawPos.y, 0.f);
			pSpriteToDraw->Draw();
		}
		glPopMatrix();

	}
	else
	{
		pSpriteToDraw->Draw();
	}
}
