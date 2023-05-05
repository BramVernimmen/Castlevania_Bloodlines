#include "pch.h"
#include "Zombie.h"
#include "Sprite.h"
//#include "utils.h"

Zombie::Zombie(bool repeatSpawning, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Enemy(pSoundManager)
	, m_RepeatSpawn{repeatSpawning}
	, m_CanSpawn{false}
	, m_HasSpawned{false}
	, m_RespawnTimer{0.f}
	, m_MaxRespawnTimer{1.5f}
	, m_pSpriteMoving{new Sprite("Resources/Sprites/Enemies/Zombie/Zombie_Moving.png", 4, 1, 8, m_DrawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteDying{new Sprite("Resources/Sprites/Enemies/Zombie/Zombie_Dying.png", 4, 1, 3, m_DrawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_pSpriteBloodParticles{new Sprite("Resources/Sprites/Enemies/Zombie/Zombie_BloodParticles.png", 3, 1, 5, m_DrawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_FlyPos{m_DrawPos}
	, m_FlyAngle{0.f}
	, m_pSpriteFly{new Sprite("Resources/Sprites/Enemies/Zombie/Zombie_Fly.png", 2, 1, 8, m_FlyPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_MovementScale{50.f}
{
	m_Value = 100;
	m_Damage = 2.f;
	m_CanCollideWithStage = true;
	m_CurrentState = ActionState::moving;
	m_Health = 1.f;
	m_OriginalHealth = m_Health;
	m_DrawPos = Point2f{ -50.f, 500.f }; // out of bounds


	// in need of a CamWidth var, this will be used for the spawning and stuff
	m_HitBox.left = m_DrawPos.x;
	m_HitBox.bottom = m_DrawPos.y;
	m_HitBox.width = m_pSpriteMoving->GetFrameWidth();
	m_HitBox.height = m_pSpriteMoving->GetFrameHeight();
	m_pSpriteMoving->SetCurrentFrame(rand() % 4);
	m_MovementSpeed = float(rand() % 10 + 10) / 10.f;
	m_pSpriteMoving->SetFrameRate(int(m_MovementSpeed * 6.f));
	m_Velocity.x = m_MovementSpeed * m_MovementScale;
	m_pSpriteDying->SetCurrentFrame(1);

	// random chance for a fly spawn...
	if (rand() % 5 == 0)
	{
		m_HasFly = true;
		m_Value = 150;
	}
	else
	{
		m_HasFly = false;
		m_Value = 100;
	
	}

	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom + m_HitBox.height });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom + m_HitBox.height });


}

Zombie::~Zombie()
{
	delete m_pSpriteMoving;
	m_pSpriteMoving = nullptr;

	delete m_pSpriteDying;
	m_pSpriteDying = nullptr;

	delete m_pSpriteBloodParticles;
	m_pSpriteBloodParticles = nullptr;

	delete m_pSpriteFly;
	m_pSpriteFly = nullptr;
}

void Zombie::Update(float elapsedSec, Point2f playerPos, float levelWidth)
{
	

	switch (m_CurrentState)
	{
	case Zombie::ActionState::moving:

		if (playerPos.x > 160.f && playerPos.x < levelWidth - 160.f) // only spawn when player is more than 160 px from the start and less then 160 from the end
		{
			m_CanSpawn = true;
		}
		else
		{
			m_CanSpawn = false;
			
		}
		m_pSpriteMoving->Update(elapsedSec);
		if (m_HasSpawned == true)
		{
			if (m_IsLookingRight == true)
			{
				m_DrawPos.x += m_Velocity.x * elapsedSec;
				//m_DrawPos.x += m_MovementSpeed;
			}
			else
			{
				m_DrawPos.x -= m_Velocity.x * elapsedSec;
				//m_DrawPos.x -= m_MovementSpeed;
			}
		}
		else
		{
			m_DrawPos.x = -1000.f;
			m_DrawPos.y = -1000.f;
		}
		
		if (m_HasFly == true)
		{
			m_FlyAngle += 0.15f;
			m_FlyPos = m_CenterPoint;
			m_FlyPos.x += cosf(m_FlyAngle) * 20.f;
			m_FlyPos.y += sinf(m_FlyAngle / 3.f) * 20.f;
			m_pSpriteFly->SetDrawPos(m_FlyPos);
			m_pSpriteFly->Update(elapsedSec);
		}
		m_HitBox.left = m_DrawPos.x;
		m_CenterPoint.x = m_HitBox.left + m_HitBox.width / 2.f;
		m_CenterPoint.y = m_HitBox.bottom + m_HitBox.height / 2.f;

		m_HitBoxVertices[0] = Point2f{ m_HitBox.left, m_HitBox.bottom };
		m_HitBoxVertices[1] = Point2f{ m_HitBox.left, m_HitBox.bottom + m_HitBox.height };
		m_HitBoxVertices[2] = Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom };
		m_HitBoxVertices[3] = Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom + m_HitBox.height };



		m_pSpriteMoving->SetDrawPos(m_DrawPos);
		if (m_IsLookingRight == true)
		{
			m_pSpriteDying->SetDrawPos(Point2f{m_DrawPos.x - m_pSpriteMoving->GetFrameWidth() * 0.5f, m_DrawPos.y});
		}
		else
		{
			m_pSpriteDying->SetDrawPos(Point2f{m_DrawPos.x - m_pSpriteMoving->GetFrameWidth(), m_DrawPos.y});

		}
		if (m_IsLookingRight == true)
		{
			m_pSpriteBloodParticles->SetDrawPos(Point2f{ m_DrawPos.x + 10.f, m_DrawPos.y });

		}
		else
		{
			m_pSpriteBloodParticles->SetDrawPos(Point2f{ m_DrawPos.x - 10.f, m_DrawPos.y });

		}

		break;
	case Zombie::ActionState::dying:
		m_HitBoxVertices[0] = Point2f{-1000.f, 1000.f};
		m_HitBoxVertices[1] = Point2f{ -1000.f, 1000.f };
		m_HitBoxVertices[2] = Point2f{ -1000.f, 1000.f };
		m_HitBoxVertices[3] = Point2f{ -1000.f, 1000.f };
		if (m_HasFly == true)
		{
			m_FlyPos.x += m_Velocity.x * elapsedSec;
			m_FlyPos.y += m_Velocity.x * elapsedSec;
			m_pSpriteFly->SetDrawPos(m_FlyPos);
			m_pSpriteFly->Update(elapsedSec);
		}
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

				m_pSpriteDying->SetCurrentFrame(1);
				m_pSpriteBloodParticles->SetCurrentFrame(0);
			}
		}
		else
		{
			m_pSpriteDying->Update(elapsedSec);
		}
		
		if (m_pSpriteBloodParticles->GetCurrentFrame() == m_pSpriteBloodParticles->GetTotalAmountOfFrames() - 1)
		{
			m_pSpriteBloodParticles->SetCurrentFrame(m_pSpriteBloodParticles->GetTotalAmountOfFrames() - 1);
		}
		else
		{
			m_pSpriteBloodParticles->Update(elapsedSec);
		}
		break;
	}
	
	if (m_CanSpawn == true && m_HasSpawned == false)
	{
		// spawn zombie
		if (playerPos.x <= levelWidth / 4.f)
		{
			//m_DrawPos.x = playerPos.x + 320.f;
			m_DrawPos.x = playerPos.x + float(rand() % 251 + 160);
			
			m_IsLookingRight = false;
			// spawn right side of player
		}
		else if (playerPos.x >=  levelWidth - levelWidth / 4.f)
		{
			//m_DrawPos.x = playerPos.x - 320.f;
			m_DrawPos.x = playerPos.x - float(rand() % 251 + 160);
			
			m_IsLookingRight = true;
			// spawn left side of player
		}
		else
		{
			if (rand() % 2 == 0)
			{
				m_DrawPos.x = playerPos.x + float(rand() % 201 + 160);
				m_IsLookingRight = false;

			}
			else
			{
				m_DrawPos.x = playerPos.x - float(rand() % 201 + 160);
				m_IsLookingRight = true;
			}

		}
		m_pSpriteMoving->SetCurrentFrame(rand() % 4);
		m_MovementSpeed = float(rand() % 10 + 10) / 10.f;
		m_pSpriteMoving->SetFrameRate(int(m_MovementSpeed * 6.f));
		m_Velocity.x = m_MovementSpeed * m_MovementScale;



		m_DrawPos.y = 14.f; // currently hardcoded, only spawns in 1-2, on this y level...
		m_HitBox.bottom = m_DrawPos.y;
		m_HasSpawned = true;
		m_HitBox.left = m_DrawPos.x;
		m_pSpriteMoving->SetDrawPos(m_DrawPos);
		
		// fly
		if (rand() % 8 == 0)
		{
			m_HasFly = true;
			m_Value = 150;
			// have to check value of zombie when killed with and without fly
		}
		else
		{
			m_HasFly = false;
			m_Value = 100;
		}

	}
	if (m_HasSpawned == true && utils::GetDistance(playerPos, m_DrawPos) > 550.f && m_RepeatSpawn == true)
	{
		// when zombie is outside of view, we respawn it
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

void Zombie::Draw() const
{
	if (m_HasFly == true)
	{
		m_pSpriteFly->Draw();
	}
	switch (m_CurrentState)
	{
	case Zombie::ActionState::moving:
		/*if (m_HasSpawned == true)
		{
			if (m_IsLookingRight == true)
			{
				glPushMatrix();
				glTranslatef(m_DrawPos.x + m_pSpriteMoving->GetFrameWidth() / 2, m_DrawPos.y, 0.f);
				glScalef(-1.f, 1.f, 1.f);
				glTranslatef(-m_DrawPos.x - m_pSpriteMoving->GetFrameWidth() / 2, -m_DrawPos.y, 0.f);
				m_pSpriteMoving->Draw();
				glPopMatrix();

			}
			else
			{
				m_pSpriteMoving->Draw();
			}
		}*/
		DrawSprite(m_pSpriteMoving);
		// test hitbox
		//utils::SetColor(Color4f{1.f,1.f,0.f,1.f});
		//utils::DrawRect(m_HitBox, 2.f);

		break;
	case Zombie::ActionState::dying:
		if (m_IsLookingRight == true)
		{
			glPushMatrix();
			{
				glTranslatef(m_DrawPos.x + m_pSpriteDying->GetFrameWidth() / 2, m_DrawPos.y, 0.f);
				glScalef(-1.f, 1.f, 1.f);
				glTranslatef(-m_DrawPos.x - m_pSpriteDying->GetFrameWidth() / 2, -m_DrawPos.y, 0.f);
				m_pSpriteBloodParticles->Draw();
				m_pSpriteDying->Draw();
			}
			glPopMatrix();

		}
		else
		{
			m_pSpriteBloodParticles->Draw();
			m_pSpriteDying->Draw();
		}
		break;
	}
}

