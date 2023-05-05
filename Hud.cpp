#include "pch.h"
#include "Hud.h"
#include "utils.h"
#include "Sprite.h"
#include "Player.h"
#include "Level.h"
#include "BossManager.h"
#include "SoundManager.h"

Hud::Hud(const Window& window, float camScale, TextureManager* pTextureManager)
	: m_HudColor{ Color4f{33 / 255.f, 0.f, 33 / 255.f, 1.f} }
	//, m_HudRect{ Rectf{ 0.f, window.height - (32.f * camScale), window.width, 32.f * camScale} } // hud is 32 px high in original game
	, m_HudRect{ Rectf{ 0.f,224.f - 32.f, 320.f, 32.f} } // hud is 32 px high in original game
	, m_CamScale{camScale}
	, m_PlayerHealthBarPosition{Point2f{66.f, m_HudRect.bottom + 9.f}}
	, m_EnemyHealthBarPosition{Point2f{66.f, m_HudRect.bottom + 1.f}}
	, m_AmountOfHealthBars{20}
	, m_ScorePosition{Point2f{120.f, m_HudRect.bottom + 18.f}}
	, m_GemsPosition{ Point2f{209.f, m_HudRect.bottom + 1.f} }
	, m_CurrentLevelPosition{ Point2f{270.f, m_HudRect.bottom + 17.f}}
	, m_CurrentStagePosition{ Point2f{298.f, m_HudRect.bottom + 17.f}}
	, m_PlayerAmountOfLivesPosition{ Point2f{293.f, m_HudRect.bottom + 1.f} }
	, m_VolumePosition{ Point2f{232.f, 44.f} }
{
	m_pHudElementsNonChangable = new Sprite("Resources/Sprites/Misc/HUD/HudElementsNonChangable.png", 1, 1, 1, Point2f{ m_HudRect.left, m_HudRect.bottom }, Sprite::SpriteType::Other, pTextureManager);
	m_pHudNumbers = new Sprite("Resources/Sprites/Misc/HUD/HudNumbers.png", 10, 1, 1, Point2f{ m_HudRect.left, m_HudRect.bottom }, Sprite::SpriteType::Other, pTextureManager);
	
	m_pSubWeaponBoomerang = new Sprite("Resources/Sprites/Objects/Drops/Boomerang.png", 1, 1, 1, Point2f{ 162.f, m_HudRect.bottom + 9.f}, Sprite::SpriteType::Other, pTextureManager);
	m_pSubWeaponBattleAxe = new Sprite("Resources/Sprites/Objects/Drops/BattleAxe.png", 1, 1, 1, Point2f{ 162.f, m_HudRect.bottom + 8.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pSubWeaponSacredWater = new Sprite("Resources/Sprites/Objects/Drops/SacredWater.png", 1, 1, 1, Point2f{ 163.f, m_HudRect.bottom + 8.f }, Sprite::SpriteType::Other, pTextureManager);
	
	m_pHealthBar = new Sprite("Resources/Sprites/Misc/HUD/HealthBar.png", 2, 1, 1, m_PlayerHealthBarPosition, Sprite::SpriteType::Other, pTextureManager);

	m_pPause = new Sprite("Resources/Sprites/Misc/HUD/Pause.png", 1, 1, 1, Point2f{ 0.f,0.f }, Sprite::SpriteType::Other, pTextureManager);
	m_pPause->SetDrawPos(Point2f{160.f - m_pPause->GetFrameWidth() / 2.f, 112.f});

	m_pVolume = new Sprite("Resources/Sprites/Misc/HUD/Volume_Text.png", 1, 1, 1, Point2f{0.f, m_VolumePosition.y}, Sprite::SpriteType::Other, pTextureManager);

	m_pNumbers = new Sprite("Resources/Sprites/Misc/TitleScreen/GameOver_Numbers.png", 10, 1, 1, Point2f{0.f, 0.f}, Sprite::SpriteType::Other, pTextureManager);
}

Hud::~Hud()
{
	delete m_pHudElementsNonChangable;
	m_pHudElementsNonChangable = nullptr;

	delete m_pHudNumbers;
	m_pHudNumbers = nullptr;
		
	delete m_pSubWeaponBoomerang;
	m_pSubWeaponBoomerang = nullptr;

	delete m_pSubWeaponBattleAxe;
	m_pSubWeaponBattleAxe = nullptr;

	delete m_pSubWeaponSacredWater;
	m_pSubWeaponSacredWater = nullptr;

	delete m_pHealthBar;
	m_pHealthBar = nullptr;

	delete m_pPause;
	m_pPause = nullptr;

	delete m_pVolume;
	m_pVolume = nullptr;

	delete m_pNumbers;
	m_pNumbers = nullptr;

}

void Hud::Draw(Player* pPlayer, Level* pCurrentLevel, BossManager* pBossManager, int currentLevelIndex) const
{
	glPushMatrix();
	{
		glScalef(m_CamScale, m_CamScale, 1.f);
		utils::SetColor(m_HudColor);
		utils::FillRect(m_HudRect);
		m_pHudElementsNonChangable->Draw();


		float pixelBuffer{ 1.f };

		// current subweapon
		{
			switch (pPlayer->GetSubWeapon())
			{
			case Player::CurrentSubWeapon::boomerang:
				m_pSubWeaponBoomerang->Draw();
				break;
			case Player::CurrentSubWeapon::battleAxe:
				m_pSubWeaponBattleAxe->Draw();
				break;
			case Player::CurrentSubWeapon::sacredWater:
				m_pSubWeaponSacredWater->Draw();
				break;
			}
		}

		// player healthbar
		{
			float healthPerBar{ pPlayer->GetMaximumHealth() / m_AmountOfHealthBars };
			int amountOfFullBars{int( pPlayer->GetCurrentHealth() / healthPerBar )};
			for (int index{ 0 }; index < m_AmountOfHealthBars ; ++index)
			{
				if (index >= amountOfFullBars)
				{
					m_pHealthBar->SetCurrentFrame(1);
				}
				else
				{
					m_pHealthBar->SetCurrentFrame(0);
				}
				m_pHealthBar->SetDrawPos(Point2f{ m_PlayerHealthBarPosition.x + (m_pHealthBar->GetFrameWidth() * index) + (pixelBuffer * index), m_PlayerHealthBarPosition.y });
				m_pHealthBar->Draw();
			}
		}
		
		// enemy healthbar
		{
			// only shows boss healthbar, so will need to add more later
			float healthPerBar{ pBossManager->GetBossMaximumHealth() / m_AmountOfHealthBars };
			int amountOfFullBars{ int(pBossManager->GetBossCurrentHealth() / healthPerBar) };
			for (int index{0}; index < m_AmountOfHealthBars; ++index)
			{
				// since no enemy yet, just make an empty healthbar
				if (index >= amountOfFullBars)
				{
					m_pHealthBar->SetCurrentFrame(1);
				}
				else
				{
					m_pHealthBar->SetCurrentFrame(0);
				}

				m_pHealthBar->SetDrawPos(Point2f{ m_EnemyHealthBarPosition.x + (m_pHealthBar->GetFrameWidth() * index) + (pixelBuffer * index), m_EnemyHealthBarPosition.y });
				m_pHealthBar->Draw();
			}
		}

		// score
		{
			std::string scoreString{ };
			if (pPlayer->GetCurrentScore() < 10)
			{
				scoreString = "0" + std::to_string(pPlayer->GetCurrentScore());
			}
			else
			{
				scoreString = std::to_string(pPlayer->GetCurrentScore());
			}
			for (size_t index{1}; index < scoreString.size() + 1; ++index)
			{
				m_pHudNumbers->SetDrawPos(Point2f{ m_ScorePosition.x - (m_pHudNumbers->GetFrameWidth() * (index - 1)) - (pixelBuffer * (index - 1)), m_ScorePosition.y });
				//int value{ scoreString[scoreString.size() - index]};
				char charValue{scoreString[scoreString.size() - index]}; // we get the char of the index
				int correctValue{ int(charValue) - 48 };
				m_pHudNumbers->SetCurrentFrame(correctValue);
				m_pHudNumbers->Draw();
			}
		}

		// gems
		{
			std::string gemString{};
			if (pPlayer->GetAmountOfGems() < 10)
			{
				gemString = "0" + std::to_string(pPlayer->GetAmountOfGems());
			}
			else
			{
				gemString = std::to_string(pPlayer->GetAmountOfGems());
			}
			for (size_t index{1}; index < gemString.size() + 1; ++index)
			{
				m_pHudNumbers->SetDrawPos(Point2f{ m_GemsPosition.x - (m_pHudNumbers->GetFrameWidth() * (index - 1)) - (pixelBuffer * (index - 1)), m_GemsPosition.y });
				char charValue{ gemString[gemString.size() - index] }; // we get the char of the index
				int correctValue{ int(charValue) - 48 };
				m_pHudNumbers->SetCurrentFrame(correctValue);
				m_pHudNumbers->Draw();
			}
		}

		// current level
		{
			m_pHudNumbers->SetDrawPos(Point2f{ m_CurrentLevelPosition.x , m_CurrentLevelPosition.y });
			m_pHudNumbers->SetCurrentFrame(currentLevelIndex + 1);
			m_pHudNumbers->Draw();
		}

		// current stage
		{
			std::string stageString{std::to_string(pCurrentLevel->GetStageNumber())};
			for (size_t index{ 1 }; index < stageString.size() + 1; ++index)
			{
				m_pHudNumbers->SetDrawPos(Point2f{ m_CurrentStagePosition.x - (m_pHudNumbers->GetFrameWidth() * (index - 1)) - (pixelBuffer * (index - 1)), m_CurrentStagePosition.y });
				char charValue{ stageString[stageString.size() - index] }; // we get the char of the index
				int correctValue{ int(charValue) - 48 };
				m_pHudNumbers->SetCurrentFrame(correctValue);
				m_pHudNumbers->Draw();
			}
		}

		// lives left
		{
			std::string livesString{};
			int livesCorrection{ 1 }; // player will see how many lives are left
			if (pPlayer->GetAmountOfLives() - livesCorrection < 10)
			{
				livesString = "0" + std::to_string(pPlayer->GetAmountOfLives() - livesCorrection);
			}
			else
			{
				livesString = std::to_string(pPlayer->GetAmountOfLives() - livesCorrection);
			}
			for (size_t index{ 1 }; index < livesString.size() + 1; ++index)
			{
				m_pHudNumbers->SetDrawPos(Point2f{ m_PlayerAmountOfLivesPosition.x - (m_pHudNumbers->GetFrameWidth() * (index - 1)) - (pixelBuffer * (index - 1)), m_PlayerAmountOfLivesPosition.y });
				char charValue{ livesString[livesString.size() - index] }; // we get the char of the index
				int correctValue{ int(charValue) - 48 };
				m_pHudNumbers->SetCurrentFrame(correctValue);
				m_pHudNumbers->Draw();
			}
		}
	}
	glPopMatrix();
}

void Hud::DrawPause(SoundManager* pSoundManager) const
{
	glPushMatrix();
	{
		glScalef(m_CamScale, m_CamScale, 1.f);
		m_pPause->Draw();

		float pixelBuffer{ 1.f };
		int globalSoundInPercent{ int(float(pSoundManager->GetGlobalValue() / 128.f) * 100) };

		m_pVolume->Draw();
		// volume
		{
			std::string volumeString{};
			if (globalSoundInPercent < 10)
			{
				volumeString = "00" + std::to_string(globalSoundInPercent);
			}
			else if (globalSoundInPercent < 100)
			{
				volumeString = "0" + std::to_string(globalSoundInPercent);
			}
			else
			{
				volumeString = std::to_string(globalSoundInPercent);
			}

			for (size_t index{ 1 }; index < volumeString.size() + 1; ++index)
			{
				m_pNumbers->SetDrawPos(Point2f{ m_VolumePosition.x - (m_pNumbers->GetFrameWidth() * (index - 1)) - (pixelBuffer * (index - 1)), m_VolumePosition.y });
				char charValue{ volumeString[volumeString.size() - index] }; // we get the char of the index
				int correctValue{ int(charValue) - 48 };
				m_pNumbers->SetCurrentFrame(correctValue);
				m_pNumbers->Draw();
			}
		}

		
	}
	glPopMatrix();
}
