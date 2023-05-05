#include "pch.h"
#include "Raven.h"
#include "Sprite.h"
#include "utils.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "SoundEffect.h"

Raven::Raven(const Point2f& drawPos, bool isLookingRight, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Object{ drawPos }
	, m_CurrentState{ActionState::idle}
	, m_IsLookingRight{isLookingRight}
	, m_IsMovingRight{isLookingRight}
	, m_EatingTimer{0.f}
	, m_RandomEatingTimer{0.f}
	, m_featherAngle{0.f}
	, m_featherAngleIncrement{0.11f}
	, m_MovementSpeed{100.f}
{
	m_pSpriteIdle		= new Sprite("Resources/Sprites/Objects/Raven/Raven_Idle.png", 3, 1, 12, Point2f{drawPos}, Sprite::SpriteType::Other, pTextureManager);
	m_pSpriteMoving		= new Sprite("Resources/Sprites/Objects/Raven/Raven_Moving.png", 4, 1, 14, Point2f{drawPos}, Sprite::SpriteType::Other, pTextureManager);
	m_pSpriteDying		= new Sprite("Resources/Sprites/Objects/Raven/Raven_Dying.png", 3, 1, 6, Point2f{drawPos}, Sprite::SpriteType::Other, pTextureManager);
	m_pSpriteFeather	= new Sprite("Resources/Sprites/Objects/Raven/Raven_Feather.png", 1, 1, 6, Point2f{drawPos}, Sprite::SpriteType::Other, pTextureManager);
	m_RandomEatingTimer = float(rand() % 3) / 2.f; // random time between 0 and 2 seconds
	m_FeatherPos.push_back(Point2f{ drawPos.x + 20.f, drawPos.y + 20.f});
	m_FeatherPos.push_back(Point2f{ drawPos.x + 10.f, drawPos.y + 5.f });
	m_FeatherPos.push_back(Point2f{ drawPos.x - 10.f, drawPos.y - 5.f });
	m_FeatherPos.push_back(Point2f{ drawPos.x, drawPos.y + 10.f });

	m_pFlyAwaySoundeffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/BirdFlyingAway.mp3");
}

Raven::~Raven()
{
	delete m_pSpriteIdle;
	delete m_pSpriteMoving;
	delete m_pSpriteDying;
	delete m_pSpriteFeather;

	m_pSpriteIdle = nullptr;
	m_pSpriteMoving = nullptr;
	m_pSpriteDying = nullptr;
	m_pSpriteFeather = nullptr;
}

void Raven::Draw() const
{
	switch (m_CurrentState)
	{
	case Raven::ActionState::idle:
		if (m_IsLookingRight == true)
		{
			glPushMatrix();
			glTranslatef(m_DrawPos.x + m_pSpriteIdle->GetFrameWidth() / 2, m_DrawPos.y, 0.f);
			glScalef(-1.f, 1.f, 1.f);
			glTranslatef(-m_DrawPos.x - m_pSpriteIdle->GetFrameWidth() / 2, -m_DrawPos.y, 0.f);

			m_pSpriteIdle->Draw();


			glPopMatrix();
		}
		else
		{
			m_pSpriteIdle->Draw();

		}
		break;
	case Raven::ActionState::moving:
		if (m_IsMovingRight == true)
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
		// draw feather
		// update pos
		// repeat for amount of feathers

		for (Point2f ptr: m_FeatherPos)
		{
			m_pSpriteFeather->SetDrawPos(ptr);
			m_pSpriteFeather->Draw();
		}

		break;
	case Raven::ActionState::dying:
		m_pSpriteDying->Draw();
		break;
	}
}

void Raven::Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle)
{
	switch (m_CurrentState)
	{
	case Raven::ActionState::idle:
		m_EatingTimer += elapsedSec;
		if (m_EatingTimer < m_RandomEatingTimer)
		{
			if (int(m_EatingTimer * 5) % 2 == 0)
			{
				m_pSpriteIdle->SetCurrentFrame(1);
			}
			else
			{
				m_pSpriteIdle->SetCurrentFrame(2);
			}
		}
		else
		{
			m_pSpriteIdle->SetCurrentFrame(0);
			if (m_EatingTimer >= 1.5f) // reset eating after 5 seconds
			{
				m_RandomEatingTimer = float(rand() % 3) / 2.f;
				m_EatingTimer = 0.f;
			}

		}

		break;
	case Raven::ActionState::moving:
		m_pSpriteMoving->Update(elapsedSec);
		if (m_IsMovingRight == true)
		{
			m_DrawPos.x += m_MovementSpeed * elapsedSec;
			m_DrawPos.y += m_MovementSpeed * elapsedSec;
			m_pSpriteMoving->SetDrawPos(m_DrawPos);
		}
		else
		{
			m_DrawPos.x -= m_MovementSpeed * elapsedSec;
			m_DrawPos.y += m_MovementSpeed * elapsedSec;
			m_pSpriteMoving->SetDrawPos(m_DrawPos);
		}
		
		m_featherAngle += m_featherAngleIncrement;
		for (size_t i{0}; i < m_FeatherPos.size(); ++i)
		{
			--m_FeatherPos[i].y;
			m_FeatherPos[i].x += sinf(m_featherAngle) * 2.5f;
		}


		break;
	case Raven::ActionState::dying:
		m_pSpriteDying->Update(elapsedSec);
		break;
	}
}

void Raven::IsHit(const std::vector<Rectf>& playerAttackHitBox, const Rectf& playerHitBox, int playerLevel, DropManager* dropList, TextureManager* pTextureManager, SoundManager* pSoundManager)
{
	// this uses distance from center player to drawpos of raven
	// later will change hitbox of player to vector of Point2f, then check each point with center of raven
	if (utils::GetDistance(Point2f{ playerHitBox.left + playerHitBox.width/2, playerHitBox.bottom + playerHitBox.height/2}, m_DrawPos) < 120.f)
	{
		if (playerHitBox.left + playerHitBox.width / 2 < m_DrawPos.x)
		{
			m_IsMovingRight = true;
		}
		else
		{
			m_IsMovingRight = false;
		}
		if (m_CurrentState != ActionState::moving)
		{
			m_pFlyAwaySoundeffect->Play(5);

		}
		m_CurrentState = ActionState::moving;
		
	}
}
