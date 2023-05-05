#include "pch.h"
#include "PeepingEye.h"
//#include "utils.h"
#include "Sprite.h"

PeepingEye::PeepingEye(const Point2f& drawPos, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Enemy(pSoundManager)	
	, m_DrawPosEye{drawPos}
	, m_DrawPosChain1{drawPos}
	, m_DrawPosChain2{drawPos}
	, m_DrawPosChain3{drawPos}
	, m_IsDestroyed{false}
	, m_pSpriteEyeSpawning{new Sprite("Resources/Sprites/Enemies/Peeping Eye/PeepingEye_Spawning.png", 5, 1, 8, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteEyeTracker{new Sprite("Resources/Sprites/Enemies/Peeping Eye/PeepingEye_Tracker.png", 7, 1, 1, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteMoving{new Sprite("Resources/Sprites/Enemies/Peeping Eye/PeepingEye_Moving.png", 12, 1, 12, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteChain1{new Sprite("Resources/Sprites/Enemies/Peeping Eye/PeepingEye_Chain.png", 5, 3, 9, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteChain2{new Sprite("Resources/Sprites/Enemies/Peeping Eye/PeepingEye_Chain.png", 5, 3, 9, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteChain3{new Sprite("Resources/Sprites/Enemies/Peeping Eye/PeepingEye_Chain.png", 5, 3, 9, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_CurrentTimer{0.f}
	, m_DestroyingTimer{0.33f}
	, m_Angle{}
	, m_AngleIncrement{0.1f}
	, m_IsLockedOnPlayer{false}
{
	m_Value = 500;		// need value
	m_Damage = 2.f;		// need damage
	m_CanCollideWithStage = false;
	m_CurrentState = ActionState::sleeping;
	m_Health = 5.f;	// need health
	m_MovementSpeed = 50.f;

	m_HitBox.left = m_DrawPosEye.x;
	m_HitBox.bottom = m_DrawPosEye.y;
	m_HitBox.width = m_pSpriteMoving->GetFrameWidth();
	m_HitBox.height = m_pSpriteMoving->GetFrameHeight();

	m_CenterPoint.x = m_HitBox.left + m_HitBox.width / 2.f;
	m_CenterPoint.y = m_HitBox.bottom + m_HitBox.height / 2.f;

	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom + m_HitBox.height });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom + m_HitBox.height });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom });

	m_RandomTimerMax = float(rand() % 10 + 2.f);
}

PeepingEye::~PeepingEye()
{
	delete m_pSpriteEyeSpawning;
	m_pSpriteEyeSpawning = nullptr;

	delete m_pSpriteEyeTracker;
	m_pSpriteEyeTracker = nullptr;

	delete m_pSpriteMoving;
	m_pSpriteMoving = nullptr;

	delete m_pSpriteChain1;
	m_pSpriteChain1 = nullptr;

	delete m_pSpriteChain2;
	m_pSpriteChain2 = nullptr;

	delete m_pSpriteChain3;
	m_pSpriteChain3 = nullptr;

}

void PeepingEye::Update(float elapsedSec, Point2f playerPos, float levelWidth)
{
	switch (m_CurrentState)
	{
	case PeepingEye::ActionState::sleeping:
		if (utils::GetDistance(playerPos, m_CenterPoint) <= 175.f)
		{
			m_CurrentState = ActionState::waking;
		}
		break;
	case PeepingEye::ActionState::waking:
		if (m_pSpriteEyeSpawning->GetCurrentFrame() == m_pSpriteEyeSpawning->GetTotalAmountOfFrames() - 1) // if final frame, make it move
		{
			m_CurrentState = ActionState::moving;
		}
		else
		{
			m_pSpriteEyeSpawning->Update(elapsedSec);
		}
		break;
	case PeepingEye::ActionState::moving:
		//add a timer to randomly get the turning animation
		m_CurrentTimer += elapsedSec;
		if (m_CurrentTimer >= m_RandomTimerMax)
		{
			m_pSpriteMoving->Update(elapsedSec);
			if (m_pSpriteMoving->GetCurrentFrame() == m_pSpriteMoving->GetTotalAmountOfFrames() - 1) // if last frame, reset timer and frames
			{
				m_pSpriteMoving->SetCurrentFrame(0);
				m_CurrentTimer = 0.f;
				m_RandomTimerMax = float(rand() % 10 + 2.f);
			}
		}
		// should update the chain of the eye, currently too confusing to start on, will prioritize everything else first


		// update the position
		if (m_IsLockedOnPlayer == false)
		{
			// move towards player
			if (playerPos.x < m_CenterPoint.x) // we move left
			{
				m_Velocity.x = -m_MovementSpeed; // moveSpeed
			}
			else
			{
				m_Velocity.x = m_MovementSpeed;
			}

			if (playerPos.y < m_CenterPoint.y)
			{
				m_Velocity.y = -m_MovementSpeed;
			}
			else
			{
				m_Velocity.y = m_MovementSpeed;
			}

			m_CenterPoint.x += m_Velocity.x * elapsedSec;
			m_CenterPoint.y += m_Velocity.y * elapsedSec;
			UpdatePos();
			if (utils::GetDistance(m_CenterPoint, playerPos) <= 50.f)
			{
				m_IsLockedOnPlayer = true;
			}
		}
		else // update using pattern
		{
			m_Velocity = Vector2f{ 0.f,0.f };

	

			if (utils::GetDistance(m_CenterPoint, playerPos) >= 75.f)
			{
				m_IsLockedOnPlayer = false;
			}
		}
			m_Angle += m_AngleIncrement;
		
			m_CenterPoint.x += cosf(m_Angle / 2.f) * 1.5f;
			m_CenterPoint.y += sinf(m_Angle / 3.f + 1/20.f * 3.1415f) * 1.5f;
			UpdatePos();

		// track the player
		if (playerPos.y < m_CenterPoint.y - m_HitBox.height / 2.f && playerPos.x >= m_HitBox.left && playerPos.x <= m_HitBox.left + m_HitBox.width)
		{
			m_pSpriteEyeTracker->SetCurrentFrame(1);
		}
		else if(playerPos.y > m_CenterPoint.y + m_HitBox.height / 2.f && playerPos.x >= m_HitBox.left && playerPos.x <= m_HitBox.left + m_HitBox.width)
		{
			m_pSpriteEyeTracker->SetCurrentFrame(2);
		}
		else if (playerPos.y < m_CenterPoint.y && playerPos.x >= m_HitBox.left + m_HitBox.width)
		{
			m_pSpriteEyeTracker->SetCurrentFrame(3);
		}
		else if (playerPos.y > m_CenterPoint.y && playerPos.x >= m_HitBox.left + m_HitBox.width)
		{
			m_pSpriteEyeTracker->SetCurrentFrame(4);
		}
		else if (playerPos.y > m_CenterPoint.y && playerPos.x <= m_HitBox.left)
		{
			m_pSpriteEyeTracker->SetCurrentFrame(5);
		}
		else if (playerPos.y < m_CenterPoint.y && playerPos.x <= m_HitBox.left)
		{
			m_pSpriteEyeTracker->SetCurrentFrame(6);
		}
		else
		{
			m_pSpriteEyeTracker->SetCurrentFrame(0);
		}
		break;
	case PeepingEye::ActionState::dying:
		m_CurrentTimer += elapsedSec;
		if (m_CurrentTimer >= m_DestroyingTimer * 4.f)
		{
			m_IsDestroyed = true;
		}
		break;
	}


	if (m_Health <= 0.f && m_IsDestroyed == false)
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

void PeepingEye::Draw() const
{
	switch (m_CurrentState)
	{
	case PeepingEye::ActionState::sleeping:
		m_pSpriteEyeSpawning->Draw();
		break;
	case PeepingEye::ActionState::waking:
		m_pSpriteEyeSpawning->Draw();
		break;
	case PeepingEye::ActionState::moving:
		m_pSpriteEyeTracker->Draw();
		m_pSpriteMoving->Draw();
		break;
	case PeepingEye::ActionState::dying:
		if (m_CurrentTimer <= m_DestroyingTimer)
		{
			// draw all
		}
		else if (m_CurrentTimer <= m_DestroyingTimer * 2.f)
		{
			// draw 2 chains and eye
		}
		else if (m_CurrentTimer <= m_DestroyingTimer * 3.f)
		{
			// draw 1 chain and eye
		}
		else if (m_CurrentTimer <= m_DestroyingTimer * 4.f)
		{
			// draw eye
		}
		// "destroying" the chain from bottom to top, when all destroyed, get rid of eye, then set destroyed
		break;
	}
}


void PeepingEye::UpdatePos()
{
	m_DrawPosEye.x = m_CenterPoint.x - m_HitBox.width / 2.f;
	m_DrawPosEye.y = m_CenterPoint.y - m_HitBox.height / 2.f;
	
	m_pSpriteMoving->SetDrawPos(m_DrawPosEye);
	m_pSpriteEyeTracker->SetDrawPos(m_DrawPosEye);

	m_HitBox.left = m_DrawPosEye.x;
	m_HitBox.bottom = m_DrawPosEye.y;

	

	m_HitBoxVertices[0] = Point2f{ m_HitBox.left, m_HitBox.bottom };
	m_HitBoxVertices[1] = Point2f{ m_HitBox.left, m_HitBox.bottom + m_HitBox.height };
	m_HitBoxVertices[2] = Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom + m_HitBox.height };
	m_HitBoxVertices[3] = Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom };
}
