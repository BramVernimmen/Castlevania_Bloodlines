#include "pch.h"
#include "HellHound.h"
#include "Sprite.h"
#include "utils.h"

#include <iostream>

HellHound::HellHound(float verticalTrigger, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Boss(pSoundManager, "Resources/Sounds/Streams/Small_Boss.mp3")
	, m_pSpriteMoving{ new Sprite("Resources/Sprites/Bosses/HellHound/HellHound_Walking.png", 4, 1, 10, m_DrawPos, Sprite::SpriteType::Other, pTextureManager) }
	, m_pSpriteDying{ new Sprite("Resources/Sprites/Bosses/HellHound/HellHound_Dying.png", 6, 1, 12, m_DrawPos, Sprite::SpriteType::Other, pTextureManager) }
	, m_pSpriteDespawning{ new Sprite("Resources/Sprites/Bosses/HellHound/HellHound_Despawning.png", 2, 1, 8, m_DrawPos, Sprite::SpriteType::Other, pTextureManager) }
	, m_pSpriteIdle{ new Sprite("Resources/Sprites/Bosses/HellHound/HellHound_Idle.png", 2, 1, 8, m_DrawPos, Sprite::SpriteType::Other, pTextureManager) }
	, m_pSpriteAttackingFire{ new Sprite("Resources/Sprites/Bosses/HellHound/HellHound_Attacking_Fire.png", 2, 1, 8, m_DrawPos, Sprite::SpriteType::Other, pTextureManager) }
	, m_pSpriteSmallFlame{ new Sprite("Resources/Sprites/Bosses/HellHound/HellHound_Fire_Small.png", 2, 1, 24, m_DrawPos, Sprite::SpriteType::Other, pTextureManager) }
	, m_pSpriteBigFlame{ new Sprite("Resources/Sprites/Bosses/HellHound/HellHound_Fire_Big.png", 2, 1, 24, m_DrawPos, Sprite::SpriteType::Other, pTextureManager) }
	, m_OriginalDespawnTimer{5.f}
	, m_OriginalAttackingTimer{2.f}
	, m_AmountOfFlames{6}
	, m_Angle{0.f}
	, m_AngleCosine{1.57f} // cosine of 1.57 = 0.f
{
	m_DespawnTimer = m_OriginalDespawnTimer;
	m_Value = 1200;
	m_Damage = 2.f;
	m_CurrentState = ActionState::none;
	//m_Health = 2.f;
	m_Health = 20.f;
	m_MaximumHealth = m_Health;
	m_DrawPos = Point2f{ 1050.f, 18.f }; // out of bounds
	m_RightWallPosition = m_DrawPos.x;

	m_HitBox.left = m_DrawPos.x;
	m_HitBox.bottom = m_DrawPos.y;
	m_HitBox.width = m_pSpriteIdle->GetFrameWidth();
	m_HitBox.height = m_pSpriteIdle->GetFrameHeight();
	m_MovementSpeed = 105.f;

	m_Velocity.x = m_MovementSpeed;


	m_VerticalTrigger = verticalTrigger;

	m_IdleWaitTimer = float(rand() % 15 + 5) / 10.f;
	m_AttackingTimer = m_OriginalAttackingTimer;

	m_AngleCosineScale = (m_OriginalAttackingTimer / 2.f) / m_AngleCosine;

	//m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom });
	//m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom + m_HitBox.height });
	//m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom });
	//m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom + m_HitBox.height });


}

HellHound::~HellHound()
{
	delete m_pSpriteMoving;
	m_pSpriteMoving = nullptr;

	delete m_pSpriteDying;
	m_pSpriteDying = nullptr;

	delete m_pSpriteDespawning;
	m_pSpriteDespawning = nullptr;

	delete m_pSpriteIdle;
	m_pSpriteIdle = nullptr;

	delete m_pSpriteAttackingFire;
	m_pSpriteAttackingFire = nullptr;

	delete m_pSpriteSmallFlame;
	m_pSpriteSmallFlame = nullptr;

	delete m_pSpriteBigFlame;
	m_pSpriteBigFlame = nullptr;


}

void HellHound::Update(float elapsedSec, Point2f playerPos)
{
	m_HitBoxVertices.clear(); // clearing the vector every frame, where needed, we will push in the correct values
	// doing it this way makes it easier to get the hitboxes for projectiles
	m_FlamePositions.clear();

	float distanceToTriggerPlayer{ 100.f };
	Point2f middlePointOfHellHound{ m_DrawPos.x + m_pSpriteIdle->GetFrameWidth() / 2.f, 0.f };

	//if ( m_CurrentState != ActionState::despawning)

	switch (m_CurrentState)
	{
	case Boss::ActionState::spawning:
	{
		if (m_DrawPos.x + m_pSpriteMoving->GetFrameWidth() >= m_RightWallPosition)
		{
			m_DrawPos.x -= m_MovementSpeed * elapsedSec; // needs to go left
			m_pSpriteMoving->Update(elapsedSec);
			UpdatePosition();
		}
		else
		{
			m_CurrentState = ActionState::moving;
		}
	}
		break;
	case Boss::ActionState::moving:
	{
		if (utils::GetDistance(middlePointOfHellHound, Point2f{playerPos.x, 0.f}) < distanceToTriggerPlayer)
		{
			m_CurrentState = ActionState::idle;
		}
		else
		{
			if (m_DrawPos.x < playerPos.x)
			{
				m_DrawPos.x += m_MovementSpeed * elapsedSec; // needs to go left
			}
			else
			{
				m_DrawPos.x -= m_MovementSpeed * elapsedSec; // needs to go left
			}
		}
		m_pSpriteMoving->Update(elapsedSec);
		UpdatePosition();


		// update hitbox vertices
		// move boss, at random times, make it use a random attack
		// move boss towards player, yet 
	}
		break;
	case Boss::ActionState::idle:
	{
		m_pSpriteIdle->Update(elapsedSec);
		if (utils::GetDistance(middlePointOfHellHound, Point2f{ playerPos.x, 0.f }) > distanceToTriggerPlayer)
		{
			m_CurrentState = ActionState::moving;
		}
		else
		{
			m_IdleWaitTimer -= elapsedSec;
			if (m_IdleWaitTimer <= 0.f)
			{
				m_CurrentState = ActionState::attacking;
				m_IdleWaitTimer = float(rand() % 15 + 5) / 10.f;
			}
			// wait for x amount of seconds, then switch to attack
		}
	}
		break;
	case Boss::ActionState::attacking:
	{
		m_AttackingTimer -= elapsedSec;
		m_pSpriteAttackingFire->Update(elapsedSec);
		m_pSpriteBigFlame->Update(elapsedSec);
		m_pSpriteSmallFlame->Update(elapsedSec);

		if (m_AttackingTimer <= 0.f)
		{
			m_CurrentState = ActionState::idle;
			m_AttackingTimer = m_OriginalAttackingTimer;
			m_Angle = 0.f;
		}

		if (m_AttackingTimer >= m_OriginalAttackingTimer / 2.f)
		{
			m_Angle += elapsedSec;
		}
		else
		{
			m_Angle -= elapsedSec;
		}
		
		if (m_AttackingTimer >= m_OriginalAttackingTimer / 2.f)
		{
			m_AngleCosine -= elapsedSec / m_AngleCosineScale;
		}
		else
		{
			m_AngleCosine += elapsedSec / m_AngleCosineScale;
		}

		//std::cout << cos(m_AngleCosine) << std::endl;
	}
		break;
	case Boss::ActionState::dying:
	{
		m_pSpriteDying->Update(elapsedSec);
		if (m_pSpriteDying->GetCurrentFrame() == m_pSpriteDying->GetTotalAmountOfFrames() - 1)
		{
			m_CurrentState = ActionState::despawning;
		}
	}
		break;
	case Boss::ActionState::despawning:
	{
		m_pSpriteDespawning->Update(elapsedSec);
		if (m_DespawnTimer >= 0.f)
		{
			m_DespawnTimer -= elapsedSec;
		}
	}
		break;
	case Boss::ActionState::none:
	{
		if (playerPos.x >= m_VerticalTrigger)
		{
			m_CurrentState = ActionState::spawning;
		}
	}
		break;
	}
	

	if (m_Health <= 0.f && m_CurrentState != ActionState::despawning)
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

	if (m_CurrentState != ActionState::dying && m_CurrentState != ActionState::despawning && m_CurrentState != ActionState::none)
	{
		if (middlePointOfHellHound.x < playerPos.x)
		{
			m_IsLookingRight = true;
		}
		else
		{
			m_IsLookingRight = false;
		}
		UpdatePosition();
	}

}

void HellHound::Draw() const
{
	
	switch (m_CurrentState)
	{
	case Boss::ActionState::spawning:
	{
		DrawSprite(m_pSpriteMoving);
	}
		break;
	case Boss::ActionState::moving:
	{
		DrawSprite(m_pSpriteMoving);
	}
		break;
	case Boss::ActionState::idle:
	{
		DrawSprite(m_pSpriteIdle);
	}
		break;
	case Boss::ActionState::attacking:
	{
		for (int index{ 0 }; index < m_AmountOfFlames; ++index)
		{
			int halfAmountOfFlames{ m_AmountOfFlames / 2 };
			if (index < halfAmountOfFlames)
			{
				m_pSpriteSmallFlame->SetDrawPos(m_FlamePositions[index]);
				m_pSpriteSmallFlame->Draw();
			}
			else
			{
				m_pSpriteBigFlame->SetDrawPos(m_FlamePositions[index]);
				m_pSpriteBigFlame->Draw();
			}
		}
		DrawSprite(m_pSpriteAttackingFire);
	}
		break;
	case Boss::ActionState::dying:
	{
		DrawSprite(m_pSpriteDying);
	}
		break;
	case Boss::ActionState::despawning:
	{
		if (m_DespawnTimer > 0.f)
		{
			if (m_DespawnTimer <= m_OriginalDespawnTimer / 2.f)
			{
				if (int(m_DespawnTimer * 10.f) % 2 == 0)
				{
					DrawSprite(m_pSpriteDespawning);
				}
				m_pSpriteDespawning->SetFrameRate(12);
			}
			else
			{
				DrawSprite(m_pSpriteDespawning);
			}
		}
	}
		break;
	}

	// hitbox debug
	//utils::SetColor(Color4f{1.f,1.f,0.f,1.f});
	//utils::DrawRect(m_HitBox);
}

void HellHound::UpdatePosition()
{
	m_pSpriteMoving->SetDrawPos(m_DrawPos);
	m_pSpriteDying->SetDrawPos(m_DrawPos);
	m_pSpriteDespawning->SetDrawPos(m_DrawPos);
	m_pSpriteIdle->SetDrawPos(m_DrawPos);
	m_pSpriteAttackingFire->SetDrawPos(m_DrawPos);

	m_HitBox.left = m_DrawPos.x + 10.f; // 10 pixels buffer so player doesn't get hit when the hound isn't there yet
	m_HitBox.bottom = m_DrawPos.y;


	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left, m_HitBox.bottom + m_HitBox.height });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom });
	m_HitBoxVertices.push_back(Point2f{ m_HitBox.left + m_HitBox.width, m_HitBox.bottom + m_HitBox.height });

	if (m_CurrentState == ActionState::attacking)
	{
		// add projectile hitboxes to the vector	 m_HitBoxVertices

		
		// first update their position
		{
			float cosineOffset{ cos(m_AngleCosine) };
			if (m_IsLookingRight == false)
			{
				for (int index{ 0 }; index < m_AmountOfFlames; ++index)
				{
					int halfAmountOfFlames{ m_AmountOfFlames / 2 };
					if (index < halfAmountOfFlames)
					{
						Point2f newPosition{};
						newPosition.x =  (m_DrawPos.x - cosineOffset *(m_pSpriteSmallFlame->GetFrameWidth() * index + 1));
						newPosition.y = m_DrawPos.y + (m_pSpriteAttackingFire->GetFrameHeight() / 2.f + (sinf(m_Angle * index ) * index * 5.f));

						m_FlamePositions.push_back(newPosition);
					}
					else
					{
						Point2f newPosition{};
						int correctIndexForSmallFlamePosition{ halfAmountOfFlames - 1 };
						newPosition.x = m_FlamePositions[correctIndexForSmallFlamePosition].x - cosineOffset * (m_pSpriteBigFlame->GetFrameWidth() * (index - correctIndexForSmallFlamePosition));
						newPosition.y = m_DrawPos.y + (m_pSpriteAttackingFire->GetFrameHeight() / 2.f + (sinf(m_Angle * index) * index * 5.f));
						m_FlamePositions.push_back(newPosition);
					}
				}
			}
			else
			{
				for (int index{ 0 }; index < m_AmountOfFlames; ++index)
				{
					int halfAmountOfFlames{ m_AmountOfFlames / 2 };
					if (index < halfAmountOfFlames)
					{
						Point2f newPosition{};
						newPosition.x = m_DrawPos.x + m_pSpriteAttackingFire->GetFrameWidth() + cosineOffset * (m_pSpriteSmallFlame->GetFrameWidth() * index);
						newPosition.y = m_DrawPos.y + (m_pSpriteAttackingFire->GetFrameHeight() / 2.f + (sinf(m_Angle * index) * index * 10.f));

						m_FlamePositions.push_back(newPosition);
					}
					else
					{
						Point2f newPosition{};
						int correctIndexForSmallFlamePosition{ halfAmountOfFlames - 1 };
						newPosition.x = m_FlamePositions[correctIndexForSmallFlamePosition].x + cosineOffset * (m_pSpriteBigFlame->GetFrameWidth() * (index - halfAmountOfFlames));
						newPosition.y = m_DrawPos.y + (m_pSpriteAttackingFire->GetFrameHeight() / 2.f + (sinf(m_Angle * index) * index * 10.f));
						m_FlamePositions.push_back(newPosition);
					}
				}
			}
		}
		

		// add to vector
		{
			for (int index{ 0 }; index < m_AmountOfFlames; ++index)
			{
				int halfAmountOfFlames{ m_AmountOfFlames / 2 };
				if (index < halfAmountOfFlames)
				{
					m_HitBoxVertices.push_back(m_FlamePositions[index]);
					m_HitBoxVertices.push_back(Point2f{m_FlamePositions[index].x, m_FlamePositions[index].y + m_pSpriteSmallFlame->GetFrameHeight()});
					m_HitBoxVertices.push_back(Point2f{m_FlamePositions[index].x + m_pSpriteSmallFlame->GetFrameWidth(), m_FlamePositions[index].y});
					m_HitBoxVertices.push_back(Point2f{m_FlamePositions[index].x + m_pSpriteSmallFlame->GetFrameWidth(), m_FlamePositions[index].y + m_pSpriteSmallFlame->GetFrameHeight()});
				}
				else
				{
					m_HitBoxVertices.push_back(m_FlamePositions[index]);
					m_HitBoxVertices.push_back(Point2f{ m_FlamePositions[index].x, m_FlamePositions[index].y + m_pSpriteBigFlame->GetFrameHeight() });
					m_HitBoxVertices.push_back(Point2f{ m_FlamePositions[index].x + m_pSpriteBigFlame->GetFrameWidth(), m_FlamePositions[index].y });
					m_HitBoxVertices.push_back(Point2f{ m_FlamePositions[index].x + m_pSpriteBigFlame->GetFrameWidth(), m_FlamePositions[index].y + m_pSpriteBigFlame->GetFrameHeight() });
				}
			}
		}
	}


	// DEBUG
	//for (int i{0}; i < m_HitBoxVertices.size(); ++i)
	//{
	//	std::cout << m_HitBoxVertices[i].x << ", " << m_HitBoxVertices[i].y << std::endl;
	//}
	//
	//std::cout << std::endl << std::endl;
}

