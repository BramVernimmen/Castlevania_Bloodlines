#include "pch.h"
#include "DropManager.h"
//#include "SoundEffect.h"
#include "Sprite.h"
#include "Drop.h"

#include <iostream>
//class Drop;

DropManager::DropManager()
	//: m_pPowerUpSound{ new SoundEffect{"Resources/Sounds/powerUp.mp3"}}
{

}
DropManager::~DropManager()
{
	for (size_t dropIndex{0}; dropIndex < m_pDropManager.size(); ++dropIndex)
	{
		delete m_pDropManager[dropIndex];
		m_pDropManager[dropIndex] = nullptr;
	}
	m_pDropManager.clear();

}

Drop* DropManager::AddItem(const Point2f& pos, Drop::DropType type, int value, int playerLevel, TextureManager* pTextureManager, SoundManager* pSoundManager)
{
	m_pDropManager.push_back(new Drop(pos, type, value, playerLevel, pTextureManager, pSoundManager));
	return m_pDropManager.back();
}

void DropManager::Update(float elapsedSec, const Level* level)
{
	for (Drop* e: m_pDropManager)
	{
		e->Update(elapsedSec, level);
	}
}

void DropManager::Draw() const
{
	for (Drop* e: m_pDropManager)
	{
		e->Draw();
	}
}

size_t DropManager::Size() const
{
	return m_pDropManager.size();
}

void DropManager::HitItem(const Rectf& playerHitBox, Player* player)
{
	
	
	for (int dropIndex{int(m_pDropManager.size()) - 1}; dropIndex >= 0; --dropIndex)
	{
		if (m_pDropManager[dropIndex]->IsHit(playerHitBox, player) == true || m_pDropManager[dropIndex]->m_IsDestroyed == true)
		{
			delete m_pDropManager[dropIndex];
			m_pDropManager[dropIndex] = m_pDropManager.back();
			m_pDropManager.pop_back();

			//m_pPowerUpSound->Play(0);
		}
	}
	
}
