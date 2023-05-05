#include "pch.h"
#include "Menu.h"
#include "SoundEffect.h"
#include "SoundManager.h"

Menu::Menu(SoundManager* pSoundManager)
	: m_CurrentSelectionIndex{0}
	, m_ButtonFlicker{0.f}
	, m_CountDownStarted{false}
{
	m_pPointerMoveSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/MenuMovePointer.mp3");
	m_pSelectedSoundEffect = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/MenuSelectedButton.mp3");
}

bool Menu::ResetGame()
{
	return false;
}

int Menu::GetAmountOfLives()
{
	return 0;
}
