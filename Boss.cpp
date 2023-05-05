#include "pch.h"
#include "Boss.h"
#include "Sprite.h"
#include "SoundStream.h"

#include "SoundManager.h"
#include "SoundEffect.h"
Boss::Boss(SoundManager* pSoundManager, const std::string& streamPath)
	: m_Damage{ 0.f }
	, m_PointOfContact{ 0.f }
	, m_Value{ 0 }
	, m_Velocity{ Vector2f{0.f, 0.f} }
	, m_HitBox{ Rectf{0.f, 0.f, 0.f, 0.f} }
	, m_CurrentState{ ActionState::none }
	, m_IsInvincible{ false }
	, m_CurrentInvinsibleTimer{ 0.f }
	, m_MaxInvinsibleTimer{ 0.7f }
	, m_Health{ 0.f }
	, m_MaximumHealth{0.f}
	, m_HasGivenPoints{ false }
	, m_CenterPoint{ Point2f{0.f, 0.f} }
	, m_IsLookingRight{ false }
	, m_MovementSpeed{ 0.f }
	, m_DrawPos{ Point2f{0.f, 0.f} }
	, m_VerticalTrigger{ 0.f }
{
	m_pBossSoundStream = pSoundManager->GetSoundStreamPointer(streamPath);
	m_pHitSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/EnemyHit.mp3");
}

void Boss::IsHit(std::vector<Rectf> playerAttackHitBox, float playerDamage)
{
	if (m_CurrentState == ActionState::moving || m_CurrentState == ActionState::attacking || m_CurrentState == ActionState::jumping || m_CurrentState == ActionState::idle)
	{
		for (size_t attackHitBoxIndex{ 0 }; attackHitBoxIndex < playerAttackHitBox.size(); ++attackHitBoxIndex)
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

bool Boss::IsDead()
{
	if (m_CurrentState == ActionState::dying && m_HasGivenPoints == false)
	{
		m_HasGivenPoints = true;
		return true;
	}
	return false;
}

bool Boss::HitPlayer(Rectf playerHitBox)
{
	if (m_CurrentState == ActionState::moving || m_CurrentState == ActionState::attacking || m_CurrentState == ActionState::jumping || m_CurrentState == ActionState::idle)
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

Rectf& Boss::GetHitBox()
{
	return m_HitBox;
}

Vector2f& Boss::GetVelocity()
{
	return m_Velocity;
}

int Boss::GetValue()
{
	return m_Value;
}

float Boss::GetPointOfContact()
{
	return m_PointOfContact;
}

float Boss::GetDamage()
{
	return m_Damage;
}

float Boss::GetCurrentHealth()
{
	return m_Health;
}

float Boss::GetMaximumHealth()
{
	return m_MaximumHealth;
}

Boss::ActionState Boss::GetCurrentState()
{
	return m_CurrentState;
}

bool Boss::HasGivenPoints()
{
	return m_HasGivenPoints;
}


void Boss::PlayBossMusic()
{
	if (m_pBossSoundStream->IsPlaying() == false)
	{
		m_pBossSoundStream->Play(true);
	}
}

void Boss::StopBossMusic()
{
	m_pBossSoundStream->Stop();
}

void Boss::DrawSprite(Sprite* pSpriteToDraw) const
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