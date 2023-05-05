#include "pch.h"
#include "Player.h"
#include "Sprite.h"

#include "SoundEffect.h"
#include "SoundManager.h"

#include "utils.h"
#include <iostream>

Player::Player(const Point2f& drawPos, int amountOfLives, int amountOfContinues, TextureManager* pTextureManager, SoundManager* pSoundManager)
	: m_DrawPos{ drawPos }
	, m_pSpriteIdle{}
	, m_pSpriteJumping{}
	, m_pSpriteMoving{}
	, m_CurrentState{ ActionState::idle }
	, m_CurrentSubWeapon{CurrentSubWeapon::none}
	, m_LastUsedSubWeapon{CurrentSubWeapon::none}
	, m_SubWeaponIsActive{false}
	, m_Velocity{ Vector2f{0.0f, 0.0f } }
	, m_Acceleration{ Vector2f{ 0, -981.0f} }
	, m_IsLookingRight{ true }
	, m_HorSpeed{ 120.f }
	, m_JumpSpeed{ 350.f }
	//, m_JumpSpeed{ 600.f } // 600.f for testing
	, m_PlayerAttackHitBox{}
	, m_IsOnGround{ false }
	, m_IsInvincible{ false }
	, m_InvincibleTimer{ 0.f }
	, m_IsOnStairs{ false }
	, m_HasControl{ true }
	, m_IsCrouched{false}
	, m_CurrentLevel{1}
	, m_BaseAttackTimer{0.5f}
	, m_LongerAttackTimer{0.75f}
	, m_AttackAccumulatedTime{0.f}
	, m_Index {0}
	, m_IsGoingUpStairs{true}
	, m_AmountOfGems{0}
	, m_CurrentHealth{20}
	, m_MaximumHealth{20}
	, m_OriginalMaximumInvincibleTimer{5.f}
	, m_MaximumInvincibleTimer{5.f}
	, m_DamagedTimer{0.f}
	, m_DamageMaxTimer{1.f}
	, m_CurrentScore{0}
	, m_CurrentStairDirection{true}
	, m_AreThereStairs{false}
	, m_SubWeaponDirection{true}
	, m_IsSpecial{false}
	, m_VelocitySubWeapon{ Vector2f{0.0f, 0.0f } }
	, m_ThrowDirection{ Vector2f{100.0f, 350.0f } }
	, m_SubWeaponHitBox{ Rectf{-50.f, -50.f, 0.f, 0.f} } // place out of bounds
	, m_AmountOfLives{amountOfLives}
	, m_AmountOfContinues{amountOfContinues}
	, m_RespawnPos{drawPos}
	, m_RespawnStageIndex{0}
	, m_IsPlayerDead{false}
	, m_HighScore{0}
	, m_SubWeaponAngle{0.f}
	, m_SubWeaponAngleIncrement{0.05f}
{
	m_pSpriteIdle = new Sprite("Resources/Sprites/Player/John/John_Idle.png", 3, 1, 8, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
	m_pSpriteJumping = new Sprite("Resources/Sprites/Player/John/John_Jump.png", 3, 1, 15, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
	m_pSpriteMoving = new Sprite("Resources/Sprites/Player/John/John_Walk.png", 6, 1, 15, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
	m_pSpriteGoingDownStairs = new Sprite("Resources/Sprites/Player/John/John_GoingDownStairs.png", 6, 1, 15, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
	m_pSpriteGoingUpStairs = new Sprite("Resources/Sprites/Player/John/John_GoingUpStairs.png", 6, 1, 15, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
	m_pSpriteCrouching = new Sprite("Resources/Sprites/Player/John/John_Crouch.png", 1, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
	m_pSpriteDamaged = new Sprite("Resources/Sprites/Player/John/John_Damaged.png", 2, 1, 1, m_DrawPos, Sprite::SpriteType::Other, pTextureManager);
	
	m_pAttackingSprites.push_back(new Sprite("Resources/Sprites/Player/John/Attacking/John_Attacking_01.png", 6, 1, 16, m_DrawPos, Sprite::SpriteType::Other, pTextureManager));
	m_pAttackingSprites.push_back(new Sprite("Resources/Sprites/Player/John/Attacking/John_Attacking_02.png", 12, 1, 30, m_DrawPos, Sprite::SpriteType::Other, pTextureManager));
	m_pAttackingSprites.push_back(new Sprite("Resources/Sprites/Player/John/Attacking/John_Attacking_03.png", 12, 1, 30, m_DrawPos, Sprite::SpriteType::Other, pTextureManager));
	m_pAttackingSprites.push_back(new Sprite("Resources/Sprites/Player/John/Attacking/John_Attacking_04.png", 7, 1, 20, m_DrawPos, Sprite::SpriteType::Other, pTextureManager));
	m_pAttackingSprites.push_back(new Sprite("Resources/Sprites/Player/John/Attacking/John_Attacking_Crouched_01.png", 6, 1, 16, m_DrawPos, Sprite::SpriteType::Other, pTextureManager));
	m_pAttackingSprites.push_back(new Sprite("Resources/Sprites/Player/John/Attacking/John_Attacking_Crouched_02.png", 12, 1, 30, m_DrawPos, Sprite::SpriteType::Other, pTextureManager));
	m_pAttackingSprites.push_back(new Sprite("Resources/Sprites/Player/John/Attacking/John_Attacking_Crouched_03.png", 12, 1, 30, m_DrawPos, Sprite::SpriteType::Other, pTextureManager));
	m_pAttackingSprites.push_back(new Sprite("Resources/Sprites/Player/John/Attacking/John_Attacking_Crouched_04.png", 7, 1, 20, m_DrawPos, Sprite::SpriteType::Other, pTextureManager));
	
	m_SubWeaponDrawPos = m_DrawPos;
	m_SubWeaponCenterPos = m_DrawPos;

	m_pSpriteBigAxe = new Sprite("Resources/Sprites/Player/SubWeapons/BattleAxe/BigBattleAxe.png", 4, 1, 15, m_SubWeaponDrawPos, Sprite::SpriteType::Other, pTextureManager);
	m_pSpriteBoomerang = new Sprite("Resources/Sprites/Player/SubWeapons/Boomerang/Boomerang.png", 4, 1, 15, m_SubWeaponDrawPos, Sprite::SpriteType::Other, pTextureManager);

	m_PlayerHitBox.left = m_DrawPos.x;
	m_PlayerHitBox.bottom = m_DrawPos.y;
	m_PlayerHitBox.width = m_pSpriteIdle->GetFrameWidth();
	m_PlayerHitBox.height = m_pSpriteIdle->GetFrameHeight();

	m_pWhipSoundEffect1 = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/Whip1.mp3");
	m_pWhipSoundEffect2 = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/Whip2.mp3");
	m_pWhipSoundEffect3 = pSoundManager->GetSoundEffectPointer("Resources/Sounds/Effects/Whip3.mp3");
}

Player::~Player()
{
	delete m_pSpriteIdle;
	m_pSpriteIdle = nullptr;

	delete m_pSpriteMoving;
	m_pSpriteMoving = nullptr;

	delete m_pSpriteJumping;
	m_pSpriteJumping = nullptr;	
	
	delete m_pSpriteCrouching;
	m_pSpriteCrouching = nullptr;	

	delete m_pSpriteGoingDownStairs;
	m_pSpriteGoingDownStairs = nullptr;

	delete m_pSpriteGoingUpStairs;
	m_pSpriteGoingUpStairs = nullptr;
	
	delete m_pSpriteDamaged;
	m_pSpriteDamaged = nullptr;

	for (Sprite* ptr: m_pAttackingSprites)
	{
		delete ptr;
		ptr = nullptr;
	}

	delete m_pSpriteBigAxe;
	m_pSpriteBigAxe = nullptr;

	delete m_pSpriteBoomerang;
	m_pSpriteBoomerang = nullptr;

}

void Player::Update(float elapsedSec)
{
	if (m_CurrentScore > m_HighScore)
	{
		m_HighScore = m_CurrentScore;
	}


	//if (m_PlayerHitBox.bottom <= 5.f && !m_IsOnStairs) // 5 pixel buffer from the ground
	//{
	//	m_CurrentState = ActionState::damaged;
	//	m_CurrentHealth = 0;
	//	m_PlayerHitBox.bottom = -100.f;
	//}

	if (!(m_CurrentState == ActionState::attacking))
	{
		m_PlayerAttackHitBox = Rectf{-500.f, -500.f, 0.f, 0.f}; // out of bounds
	}

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (m_HasControl == true)
	{

		InputHandling(elapsedSec);
	}

	switch (m_CurrentState)
	{
	case Player::ActionState::idle:
		m_pSpriteIdle->Update(elapsedSec);
		break;

	case Player::ActionState::moving:
		if (m_IsOnStairs == false)
		{
			m_pSpriteMoving->Update(elapsedSec);

			m_PlayerHitBox.left += m_Velocity.x * elapsedSec;
			m_PlayerHitBox.bottom += m_Velocity.y * elapsedSec;
		}
		
		UpdatePos();
		break;

	case Player::ActionState::damaged:
		
		if (m_PlayerHitBox.bottom <= 5.f && !m_IsOnStairs) // 5 pixel buffer from the ground
		{
			m_CurrentState = ActionState::damaged;
			m_CurrentHealth = 0;
			m_PlayerHitBox.bottom = -100.f;
		}

		if (m_IsOnStairs == false)
		{
			m_Velocity.y += m_Acceleration.y * elapsedSec;
			m_PlayerHitBox.left += m_Velocity.x * elapsedSec;
			m_PlayerHitBox.bottom += m_Velocity.y * elapsedSec;
			// in ishit with enemy -> set hor en vert velocity
			if (m_IsOnGround == true)
			{
				// when player with the ground again, the player gets control again
				m_HasControl = true;
				m_CurrentState = ActionState::idle;
				m_MaximumInvincibleTimer = 2.f;
			}
			else
			{
				// update frames accordingly 
				if (m_Velocity.y > 80.f)
				{
					m_pSpriteDamaged->SetCurrentFrame(0);
				}
				else if (m_Velocity.y < 80.f)
				{
					m_pSpriteDamaged->SetCurrentFrame(1);
					// no need to update the hitbox, the player is invincible anyways
				}
				m_HasControl = false;
			}
		}
		else
		{
			m_DamagedTimer += elapsedSec;
			if (m_DamagedTimer <= m_DamageMaxTimer)
			{
				m_HasControl = false;
				m_pSpriteDamaged->SetCurrentFrame(0);
			}
			else
			{
				m_HasControl = true;
				m_MaximumInvincibleTimer = m_OriginalMaximumInvincibleTimer;
				m_DamagedTimer = 0.f;
				m_CurrentState = ActionState::moving;
			}
			// set sprite to frame 0, no control for 1 sec
		}
		
		if (m_CurrentHealth <= 0)
		{
			m_IsPlayerDead = true;
		}

		UpdatePos();

		break;

	case Player::ActionState::jumping:
		m_Velocity.y += m_Acceleration.y * elapsedSec;

		if (m_Velocity.y > 80.f)
		{
			m_pSpriteJumping->SetCurrentFrame(0);
		}
		else if (m_Velocity.y < 80.f && m_Velocity.y > -80.f)
		{
			m_pSpriteJumping->SetCurrentFrame(1);
		}
		else if (m_Velocity.y < -80.f)
		{
			m_pSpriteJumping->SetCurrentFrame(2);
		}


		m_PlayerHitBox.left += m_Velocity.x * elapsedSec;
		m_PlayerHitBox.bottom += m_Velocity.y * elapsedSec;

		UpdatePos();

		if (m_PlayerHitBox.bottom <= 5.f && !m_IsOnStairs) // 5 pixel buffer from the ground
		{
			m_CurrentState = ActionState::damaged;
			m_CurrentHealth = 0;
			m_PlayerHitBox.bottom = -100.f;
		}
		break;

	case Player::ActionState::attacking:
		m_AttackAccumulatedTime += elapsedSec;
		m_HasControl = false;
		if (m_IsOnGround == false && m_IsOnStairs == false)
		{
			m_Velocity.y += m_Acceleration.y * elapsedSec;
			m_PlayerHitBox.left += m_Velocity.x * elapsedSec;
			m_PlayerHitBox.bottom += m_Velocity.y * elapsedSec;

			UpdatePos();
			UpdateAttackState(elapsedSec);
		}
		else
		{
			m_Velocity.x = 0.f;
			UpdateAttackState(elapsedSec);
		}
		break;

	}

	

	if (m_IsInvincible == true && m_InvincibleTimer <= m_MaximumInvincibleTimer)
	{
		m_InvincibleTimer += elapsedSec;
	}
	if (m_InvincibleTimer > m_MaximumInvincibleTimer)
	{
		m_IsInvincible = false;
		m_InvincibleTimer = 0.f;
		m_MaximumInvincibleTimer = m_OriginalMaximumInvincibleTimer;
	}
	
	UpdatePos();

	if (m_SubWeaponIsActive == true)
	{
		UpdateSubWeapon(elapsedSec);
	}
	else
	{
		m_LastUsedSubWeapon = m_CurrentSubWeapon;
		m_SubWeaponAngle = 0.f;
	}


	

}

void Player::Draw() const
{
	if (m_IsInvincible == true)
	{
		if (int(m_InvincibleTimer * 500.f) % 2 == 0)
		{
			DrawPlayer();
		}
	}
	else
	{
		DrawPlayer();
	}

	if (m_SubWeaponIsActive == true)
	{
		glPushMatrix();
		if (m_SubWeaponDirection == true) // flip if looking right
		{
			glTranslatef(m_SubWeaponCenterPos.x, m_SubWeaponCenterPos.y, 0.f);
			glScalef(-1.f, 1.f, 1.f);
			glTranslatef(-m_SubWeaponCenterPos.x, -m_SubWeaponCenterPos.y, 0.f);
		}
		switch (m_LastUsedSubWeapon)
		{
		case Player::CurrentSubWeapon::battleAxe:
			m_pSpriteBigAxe->Draw();
			break;
		case Player::CurrentSubWeapon::boomerang:
			m_pSpriteBoomerang->Draw();
			break;
		case Player::CurrentSubWeapon::sacredWater:
			break;
		}
		glPopMatrix();
	}

}

Rectf& Player::GetPlayerHitBox()
{
	return m_PlayerHitBox;
}

//Rectf Player::GetPlayerAttackHitBox()
//{
//
//	return m_PlayerAttackHitBox;
//}

std::vector<Rectf> Player::GetPlayerAttackHitBox()
{
	std::vector<Rectf> attackHitBoxes;
	attackHitBoxes.push_back(m_PlayerAttackHitBox);
	if (m_SubWeaponIsActive == true)
	{
		if (m_IsSpecial == false)
		{
			attackHitBoxes.push_back(m_SubWeaponHitBox);
		}
	}
	
	return attackHitBoxes;
}

Vector2f& Player::GetPlayerVelocity()
{
	return m_Velocity;
}

float Player::GetPlayerHorSpeed() const
{
	return m_HorSpeed;
}

Point2f Player::GetPlayerPos()
{
	Point2f center{};
	center.x = m_PlayerHitBox.left + m_PlayerHitBox.width / 2.f;
	center.y = m_PlayerHitBox.bottom + m_PlayerHitBox.height / 2.f;
	return center;
}

Point2f Player::GetPlayerDrawPos() const
{
	return m_DrawPos;
}

float Player::GetPlayerDamage()
{
	if (m_CurrentLevel == 1)
	{
		return 1.f;
	}
	else if (m_CurrentLevel == 2)
	{
		return 1.33f;
	}
	else if (m_CurrentLevel == 3)
	{
		return 1.5f;
	}
	else if (m_CurrentLevel == 4)
	{
		return 2.f;
	}

	return 0.f;
}

void Player::SetPlayerState(ActionState newState)
{
	m_CurrentState = newState;
}

void Player::SetPlayerControl(bool hasControl)
{
	m_HasControl = hasControl;
}

void Player::IsHit(float pointOfContact, float damage)
{
	if (m_IsInvincible == false)
	{ 
		AddHealth(-damage);

		if (m_CurrentLevel == 4) // max level
		{
			m_CurrentLevel = 3; // we level down
		}
		// will need a function to check for dead aswell

		if (m_IsOnStairs == false)
		{
			if (pointOfContact < m_PlayerHitBox.left + m_PlayerHitBox.width / 2) // attack came from left
			{
				m_Velocity.x += m_HorSpeed;
			}
			else if (pointOfContact > m_PlayerHitBox.left + m_PlayerHitBox.width / 2) // attack came from right
			{
				m_Velocity.x -= m_HorSpeed;
			}
			else // attack came from center, should be very rare to happen
			{
				m_Velocity.x = 0.f;
			}
			m_Velocity.y = m_JumpSpeed;
		}
		else
		{
			m_Velocity.x = 0.f;
			m_Velocity.y = 0.f;
		}
		//m_IsOnStairs = false;
		//m_Velocity.x = 0.f;
		//m_Velocity.y = m_JumpSpeed;
		m_MaximumInvincibleTimer = 1.f;
		m_CurrentState = ActionState::damaged;
		m_IsOnGround = false;


		// if attack comes from left, add positive horspeed, else negative
		
		SetInvincible();
	}
	
}

void Player::SetInvincible()
{
	m_IsInvincible = true;
	m_InvincibleTimer = 0.f; 
}

void Player::LevelUp()
{
	if (m_CurrentLevel < 4)
	{
		if (m_IsInvincible == false)
		{
			m_MaximumInvincibleTimer = 0.75f;
			SetInvincible();
		}
		++m_CurrentLevel;
		m_AttackAccumulatedTime = 0.f;
		m_HasControl = true;
		m_pAttackingSprites[m_Index]->SetCurrentFrame(0);
		m_CurrentState = ActionState::idle;
	}
}

int Player::GetPlayerLevel() const
{
	return m_CurrentLevel;
}

void Player::AddGems(int amount)
{
	m_AmountOfGems += amount;
}

void Player::AddHealth(float amount)
{
	m_CurrentHealth += amount;
	if (m_CurrentHealth > m_MaximumHealth)
	{
		m_CurrentHealth = m_MaximumHealth;
	}
}

void Player::AddScore(int amount)
{
	m_CurrentScore += amount;
}

void Player::SetSubWeapon(CurrentSubWeapon newWeapon)
{
	switch (newWeapon)
	{
	case Player::CurrentSubWeapon::battleAxe:
		m_CurrentSubWeapon = CurrentSubWeapon::battleAxe;
		break;
	case Player::CurrentSubWeapon::boomerang:
		m_CurrentSubWeapon = CurrentSubWeapon::boomerang;
		break;
	case Player::CurrentSubWeapon::sacredWater:
		m_CurrentSubWeapon = CurrentSubWeapon::sacredWater;
		break;
	}
}

Player::CurrentSubWeapon Player::GetSubWeapon()
{
	return m_CurrentSubWeapon;
}

void Player::SetPos(const Point2f& newPos, int safeStageIndex)
{
	m_PlayerHitBox.left = newPos.x;
	m_PlayerHitBox.bottom = newPos.y;

	// if player isn't on stairs, we can set the new position as our safe position for when the player dies
	if (m_IsOnStairs == false)
	{
		m_RespawnPos = newPos;
		m_RespawnStageIndex = safeStageIndex;
		//std::cout << m_RespawnPos.x << "  " << m_RespawnPos.y << "  " << m_RespawnStageIndex << '\n';
	}
	//std::cout << m_IsOnStairs << std::endl;
}

void Player::SetGrounded(bool isOnGround)
{
	
	m_IsOnGround = isOnGround;

}

bool Player::IsPlayerOnStairs() const
{
	return m_IsOnStairs;
}

void Player::SetStairDirection(bool rightIsUp)
{
	m_CurrentStairDirection = rightIsUp;
}

void Player::CheckForStairs(bool areThereStairs)
{
	m_AreThereStairs = areThereStairs;
}

bool Player::PlayerGetStairsDirection() const
{
	return m_CurrentStairDirection;
}

bool Player::PlayerGetFacingDirection() const
{
	return m_IsLookingRight;
}

void Player::UpdateStairsForTransition(float elapsedSec)
{
	m_pSpriteGoingDownStairs->Update(elapsedSec);
	m_pSpriteGoingUpStairs->Update(elapsedSec);
	UpdatePos();
}

bool Player::MovePlayerFadeToBlack(float elapsedSec)
{
	// optimization still needed...

	SetPlayerControl(false);
	SetPlayerState(Player::ActionState::moving);

	if (IsPlayerOnStairs() == true)
	{
		UpdateStairsForTransition(elapsedSec);
		if (PlayerGetStairsDirection() == true) // going right up
		{
			if (PlayerGetFacingDirection() == true) // looking right
			{
				m_PlayerHitBox.left = m_DrawPos.x + 1.f;
				m_PlayerHitBox.bottom = m_DrawPos.y + 1.f;
				//SetPos(Point2f{ GetPlayerDrawPos().x + 1.f, GetPlayerDrawPos().y + 1.f });
			}
			else // looking left
			{
				m_PlayerHitBox.left = m_DrawPos.x - 1.f;
				m_PlayerHitBox.bottom = m_DrawPos.y - 1.f;
				//SetPos(Point2f{ GetPlayerDrawPos().x - 1.f,GetPlayerDrawPos().y - 1.f });
			}
		}
		else
		{
			if (PlayerGetFacingDirection() == true) // looking right
			{
				m_PlayerHitBox.left = m_DrawPos.x + 1.f;
				m_PlayerHitBox.bottom = m_DrawPos.y - 1.f;
				//SetPos(Point2f{ GetPlayerDrawPos().x + 1.f,GetPlayerDrawPos().y - 1.f });
			}
			else // looking left
			{
				m_PlayerHitBox.left = m_DrawPos.x - 1.f;
				m_PlayerHitBox.bottom = m_DrawPos.y + 1.f;
				//SetPos(Point2f{ GetPlayerDrawPos().x - 1.f, GetPlayerDrawPos().y + 1.f });
			}
		}
		SetPlayerControl(true);
		return true;
	}
	else
	{
		Update(elapsedSec);

		//m_pLevelManager->GetCurrentLevel(m_CurrentLevel, m_CurrentStage)->HandleCollision(m_pPlayer->GetPlayerHitBox(), m_pPlayer->GetPlayerVelocity());

		//m_pStagesLevel1[m_CurrentStage]->HandleCollision(m_pPlayer->GetPlayerHitBox(), m_pPlayer->GetPlayerVelocity());
		GetPlayerVelocity().x =GetPlayerHorSpeed();
		SetPlayerControl(true);
		return false;
	}

	// when jumping, the player triggers the collision, resetting the x value of velocity
	// we need to give the player the velocity back when this happens

	// check if player is on stairs, give correct movement
	return false;
}

void Player::MakePlayerWait(float elapsedSec)
{
	SetPlayerControl(false);
	if (GetPlayerVelocity().y != 0.f)
	{
		SetPlayerState(Player::ActionState::jumping);
		GetPlayerVelocity().x = 0.f;
		Update(elapsedSec);
	}
	else
	{
		SetPlayerState(Player::ActionState::idle);
		Update(elapsedSec);
	}
	SetPlayerControl(true);
}

void Player::MovePlayerThroughDoor(float elapsedSec, float newPos)
{
	if (GetPlayerDrawPos().x < newPos)
	{
		SetPlayerControl(false);
		SetPlayerState(Player::ActionState::moving);
		Update(elapsedSec);


		GetPlayerVelocity().x = GetPlayerHorSpeed() / 2.f; // slow the speed so player goes a bit slower
		SetPlayerControl(true);
	}
	else
	{
		SetPlayerControl(false);
		SetPlayerState(Player::ActionState::idle);
		Update(elapsedSec);
		SetPlayerControl(true);
	}

}

float Player::GetMaximumHealth()
{
	return m_MaximumHealth;
}

float Player::GetCurrentHealth()
{
	return m_CurrentHealth;
}

int Player::GetCurrentScore()
{
	return m_CurrentScore;
}

int Player::GetHighScore()
{
	return m_HighScore;
}

int Player::GetAmountOfGems()
{
	return m_AmountOfGems;
}

int Player::GetAmountOfLives()
{
	return m_AmountOfLives;
}

bool Player::IsPlayerDead()
{
	return m_IsPlayerDead;
}

int Player::GetRespawnLevelIndex()
{
	return m_RespawnStageIndex;
}

void Player::ResetPlayerAfterDeath(int newAmountOfLives)
{
	m_PlayerHitBox.left = m_RespawnPos.x;
	m_PlayerHitBox.bottom = m_RespawnPos.y;
	m_Velocity.x = 0.f;
	m_Velocity.y = 0.f;
	m_HasControl = true;
	m_CurrentState = ActionState::jumping;

	m_CurrentHealth = m_MaximumHealth;
	//--m_AmountOfLives;
	m_AmountOfLives = newAmountOfLives;
	m_CurrentSubWeapon = CurrentSubWeapon::none;
	m_AmountOfGems = 0;
	m_CurrentLevel = 1;
	m_InvincibleTimer = 10.f;

	m_IsPlayerDead = false;

	// TODO 1: all dying options
	// if amount of lives == 0
	// show menu "game over"
	// ask for continue or END
	// when continue = 0 and amount of lives = 0
	// only show END option
	// END = restart game
	// when continuing, reset the score aswell and reset the amount of lives back to original
	//
}

int& Player::GetAmountOfContinues()
{
	return m_AmountOfContinues;
}

void Player::ResetScore()
{
	m_CurrentScore = 0;
}


void Player::UpdatePos()
{
	m_DrawPos.x = m_PlayerHitBox.left;
	m_DrawPos.y = m_PlayerHitBox.bottom;

	m_pSpriteMoving->SetDrawPos(m_DrawPos);
	m_pSpriteIdle->SetDrawPos(m_DrawPos);
	m_pSpriteJumping->SetDrawPos(m_DrawPos);
	m_pSpriteCrouching->SetDrawPos(m_DrawPos);
	m_pSpriteGoingDownStairs->SetDrawPos(m_DrawPos);
	m_pSpriteGoingUpStairs->SetDrawPos(m_DrawPos);
	m_pSpriteDamaged->SetDrawPos(m_DrawPos);

	

	// normal attacking
	m_pAttackingSprites[0]->SetDrawPos(Point2f{ m_DrawPos.x - m_pSpriteIdle->GetFrameWidth(), m_DrawPos.y });
	
	m_pAttackingSprites[1]->SetDrawPos(Point2f{ m_DrawPos.x - m_pSpriteIdle->GetFrameWidth(), m_DrawPos.y });
	m_pAttackingSprites[2]->SetDrawPos(Point2f{ m_DrawPos.x - m_pSpriteIdle->GetFrameWidth(), m_DrawPos.y });
	m_pAttackingSprites[3]->SetDrawPos(Point2f{ m_DrawPos.x - m_pSpriteIdle->GetFrameWidth() / 4, m_DrawPos.y });
	// crouched attacking
	m_pAttackingSprites[4]->SetDrawPos(Point2f{ m_DrawPos.x - m_pSpriteIdle->GetFrameWidth() + 1.f, m_DrawPos.y - 2.f });
	m_pAttackingSprites[5]->SetDrawPos(Point2f{ m_DrawPos.x - m_pSpriteIdle->GetFrameWidth() + 1.f, m_DrawPos.y - 7.f });
	m_pAttackingSprites[6]->SetDrawPos(Point2f{ m_DrawPos.x - m_pSpriteIdle->GetFrameWidth() + 1.f, m_DrawPos.y - 7.f });
	m_pAttackingSprites[7]->SetDrawPos(Point2f{ m_DrawPos.x - m_pSpriteIdle->GetFrameWidth() / 1.67f, m_DrawPos.y });




	if (m_IsCrouched == true)
	{
		//m_PlayerHitBox.width = m_pSpriteCrouching->GetFrameWidth();
		m_PlayerHitBox.width = m_pSpriteIdle->GetFrameWidth();
		m_PlayerHitBox.height = m_pSpriteCrouching->GetFrameHeight();
	}
	else
	{
		m_PlayerHitBox.width = m_pSpriteIdle->GetFrameWidth();
		m_PlayerHitBox.height = m_pSpriteIdle->GetFrameHeight();
	}
}

void Player::UpdateSubWeapon(float elapsedSec)
{
	m_SubWeaponAngle += m_SubWeaponAngleIncrement;

	
	// update last weapon used
	switch (m_LastUsedSubWeapon)
	{
	case Player::CurrentSubWeapon::battleAxe:
		m_SubWeaponCenterPos.x += m_VelocitySubWeapon.x * elapsedSec;

		m_VelocitySubWeapon.y += m_Acceleration.y * elapsedSec;

		m_SubWeaponCenterPos.y += m_VelocitySubWeapon.y * elapsedSec;
		if (m_IsSpecial == false)
		{
			m_SubWeaponDrawPos.x = m_SubWeaponCenterPos.x - m_pSpriteBigAxe->GetFrameWidth() / 2.f;
			m_SubWeaponDrawPos.y = m_SubWeaponCenterPos.y - m_pSpriteBigAxe->GetFrameHeight() / 2.f;
			m_pSpriteBigAxe->Update(elapsedSec);
			m_pSpriteBigAxe->SetDrawPos(m_SubWeaponDrawPos);

			m_SubWeaponHitBox.left = m_SubWeaponDrawPos.x;
			m_SubWeaponHitBox.bottom = m_SubWeaponDrawPos.y;
			m_SubWeaponHitBox.width = m_pSpriteBigAxe->GetFrameWidth();
			m_SubWeaponHitBox.height = m_pSpriteBigAxe->GetFrameHeight();
		}

		if (m_SubWeaponCenterPos.y < 0.f - m_pSpriteBigAxe->GetFrameHeight())
		{
			m_SubWeaponIsActive = false;
			m_VelocitySubWeapon = Vector2f{ 0.f, 0.f };
			m_SubWeaponHitBox = Rectf{ -50.f, -50.f, 0.f, 0.f }; // place out of bounds

			// reset hitbox
		}
		break;
	case Player::CurrentSubWeapon::boomerang:
		m_SubWeaponCenterPos.x += cosf(m_SubWeaponAngle) * m_VelocitySubWeapon.x;

		//m_VelocitySubWeapon.y -= m_Acceleration.y * elapsedSec;
		if (m_VelocitySubWeapon.x <= 0.f)
		{
			m_VelocitySubWeapon.x -= (elapsedSec * 4.f);
		}
		else
		{
			m_VelocitySubWeapon.x += (elapsedSec * 4.f);
		}
		//m_VelocitySubWeapon.x += elapsedSec;

		m_SubWeaponCenterPos.y += m_VelocitySubWeapon.y * elapsedSec;
		if (m_IsSpecial == false)
		{
			m_SubWeaponDrawPos.x = m_SubWeaponCenterPos.x - m_pSpriteBoomerang->GetFrameWidth() / 2.f;
			m_SubWeaponDrawPos.y = m_SubWeaponCenterPos.y - m_pSpriteBoomerang->GetFrameHeight() / 2.f;
			m_pSpriteBoomerang->Update(elapsedSec);
			m_pSpriteBoomerang->SetDrawPos(m_SubWeaponDrawPos);

			m_SubWeaponHitBox.left = m_SubWeaponDrawPos.x;
			m_SubWeaponHitBox.bottom = m_SubWeaponDrawPos.y;
			m_SubWeaponHitBox.width = m_pSpriteBoomerang->GetFrameWidth();
			m_SubWeaponHitBox.height = m_pSpriteBoomerang->GetFrameHeight();
		}
		if (utils::GetDistance(Point2f{ m_SubWeaponCenterPos.x, 0.f }, Point2f{ GetPlayerPos().x, 0.f }) > 220.f)
		{
			m_SubWeaponIsActive = false;
			m_VelocitySubWeapon = Vector2f{ 0.f, 0.f };
			m_SubWeaponHitBox = Rectf{ -50.f, -50.f, 0.f, 0.f }; // place out of bounds

			// reset hitbox
		}
		break;
	case Player::CurrentSubWeapon::sacredWater:
		break;
	}

	
}
	


void Player::InputHandling(float elapsedSec)
{
	//Check keyboard state
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);



	if (m_IsOnGround == false && m_IsOnStairs == false && !(m_CurrentState == ActionState::damaged))
	{
		m_CurrentState = ActionState::jumping;
		if (pStates[SDL_SCANCODE_RIGHT])
		{
			m_IsLookingRight = true;
		}
		if (pStates[SDL_SCANCODE_LEFT])
		{
			m_IsLookingRight = false;
		}
		if (pStates[SDL_SCANCODE_A])
		{
			m_CurrentState = ActionState::attacking;
			PlayWhipSound();
		}
	}
	else if (m_IsCrouched == true)
	{
		m_Velocity.x = 0.f;
		if (pStates[SDL_SCANCODE_RIGHT])
		{
			m_IsLookingRight = true;
		}
		if (pStates[SDL_SCANCODE_LEFT])
		{
			m_IsLookingRight = false;
		}

		if (!pStates[SDL_SCANCODE_DOWN])
		{
			m_IsCrouched = false;
		}
		if (pStates[SDL_SCANCODE_A])
		{
			m_CurrentState = ActionState::attacking;
			PlayWhipSound();

		}
		// add attacking state when crouched
	}
	else if (m_IsOnStairs == true)
	{
		//level->CheckForStairDirection(m_PlayerHitBox, m_Velocity)
		if (m_CurrentStairDirection == true) // true is right going up
		{
			if (pStates[SDL_SCANCODE_RIGHT] && !(pStates[SDL_SCANCODE_UP]))
			{
				m_IsGoingUpStairs = true;
				m_pSpriteGoingUpStairs->Update(elapsedSec);
				m_IsLookingRight = true;
				m_PlayerHitBox.left += 1.f;
				m_PlayerHitBox.bottom += 1.f;


				// going right
			}
			if (pStates[SDL_SCANCODE_LEFT] && !(pStates[SDL_SCANCODE_DOWN]))
			{
				m_IsGoingUpStairs = false;
				m_pSpriteGoingDownStairs->Update(elapsedSec);
				m_IsLookingRight = false;
				m_PlayerHitBox.left -= 1.f;
				m_PlayerHitBox.bottom -= 1.f;
				// going left
			}
			if (pStates[SDL_SCANCODE_DOWN] && !(pStates[SDL_SCANCODE_LEFT]))
			{
				m_IsLookingRight = false;
				m_pSpriteGoingDownStairs->Update(elapsedSec);
				m_IsGoingUpStairs = false;
				m_PlayerHitBox.left -= 1.f;
				m_PlayerHitBox.bottom -= 1.f;
				// goes down the stairs
				// get from level wich way the stairs are headed, if stairs are going up to right
				// pressing down will make player look left while going down the stairs
			}
			if (pStates[SDL_SCANCODE_UP] && !(pStates[SDL_SCANCODE_RIGHT]))
			{
				m_IsGoingUpStairs = true;
				m_pSpriteGoingUpStairs->Update(elapsedSec);
				m_IsLookingRight = true;
				m_PlayerHitBox.left += 1.f;
				m_PlayerHitBox.bottom += 1.f;
				// goes up the stairs
				// ditto as DOWN but for up
			}
		}
		else // false, meaning we go up to the left
		{
			if (pStates[SDL_SCANCODE_RIGHT] && !(pStates[SDL_SCANCODE_DOWN]))
			{
				m_IsGoingUpStairs = false;
				m_pSpriteGoingDownStairs->Update(elapsedSec);
				m_IsLookingRight = true;
				m_PlayerHitBox.left += 1.f;
				m_PlayerHitBox.bottom -= 1.f;


				// going right
			}
			if (pStates[SDL_SCANCODE_LEFT] && !(pStates[SDL_SCANCODE_UP]))
			{
				m_IsGoingUpStairs = true;
				m_pSpriteGoingUpStairs->Update(elapsedSec);
				m_IsLookingRight = false;
				m_PlayerHitBox.left -= 1.f;
				m_PlayerHitBox.bottom += 1.f;
				// going left
			}
			if (pStates[SDL_SCANCODE_DOWN] && !(pStates[SDL_SCANCODE_RIGHT]))
			{

				m_IsGoingUpStairs = false;
				m_pSpriteGoingDownStairs->Update(elapsedSec);
				m_IsLookingRight = true;
				m_PlayerHitBox.left += 1.f;
				m_PlayerHitBox.bottom -= 1.f;
				// goes down the stairs
				// get from level wich way the stairs are headed, if stairs are going up to right
				// pressing down will make player look left while going down the stairs
			}
			if (pStates[SDL_SCANCODE_UP] && !(pStates[SDL_SCANCODE_LEFT]))
			{
				m_IsGoingUpStairs = true;
				m_pSpriteGoingUpStairs->Update(elapsedSec);
				m_IsLookingRight = false;
				m_PlayerHitBox.left -= 1.f;
				m_PlayerHitBox.bottom += 1.f;
				// goes up the stairs
				// ditto as DOWN but for up
			}
		}
		


		if (pStates[SDL_SCANCODE_S])
		{
			m_IsOnStairs = false;
			m_Velocity.y = m_JumpSpeed;
			m_CurrentState = ActionState::jumping;
		}
		if (pStates[SDL_SCANCODE_S] && pStates[SDL_SCANCODE_RIGHT])
		{
			m_IsOnStairs = false;
			m_Velocity.y = m_JumpSpeed;
			m_Velocity.x = m_HorSpeed;
			m_CurrentState = ActionState::jumping;
		}
		if (pStates[SDL_SCANCODE_S] && pStates[SDL_SCANCODE_LEFT])
		{
			m_IsOnStairs = false;
			m_Velocity.y = m_JumpSpeed;
			m_Velocity.x = -m_HorSpeed;
			m_CurrentState = ActionState::jumping;
		}

		if (pStates[SDL_SCANCODE_A])
		{
			m_CurrentState = ActionState::attacking;
			PlayWhipSound();

		}

		if (!(m_CurrentState == ActionState::jumping))
		{
			//level->CheckForStairs(m_PlayerHitBox, m_Velocity)
			if ( m_AreThereStairs == true)
			{
				m_IsOnStairs = true; // we are still on the stairs...
			}
			else
			{
				m_IsOnStairs = false; // we walked off the stairs...

			}
		}
		
	}
	
	else if (!(m_CurrentState == ActionState::damaged))
	{

		if (pStates[SDL_SCANCODE_A])
		{
			m_CurrentState = ActionState::attacking;
			PlayWhipSound();
		}
		if (pStates[SDL_SCANCODE_A] && pStates[SDL_SCANCODE_RIGHT])
		{
			m_IsLookingRight = true;
			m_CurrentState = ActionState::attacking;
			PlayWhipSound();
		}
		if (pStates[SDL_SCANCODE_A] && pStates[SDL_SCANCODE_LEFT])
		{
			m_IsLookingRight = false;
			m_CurrentState = ActionState::attacking;
			PlayWhipSound();
		}

		if (pStates[SDL_SCANCODE_RIGHT] && !(m_CurrentState == ActionState::attacking))
		{
			m_IsLookingRight = true;
			m_CurrentState = ActionState::moving;
			m_Velocity.x = m_HorSpeed;
		}
		if (pStates[SDL_SCANCODE_LEFT] && !(m_CurrentState == ActionState::attacking))
		{
			m_IsLookingRight = false;
			m_CurrentState = ActionState::moving;
			m_Velocity.x = -m_HorSpeed;
		}


		if (!pStates[SDL_SCANCODE_RIGHT] && !pStates[SDL_SCANCODE_LEFT] && !(m_CurrentState == ActionState::attacking))
		{
			m_Velocity.x = 0.f;
			m_CurrentState = ActionState::idle;
		}


		if (pStates[SDL_SCANCODE_S])
		{

			m_Velocity.y = m_JumpSpeed;
			m_CurrentState = ActionState::jumping;

		}
		
	}


	// we do this check not in the "else if(!(m_CurrentState == ActionState::damaged))"
	// otherwise we need to check the input the same way for the first "if(m_IsOnGround == false && m_IsOnStairs == false && !(m_CurrentState == ActionState::damaged))"
	// would rather have to write this once
	if (!(m_CurrentState == ActionState::damaged))
	{
		if (pStates[SDL_SCANCODE_UP])
		{

			//level -> check for stairs
			if (m_AreThereStairs == true)
			{
				m_IsOnStairs = true;
				// reset current velocity, otherwise will speed off
				//m_Velocity.x = 0.f;
				//m_Velocity.y = 0.f;
				m_CurrentState = ActionState::moving;
			}
			else
			{
				m_IsOnStairs = false;
				// combo with special weapon attack for differrent moveset
			}


		}
		if (pStates[SDL_SCANCODE_DOWN] && !(m_CurrentState == ActionState::jumping))
		{
			if (m_AreThereStairs == true)
			{
				m_IsOnStairs = true;
				//m_Velocity.x = 0.f;
				//m_Velocity.y = 0.f;
				m_CurrentState = ActionState::moving;
			}
			else
			{
				m_IsOnStairs = false;
				if (m_IsOnGround == true)
				{

					m_IsCrouched = true;
				}

			}
		}
		if (pStates[SDL_SCANCODE_D] && !(pStates[SDL_SCANCODE_UP]) && m_SubWeaponIsActive == false && m_LastUsedSubWeapon != CurrentSubWeapon::none) // && m_AmountOfGems >= 1
		{
			if (m_AmountOfGems > 0)
			{
				m_SubWeaponIsActive = true;
				m_SubWeaponCenterPos = GetPlayerPos();
				m_SubWeaponDirection = m_IsLookingRight;
				switch (m_LastUsedSubWeapon)
				{
				case Player::CurrentSubWeapon::battleAxe:
					m_pSpriteBigAxe->SetDrawPos(m_SubWeaponCenterPos);
					m_VelocitySubWeapon.y += m_ThrowDirection.y;
					if (m_IsLookingRight == true)
					{
						m_VelocitySubWeapon.x += m_ThrowDirection.x + m_Velocity.x;
					}
					else
					{
						m_VelocitySubWeapon.x += -(m_ThrowDirection.x) + m_Velocity.x;

					}
					break;
				case Player::CurrentSubWeapon::boomerang:
					m_pSpriteBigAxe->SetDrawPos(m_SubWeaponCenterPos);
					m_VelocitySubWeapon.y += m_ThrowDirection.y/10.f;
					if (m_IsLookingRight == true)
					{
						m_VelocitySubWeapon.x = 7.f;
					}
					else
					{
						m_VelocitySubWeapon.x = -7.f;

					}
					break;
				}
				
				
				m_AmountOfGems -= 1;
			}
			
			// give the weapon the direction of the player
			// set the weapon to the mid point of the player
			// set weapon to active so it draws, then let it update based on type
			// reduce amount of gems -> currently not happening
		}
	}
	


}

void Player::DrawPlayer() const
{
	if (m_IsOnGround == true)
	{
		if (m_Velocity.x < 0.f || m_IsLookingRight == false)
		{
			glPushMatrix();
				glTranslatef(m_PlayerHitBox.left + m_PlayerHitBox.width / 2, m_PlayerHitBox.bottom, 0.f);
				glScalef(-1.f, 1.f, 1.f);
				glTranslatef(-m_PlayerHitBox.left - m_PlayerHitBox.width / 2, -m_PlayerHitBox.bottom, 0.f);

				DrawPlayerState();

			glPopMatrix();
		}
		else
		{
			DrawPlayerState();
		}
	}
	else
	{
		if (m_IsLookingRight == false)
		{
			glPushMatrix();
				glTranslatef(m_PlayerHitBox.left + m_PlayerHitBox.width / 2, m_PlayerHitBox.bottom, 0.f);
				glScalef(-1.f, 1.f, 1.f);
				glTranslatef(-m_PlayerHitBox.left - m_PlayerHitBox.width / 2, -m_PlayerHitBox.bottom, 0.f);

				DrawPlayerState();
			
			glPopMatrix();
		}
		else
		{
			DrawPlayerState();
		}
	}


	// hitboxes test
	
	//{
	//	// test hitbox
	//	utils::SetColor(Color4f{ 1.f, 1.f, 0.f, 1.f });
	//	utils::DrawRect(m_PlayerHitBox, 2.f);
	//	// test attackhitbox
	//	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	//	utils::DrawRect(m_PlayerAttackHitBox, 2.f);
	//}
	
}

void Player::DrawPlayerState() const
{
	if (m_IsCrouched == false)
	{
		switch (m_CurrentState)
		{
		case Player::ActionState::idle:
			m_pSpriteIdle->Draw();
			break;
		case Player::ActionState::moving:
			if (m_IsOnStairs == true)
			{
				if (m_IsGoingUpStairs == true)
				{
					m_pSpriteGoingUpStairs->Draw();
				}
				else
				{
					m_pSpriteGoingDownStairs->Draw();
				}
			}
			else
			{
				m_pSpriteMoving->Draw();
			}
			break;
		case Player::ActionState::damaged:
			m_pSpriteDamaged->Draw();
			break;
		case Player::ActionState::jumping:
			m_pSpriteJumping->Draw();
			break;
		case Player::ActionState::attacking:
			m_pAttackingSprites[m_Index]->Draw();
			
			break;
		}
	}
	else if(m_IsCrouched == true && m_CurrentState == ActionState::attacking)
	{
		m_pAttackingSprites[m_Index]->Draw();
	}
	else
	{
		m_pSpriteCrouching->Draw();
	}

	


}



void Player::UpdateAttackState(float elapsedSec)
{
	if (m_IsCrouched == false)
	{
		m_Index = m_CurrentLevel - 1;
	}
	else
	{
		m_Index = 3 + m_CurrentLevel;
	}

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	m_pAttackingSprites[m_Index]->Update(elapsedSec);

	// checking for last frame +1, so last frame can still be drawn
	if (m_pAttackingSprites[m_Index]->GetCurrentFrame() == m_pAttackingSprites[m_Index]->GetTotalAmountOfFrames()) 
	{
		if ((pStates[SDL_SCANCODE_A]))
		{
			if (m_AttackAccumulatedTime < m_LongerAttackTimer)
			{
				m_pAttackingSprites[m_Index]->SetCurrentFrame(m_pAttackingSprites[m_Index]->GetTotalAmountOfFrames() - 1);
				SetAttackHitBox();
			}
			else
			{
				m_AttackAccumulatedTime = 0.f;
				m_HasControl = true;
				m_pAttackingSprites[m_Index]->SetCurrentFrame(0);
				if (m_IsOnStairs == true)
				{
					m_CurrentState = ActionState::moving;

				}
				else
				{
					m_CurrentState = ActionState::idle;

				}
			}
		}
		else
		{
			if (m_AttackAccumulatedTime < m_BaseAttackTimer)
			{
				m_pAttackingSprites[m_Index]->SetCurrentFrame(m_pAttackingSprites[m_Index]->GetTotalAmountOfFrames() - 1);
				SetAttackHitBox();
			}
			else
			{
				m_AttackAccumulatedTime = 0.f;
				m_HasControl = true;
				m_pAttackingSprites[m_Index]->SetCurrentFrame(0);
				if (m_IsOnStairs == true)
				{
					m_CurrentState = ActionState::moving;

				}
				else
				{
					m_CurrentState = ActionState::idle;

				}
			}
		}
	}
}

void Player::SetAttackHitBox()
{
	if (m_CurrentLevel == 1)
	{
		m_PlayerAttackHitBox.width = m_pSpriteIdle->GetFrameWidth() * 1.5f;
		m_PlayerAttackHitBox.height = m_pSpriteIdle->GetFrameHeight() / 4;

		if (m_IsCrouched == false)
		{
			m_PlayerAttackHitBox.bottom = m_DrawPos.y + m_PlayerHitBox.height / 1.5f;
			if (m_IsLookingRight == true)
			{
				m_PlayerAttackHitBox.left = m_DrawPos.x + m_PlayerHitBox.width / 2.f + 2.f;
			}
			else
			{
				m_PlayerAttackHitBox.left = (m_DrawPos.x + m_PlayerHitBox.width / 2.f - 2.f) - m_PlayerAttackHitBox.width;
			}
		}
		else
		{
			m_PlayerAttackHitBox.bottom = m_DrawPos.y + m_PlayerHitBox.height / 3.f;
			if (m_IsLookingRight == true)
			{
				m_PlayerAttackHitBox.left = m_DrawPos.x + m_PlayerHitBox.width / 2.f + 6.f;
			}
			else
			{
				m_PlayerAttackHitBox.left = (m_DrawPos.x + m_PlayerHitBox.width / 2.f - 6.f) - m_PlayerAttackHitBox.width;
			}
		}
	}
	else if (m_CurrentLevel == 2)
	{
		m_PlayerAttackHitBox.width = m_pSpriteIdle->GetFrameWidth() * 1.5f;
		m_PlayerAttackHitBox.height = m_pSpriteIdle->GetFrameHeight() / 4;

		if (m_IsCrouched == false)
		{
			m_PlayerAttackHitBox.bottom = m_DrawPos.y + m_PlayerHitBox.height / 1.5f;
			if (m_IsLookingRight == true)
			{
				m_PlayerAttackHitBox.left = m_DrawPos.x + m_PlayerHitBox.width / 2.f + 3.f;
			}
			else
			{
				m_PlayerAttackHitBox.left = (m_DrawPos.x + m_PlayerHitBox.width / 2.f - 3.f) - m_PlayerAttackHitBox.width;
			}
		}
		else
		{
			m_PlayerAttackHitBox.bottom = m_DrawPos.y + m_PlayerHitBox.height / 3.f;
			if (m_IsLookingRight == true)
			{
				m_PlayerAttackHitBox.left = m_DrawPos.x + m_PlayerHitBox.width / 2.f + 7.f;
			}
			else
			{
				m_PlayerAttackHitBox.left = (m_DrawPos.x + m_PlayerHitBox.width / 2.f - 7.f) - m_PlayerAttackHitBox.width;
			}
		}
	}
	else if (m_CurrentLevel == 3)
	{
		m_PlayerAttackHitBox.width = m_pSpriteIdle->GetFrameWidth() * 2.f;
		m_PlayerAttackHitBox.height = m_pSpriteIdle->GetFrameHeight() / 4;

		if (m_IsCrouched == false)
		{
			m_PlayerAttackHitBox.bottom = m_DrawPos.y + m_PlayerHitBox.height / 1.5f;
			if (m_IsLookingRight == true)
			{
				m_PlayerAttackHitBox.left = m_DrawPos.x + m_PlayerHitBox.width / 2.f + 2.f;
			}
			else
			{
				m_PlayerAttackHitBox.left = (m_DrawPos.x + m_PlayerHitBox.width / 2.f - 2.f) - m_PlayerAttackHitBox.width;
			}
		}
		else
		{
			m_PlayerAttackHitBox.bottom = m_DrawPos.y + m_PlayerHitBox.height / 3.f;
			if (m_IsLookingRight == true)
			{
				m_PlayerAttackHitBox.left = m_DrawPos.x + m_PlayerHitBox.width / 2.f + 6.f;
			}
			else
			{
				m_PlayerAttackHitBox.left = (m_DrawPos.x + m_PlayerHitBox.width / 2.f - 6.f) - m_PlayerAttackHitBox.width;
			}
		}
	}
	else if (m_CurrentLevel == 4)
	{
		m_PlayerAttackHitBox.width = m_pSpriteIdle->GetFrameWidth() * 2.45f;
		m_PlayerAttackHitBox.height = m_pSpriteIdle->GetFrameHeight() / 4 + 5.f;

		if (m_IsCrouched == false)
		{
			m_PlayerAttackHitBox.bottom = m_DrawPos.y - 2.f + m_PlayerHitBox.height / 1.5f;
			if (m_IsLookingRight == true)
			{
				m_PlayerAttackHitBox.left = m_DrawPos.x + m_PlayerHitBox.width / 2.f + 2.f;
			}
			else
			{
				m_PlayerAttackHitBox.left = (m_DrawPos.x + m_PlayerHitBox.width / 2.f - 2.f) - m_PlayerAttackHitBox.width;
			}
		}
		else
		{
			m_PlayerAttackHitBox.bottom = m_DrawPos.y + m_PlayerHitBox.height / 3.f;
			if (m_IsLookingRight == true)
			{
				m_PlayerAttackHitBox.left = m_DrawPos.x + m_PlayerHitBox.width / 2.f + 4.f;
			}
			else
			{
				m_PlayerAttackHitBox.left = (m_DrawPos.x + m_PlayerHitBox.width / 2.f - 4.f) - m_PlayerAttackHitBox.width;
			}
		}
	}
}

void Player::PlayWhipSound()
{
	if (m_CurrentLevel == 1)
	{
		m_pWhipSoundEffect1->Play(0);
	}
	else if(m_CurrentLevel < 4 && m_CurrentLevel > 1)
	{
		m_pWhipSoundEffect2->Play(0);
	}
	else
	{
		m_pWhipSoundEffect3->Play(0);
	}
}
