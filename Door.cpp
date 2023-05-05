#include "pch.h"
#include "Door.h"
#include "Sprite.h"
#include "utils.h"
#include "SoundManager.h"
#include "SoundEffect.h"

Door::Door(const Point2f& drawPos, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: Object{ drawPos }
	, m_CurrentTimer{0.f}
	, m_MaxTimer{0.5f}
	, m_MaxOpenTimer{1.f}
	, m_CurrentFrame{0}
	, m_FrameUp{true}
	, m_CanOpen{false}
	, m_pSpriteDoor{new Sprite("Resources/Sprites/Objects/Door/Door_01.png", 3, 1, 1, drawPos, Sprite::SpriteType::Other, pTextureManager)}
	, m_PlayedSoundOnce{false}
{
	m_TriggerPos.x = m_DrawPos.x;
	m_TriggerPos.y = m_DrawPos.y += m_pSpriteDoor->GetFrameHeight() / 2.f;
	m_pOpenCloseSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/DoorOpenAndClose.mp3");
}

Door::~Door()
{
	delete m_pSpriteDoor;
	m_pSpriteDoor = nullptr;
}

void Door::Draw() const
{
	m_pSpriteDoor->Draw();
}

void Door::Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle)
{
	if (m_CanOpen == true)
	{
		m_pSpriteDoor->SetCurrentFrame(m_CurrentFrame);

		if (m_FrameUp == true)
		{
			if (m_PlayedSoundOnce == false)
			{
				m_pOpenCloseSoundEffect->Play(0);
				m_PlayedSoundOnce = true;
			}
			if (m_CurrentFrame < m_pSpriteDoor->GetTotalAmountOfFrames() - 1)
			{
				m_CurrentTimer += elapsedSec;
				if (m_CurrentTimer >= m_MaxTimer)
				{
					++m_CurrentFrame;
					m_CurrentTimer = 0.f;
				}
			}
			else // when at last frame, we want the door to stay open for a few seconds so the player "can" pass
			{
				m_CurrentTimer += elapsedSec;
				if (m_CurrentTimer >= m_MaxOpenTimer)
				{
					m_FrameUp = false;
					m_PlayedSoundOnce = false;
					m_CurrentTimer = 0.f;
				}
			}
			
		}
		else // door is closing again
		{
			if (m_PlayedSoundOnce == false)
			{
				m_pOpenCloseSoundEffect->Play(0);
				m_PlayedSoundOnce = true;
			}
			if (m_CurrentFrame > 0)
			{
				m_CurrentTimer += elapsedSec;
				if (m_CurrentTimer >= m_MaxTimer)
				{
					--m_CurrentFrame;
					m_CurrentTimer = 0.f;
				}
			}
			else // we are back at the first frame -> door is closed
			{
				m_CanOpen = false;
				m_FrameUp = true;
				m_PlayedSoundOnce = false;
			}
		}
	}
}


void Door::IsHit(const std::vector<Rectf>& playerAttackHitBox, const Rectf& playerHitBox, int playerLevel, DropManager* dropList, TextureManager* pTextureManager, SoundManager* pSoundManager)
{
	if (utils::GetDistance(Point2f{ playerHitBox.left + playerHitBox.width / 2.f, playerHitBox.bottom + playerHitBox.height / 2.f }, m_TriggerPos) < 20.f)
	{
		m_CanOpen = true;
	}

}
