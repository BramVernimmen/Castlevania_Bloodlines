#include "pch.h"
#include "VampireBat.h"
#include "Sprite.h"
//#include "utils.h"

#include <iostream>

VampireBat::VampireBat(bool canRepeat, Point2f drawPos, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Enemy(pSoundManager)
	, m_pSpriteMoving{ new Sprite("Resources/Sprites/Enemies/Vampire Bat/VampireBat_Moving.png", 6, 1, 10, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteDying{ new Sprite("Resources/Sprites/Enemies/Vampire Bat/VampireBat_Dying.png", 2, 1, 3, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteIdle{ new Sprite("Resources/Sprites/Enemies/Vampire Bat/VampireBat_Idle.png", 1, 1, 1, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_RespawnTimer{0.f}
	, m_MaxRespawnTimer{0.5f}
	, m_Angle{0.f}
	, m_AngleIncrement{0.05f}
{
	m_Value = 150;	// need value
	m_Damage = 2.f; // need damage
	m_CanCollideWithStage = false;
	m_Health = 1.f;
	m_OriginalHealth = 1.f;
	m_MovementSpeed = 150.f;

	if (canRepeat == true)
	{
		m_CurrentState = ActionState::moving;
		m_DrawPos = Point2f{-100.f, -100.f};
		m_CenterPoint = m_DrawPos;
		m_RepeatSpawn = true;
		m_CanSpawn = false;
		m_HasSpawned = false;
		m_HitBox = Rectf{};
		m_HitBoxVertices.push_back(m_DrawPos);
		m_HitBoxVertices.push_back(Point2f{ m_DrawPos.x, m_DrawPos.y + m_HitBox.height });
		m_HitBoxVertices.push_back(Point2f{ m_DrawPos.x + m_HitBox.width, m_DrawPos.y + m_HitBox.height });
		m_HitBoxVertices.push_back(Point2f{ m_DrawPos.x + m_HitBox.width, m_DrawPos.y });
	}
	else
	{
		m_CurrentState = ActionState::idle;
		m_DrawPos = drawPos;
		m_CenterPoint.x = m_DrawPos.x - m_pSpriteIdle->GetFrameWidth() / 2.f;
		m_CenterPoint.y = m_DrawPos.y - m_pSpriteIdle->GetFrameHeight() / 2.f;
		m_HitBox.bottom = m_DrawPos.y;
		m_HitBox.left = m_DrawPos.x;
		m_HitBox.width = m_pSpriteIdle->GetFrameWidth();
		m_HitBox.height = m_pSpriteIdle->GetFrameHeight();

		m_HitBoxVertices.push_back( m_DrawPos );
		m_HitBoxVertices.push_back(Point2f{ m_DrawPos.x, m_DrawPos.y + m_HitBox.height });
		m_HitBoxVertices.push_back(Point2f{ m_DrawPos.x + m_HitBox.width, m_DrawPos.y + m_HitBox.height });
		m_HitBoxVertices.push_back(Point2f{ m_DrawPos.x + m_HitBox.width, m_DrawPos.y});
		
		m_RepeatSpawn = false;
		m_CanSpawn = true;
		m_HasSpawned = true;
	}
}

VampireBat::~VampireBat()
{
	delete m_pSpriteMoving;
	m_pSpriteMoving	= nullptr;

	delete m_pSpriteDying;
	m_pSpriteDying	= nullptr;

	delete m_pSpriteIdle;
	m_pSpriteIdle	= nullptr;
}

void VampireBat::Update(float elapsedSec, Point2f playerPos, float levelWidth)
{
	switch (m_CurrentState)
	{
	case VampireBat::ActionState::idle:
		if (utils::GetDistance(playerPos, m_CenterPoint) <= 100.f)
		{
			m_CurrentState = ActionState::moving;
		}
		// when player is too close, change to moving, check direction
		if (playerPos.x <= m_CenterPoint.x)
		{
			m_Velocity.x = -m_MovementSpeed;
		}
		else
		{
			m_Velocity.x = m_MovementSpeed;
		}
		break;
	case VampireBat::ActionState::moving:
		if (playerPos.x > 160.f && playerPos.x < levelWidth - 560.f) // only spawn when player is more than 160 px from the start and less then 160 from the end
		{
			m_CanSpawn = true;
		}
		else
		{
			m_CanSpawn = false;

		}


		if (m_RepeatSpawn == true)
		{
			// when repeat, always go to the right
			m_Velocity.x = -m_MovementSpeed;
		}

		m_Angle += m_AngleIncrement;
		m_DrawPos.y += sinf(m_Angle);

		m_DrawPos.x += m_Velocity.x * elapsedSec;
		UpdatePos();
		m_pSpriteMoving->Update(elapsedSec);
		break;
	case VampireBat::ActionState::dying:

		m_DrawPos.y -= m_MovementSpeed * elapsedSec;
		UpdatePos();
		m_HitBoxVertices[0] = Point2f{ -1000.f, 1000.f };
		m_HitBoxVertices[1] = Point2f{ -1000.f, 1000.f };
		m_HitBoxVertices[2] = Point2f{ -1000.f, 1000.f };
		m_HitBoxVertices[3] = Point2f{ -1000.f, 1000.f };
		if (m_pSpriteDying->GetCurrentFrame() == m_pSpriteDying->GetTotalAmountOfFrames() - 1)
		{
			m_pSpriteDying->SetCurrentFrame(m_pSpriteDying->GetTotalAmountOfFrames() - 1);
			m_RespawnTimer += elapsedSec;
			if (m_RespawnTimer >= m_MaxRespawnTimer)
			{
				m_CurrentState = ActionState::moving;
				m_RespawnTimer = 0.f;
				m_HasSpawned = false;
				m_HasGivenPoints = false;
				m_Health = m_OriginalHealth;

				m_pSpriteDying->SetCurrentFrame(0);
			}
		}
		else
		{
			m_pSpriteDying->Update(elapsedSec);
		}
		break;
	}



	if (m_CanSpawn == true && m_HasSpawned == false && m_RepeatSpawn == true)
	{
		// we always spawn the bat on the right side of the player on repeat
		// set distance for a spawn
		m_DrawPos.x = playerPos.x + 280.f;
		//m_DrawPos.x = playerPos.x + float(rand() % 251 + 160);

		
		

		// use player y
		m_DrawPos.y = playerPos.y; // currently hardcoded, only spawns in 1-2, on this y level...
		UpdatePos();
		m_HasSpawned = true;



	}


	if (m_HasSpawned == true && utils::GetDistance(playerPos, m_CenterPoint) > 400.f && m_RepeatSpawn == true)
	{
		// when bat is outside of view, we respawn it
		// if repeat spawn is off, we keep it spawned, so it wont spawn again
		m_HasSpawned = false;
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

void VampireBat::Draw() const
{
	switch (m_CurrentState)
	{
	case VampireBat::ActionState::idle:
		m_pSpriteIdle->Draw();
		break;
	case VampireBat::ActionState::moving:
		/*if (m_IsLookingRight == false)
		{
			m_pSpriteMoving->Draw();

		}
		else
		{
			glPushMatrix();
			{
				glTranslatef(m_DrawPos.x + m_pSpriteMoving->GetFrameWidth() / 2, m_DrawPos.y, 0.f);
				glScalef(-1.f, 1.f, 1.f);
				glTranslatef(-m_DrawPos.x - m_pSpriteMoving->GetFrameWidth() / 2, -m_DrawPos.y, 0.f);
				m_pSpriteMoving->Draw();
			}
			glPopMatrix();
			
		}*/
		DrawSprite(m_pSpriteMoving);
		break;
	case VampireBat::ActionState::dying:
		/*if (m_IsLookingRight == false)
		{
			m_pSpriteDying->Draw();

		}
		else
		{
			glPushMatrix();
			{
				glTranslatef(m_DrawPos.x + m_pSpriteDying->GetFrameWidth() / 2, m_DrawPos.y, 0.f);
				glScalef(-1.f, 1.f, 1.f);
				glTranslatef(-m_DrawPos.x - m_pSpriteDying->GetFrameWidth() / 2, -m_DrawPos.y, 0.f);
				m_pSpriteDying->Draw();
			}
			glPopMatrix();

		}*/
		DrawSprite(m_pSpriteDying);
		break;
	}
}

void VampireBat::UpdatePos()
{
	m_CenterPoint.x = m_DrawPos.x + m_pSpriteMoving->GetFrameWidth() / 2.f;
	m_CenterPoint.y = m_DrawPos.y + m_pSpriteMoving->GetFrameHeight() / 2.f;

	m_HitBox.bottom = m_DrawPos.y;
	m_HitBox.left = m_DrawPos.x;
	m_HitBox.width = m_pSpriteMoving->GetFrameWidth();
	m_HitBox.height = m_pSpriteMoving->GetFrameHeight();

	m_HitBoxVertices[0] = (m_DrawPos);
	m_HitBoxVertices[1] = (Point2f{ m_DrawPos.x, m_DrawPos.y + m_HitBox.height });
	m_HitBoxVertices[2] = (Point2f{ m_DrawPos.x + m_HitBox.width, m_DrawPos.y + m_HitBox.height });
	m_HitBoxVertices[3] = (Point2f{ m_DrawPos.x + m_HitBox.width, m_DrawPos.y });

	m_pSpriteMoving->SetDrawPos(m_DrawPos);
	m_pSpriteDying->SetDrawPos(m_DrawPos);
}
