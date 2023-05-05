#include "pch.h"
#include "FishMan.h"
#include "Sprite.h"

FishMan::FishMan(bool repeatSpawning, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Enemy(pSoundManager)
	, m_RepeatSpawn{ repeatSpawning }
	, m_CanSpawn{ false }
	, m_HasSpawned{ false }
	, m_RespawnTimer{ 0.f }
	, m_MaxRespawnTimer{ 1.75f }
	, m_HasAttacked{false}
	, m_JumpSpeed{800.f}
	, m_Acceleration{0.f, -981.f}
	, m_IsWalking{false}
	, m_WalkWaitTimer{0.25f}
	, m_CurrentTimer{0.f}
	, m_WalkMaxTimer{0.25f}
	, m_AttackMaxTimer{1.5f}
	, m_pSpriteMoving{new Sprite("Resources/Sprites/Enemies/Fish Man/FishMan_Moving.png", 2, 1, 8, m_DrawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteSpawning{new Sprite("Resources/Sprites/Enemies/Fish Man/FishMan_Spawning.png", 3, 1, 8, m_DrawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteDying{new Sprite("Resources/Sprites/Enemies/Fish Man/FishMan_Dying.png", 3, 1, 8, m_DrawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteAttacking{new Sprite("Resources/Sprites/Enemies/Fish Man/FishMan_Attacking.png", 2, 1, 12, m_DrawPos, Sprite::SpriteType::Other, pTextureManager)}
{
	m_Value = 300;
	m_Damage = 2.f;
	m_CanCollideWithStage = true;
	m_MovementSpeed = 75.f;
	m_CurrentState = ActionState::waking;
	m_Health = 1.5f;
	m_OriginalHealth = m_Health;
	m_DrawPos = Point2f{ -50.f, -500.f }; // out of bounds
	


	m_HitBox.left = m_DrawPos.x;
	m_HitBox.bottom = m_DrawPos.y;
	m_HitBox.width = m_pSpriteMoving->GetFrameWidth();
	m_HitBox.height = m_pSpriteMoving->GetFrameHeight();


	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom + m_HitBox.height });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom + m_HitBox.height });
	// need to push back the projectile
	UpdatePos();
}

FishMan::~FishMan()
{
	delete m_pSpriteMoving;
	m_pSpriteMoving = nullptr;

	delete m_pSpriteSpawning;
	m_pSpriteSpawning = nullptr;	

	delete m_pSpriteDying;
	m_pSpriteDying = nullptr;

	delete m_pSpriteAttacking;
	m_pSpriteAttacking = nullptr;
}


void FishMan::Update(float elapsedSec, Point2f playerPos, float levelWidth)
{
	switch (m_CurrentState)
	{
	case Enemy::ActionState::waking:
		if (m_HasSpawned == true)
		{
			m_Velocity.y += m_Acceleration.y * elapsedSec;

			//m_DrawPos.x += m_Velocity.x * elapsedSec;
			//m_DrawPos.y += m_Velocity.y * elapsedSec;	
			
			m_HitBox.left += m_Velocity.x * elapsedSec;
			m_HitBox.bottom += m_Velocity.y * elapsedSec;

			UpdatePos();

			if (m_Velocity.y > 80.f)
			{
				m_pSpriteSpawning->SetCurrentFrame(0);
			}
			else if (m_Velocity.y < 80.f && m_Velocity.y > -80.f)
			{
				m_pSpriteSpawning->SetCurrentFrame(1);
			}
			else if (m_Velocity.y < -80.f)
			{
				m_pSpriteSpawning->SetCurrentFrame(2);
			}
		}
		else
		{
			m_HitBox.left	 = -1000.f; // out of bounds
			m_HitBox.bottom  = -1000.f; // out of bounds
		}
		

		

		if (playerPos.x > 105.f)
		{
			m_CanSpawn = true;
		}
		else
		{
			m_CanSpawn = false;

		}


		// only collide with stage when hasn't attacked and is falling down
		if (m_Velocity.y <= -80.f && m_HasAttacked == false)
		{
			m_CanCollideWithStage = true;
		}
		else
		{
			m_CanCollideWithStage = false;
		}

		if (m_IsOnGround == true && m_HasAttacked == false && m_HitBox.bottom > 0.f) // set on ground
		{
			m_CurrentState = ActionState::moving;
			
		}

		if (m_IsOnGround == true && m_HitBox.bottom <= 0.f && m_Velocity.y <= 0.f)
		{
			m_CanCollideWithStage = false;
			m_Velocity.y = -m_JumpSpeed;
		}

		if (m_Velocity.y < 0.f && m_HitBox.bottom < -m_pSpriteSpawning->GetFrameHeight() && m_HasSpawned == true)
		{
			m_HasGivenPoints = true;
			m_Velocity.y = 0.f;
			m_CurrentState = ActionState::dying;
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

		m_CanCollideWithStage = true;
		UpdatePos();

		if (m_IsWalking == false && m_CurrentTimer <= m_WalkWaitTimer)
		{
			m_CurrentTimer += elapsedSec;
		}
		else if (m_IsWalking == false && m_CurrentTimer > m_WalkWaitTimer)
		{
			m_IsWalking = true;
			m_CurrentTimer = 0.f;
		}

		if (m_IsWalking == true && m_CurrentTimer <= m_WalkMaxTimer)
		{
			m_CurrentTimer += elapsedSec;
			m_pSpriteMoving->Update(elapsedSec);
			if (m_IsOnGround == true)
			{
				if (m_IsLookingRight == false)
				{
					m_HitBox.left += -m_MovementSpeed * elapsedSec;
				}
				else
				{
					m_HitBox.left += m_MovementSpeed * elapsedSec;
				}
			}
			
			// update the pos of the walk
		}
		else if (m_IsWalking == true && m_CurrentTimer > m_WalkMaxTimer)
		{
			m_IsWalking = false;
			m_CurrentTimer = 0.f;
			m_CurrentState = ActionState::attacking;
		}

		break;
	case Enemy::ActionState::attacking:
		if (m_pSpriteAttacking->GetCurrentFrame() == m_pSpriteAttacking->GetTotalAmountOfFrames() - 1)
		{
			m_pSpriteAttacking->SetCurrentFrame(m_pSpriteAttacking->GetTotalAmountOfFrames() - 1);
			m_CurrentTimer += elapsedSec;
		}
		else
		{
			m_pSpriteAttacking->Update(elapsedSec);
		}

		if (m_CurrentTimer >= m_AttackMaxTimer)
		{
			m_HasAttacked = true;
			m_CurrentTimer = 0.f;
			m_Velocity.y += m_JumpSpeed;
			m_CurrentState = ActionState::waking;
		}
		
		break;
	case Enemy::ActionState::dying:
		m_HitBoxVertices[0] = Point2f{ -1000.f, 1000.f };
		m_HitBoxVertices[1] = Point2f{ -1000.f, 1000.f };
		m_HitBoxVertices[2] = Point2f{ -1000.f, 1000.f };
		m_HitBoxVertices[3] = Point2f{ -1000.f, 1000.f };
		if (m_pSpriteDying->GetCurrentFrame() == m_pSpriteDying->GetTotalAmountOfFrames() - 1)
		{
			//m_pSpriteDying->SetCurrentFrame(m_pSpriteDying->GetTotalAmountOfFrames() - 1);
			m_RespawnTimer += elapsedSec;
			if (m_RespawnTimer >= m_MaxRespawnTimer)
			{
				m_CurrentState = ActionState::waking;
				m_RespawnTimer = 0.f;
				m_HasSpawned = false;
				m_HasGivenPoints = false;
				m_HasAttacked = false;
				m_CanCollideWithStage = false;
				m_IsOnGround = false;
				m_Health = m_OriginalHealth;
				m_CurrentTimer = 0.f;
				m_Velocity.y = 0.f;
				m_pSpriteAttacking->SetCurrentFrame(0);
				m_HitBox.left = -1000.f; // out of bounds
				m_HitBox.bottom = -1000.f; // out of bounds
				UpdatePos();
				m_pSpriteDying->SetCurrentFrame(0);
			}
		}
		else
		{
			m_pSpriteDying->Update(elapsedSec);
		}
		break;
	}

	
	if (m_CanSpawn == true && m_HasSpawned == false)
	{

		// random next to 80 px left and right of the player with a distance of at least 20

		if (rand() % 2 == 0) // random spawn left or right of the player
		{
			m_HitBox.left = playerPos.x + rand() % 141 + 20;
		}
		else
		{
			m_HitBox.left = playerPos.x - rand() % 141 + 20;
		}

		UpdatePos();

		//if (playerPos.x <= m_DrawPos.x)
		if (playerPos.x <= m_CenterPoint.x)
		{
			m_IsLookingRight = false;
		}
		else
		{
			m_IsLookingRight = true;
		}

		m_Velocity.y += m_JumpSpeed;
		m_HitBox.bottom = -m_pSpriteSpawning->GetFrameHeight();
		m_HasSpawned = true;

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

void FishMan::Draw() const
{
	switch (m_CurrentState)
	{
	case Enemy::ActionState::waking:
		if (m_HasSpawned == true)
		{
			DrawSprite(m_pSpriteSpawning);
		}
		break;
	case Enemy::ActionState::moving:
		if (m_HasSpawned == true)
		{
			DrawSprite(m_pSpriteMoving);
		}
		break;
	case Enemy::ActionState::attacking:
		DrawSprite(m_pSpriteAttacking);
		break;
	case Enemy::ActionState::dying:
		if (m_pSpriteDying->GetCurrentFrame() != m_pSpriteDying->GetTotalAmountOfFrames()-1)
		{
			DrawSprite(m_pSpriteDying);
		}
		
		break;
	}
}

void FishMan::UpdatePos()
{
	m_pSpriteMoving->SetDrawPos(m_DrawPos);
	m_pSpriteSpawning->SetDrawPos(m_DrawPos);
	m_pSpriteDying->SetDrawPos(m_DrawPos);
	m_pSpriteAttacking->SetDrawPos(m_DrawPos);

	//m_HitBox.left = m_DrawPos.x;
	//m_HitBox.bottom = m_DrawPos.y;

	m_DrawPos.x = m_HitBox.left;
	m_DrawPos.y = m_HitBox.bottom;

	m_CenterPoint.x = m_DrawPos.x + m_pSpriteMoving->GetFrameWidth() / 2.f;
	m_CenterPoint.y = m_DrawPos.y + m_pSpriteMoving->GetFrameHeight() / 2.f;

	m_HitBoxVertices[0] = Point2f{ m_HitBox.left, m_HitBox.bottom };
	m_HitBoxVertices[1] = Point2f{ m_HitBox.left, m_HitBox.bottom + m_HitBox.height };
	m_HitBoxVertices[2] = Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom + m_HitBox.height };
	m_HitBoxVertices[3] = Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom };
}
