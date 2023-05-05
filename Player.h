#pragma once
class Sprite;
class TextureManager;
class SoundManager;
class SoundEffect;
#include "Vector2f.h"
#include <vector>

class Player final
{
public:
	enum class ActionState
	{
		idle,
		moving,
		damaged,
		jumping,
		attacking
		// others can be added for later
	};
	enum class CurrentSubWeapon
	{
		none,
		battleAxe,
		boomerang,
		sacredWater
	};
	Player(const Point2f& drawPos, int amountOfLives, int amountOfContinues, TextureManager* pTextureManager, SoundManager* pSoundManager);
	~Player();
	Player(const Player& other) = delete;
	Player& operator=(const Player& rhs) = delete;
	Player(Player&& other) = delete;
	Player& operator=(Player&& rhs) = delete;

	void Update(float elapsedSec);
	void Draw() const;
	Rectf& GetPlayerHitBox();
	//Rectf GetPlayerAttackHitBox();
	std::vector<Rectf> GetPlayerAttackHitBox();
	Vector2f& GetPlayerVelocity();
	float GetPlayerHorSpeed() const;
	Point2f GetPlayerPos();
	Point2f GetPlayerDrawPos() const;
	float GetPlayerDamage();

	void SetPlayerState(ActionState newState);
	void SetPlayerControl(bool hasControl);

	void IsHit(float pointOfContact, float damage);

	void SetInvincible();
	void LevelUp();
	int GetPlayerLevel() const;
	void AddGems(int amount);
	void AddHealth(float amount);
	void AddScore(int amount);
	void SetSubWeapon(CurrentSubWeapon newWeapon);
	CurrentSubWeapon GetSubWeapon();

	void SetPos(const Point2f& newPos, int safeStageIndex);
	void SetGrounded(bool isOnGround);
	bool IsPlayerOnStairs() const;
	void SetStairDirection(bool rightIsUp);
	void CheckForStairs(bool areThereStairs);
	bool PlayerGetStairsDirection() const;
	bool PlayerGetFacingDirection() const;

	void UpdateStairsForTransition(float elapsedSec);

	bool MovePlayerFadeToBlack(float elapsedSec);
	void MakePlayerWait(float elapsedSec);
	void MovePlayerThroughDoor(float elapsedSec, float newPos);

	float GetMaximumHealth();
	float GetCurrentHealth();

	int GetCurrentScore();
	int GetHighScore();
	int GetAmountOfGems();
	int GetAmountOfLives();

	bool IsPlayerDead();
	
	int GetRespawnLevelIndex();
	void ResetPlayerAfterDeath(int newAmountOfLives);

	int& GetAmountOfContinues();

	void ResetScore();

private:
	void UpdatePos();
	void UpdateSubWeapon(float elapsedSec);
	void InputHandling(float elapsedSec);
	void DrawPlayer() const;
	void DrawPlayerState() const;

	void UpdateAttackState(float elapsedSec);
	void SetAttackHitBox();
	void PlayWhipSound();

	Point2f m_DrawPos;
	Point2f m_RespawnPos;
	int m_RespawnStageIndex;

	Sprite* m_pSpriteIdle;
	Sprite* m_pSpriteMoving;
	Sprite* m_pSpriteJumping;
	Sprite* m_pSpriteCrouching;
	Sprite* m_pSpriteGoingUpStairs;
	Sprite* m_pSpriteGoingDownStairs;
	Sprite* m_pSpriteDamaged;

	Sprite* m_pSpriteBigAxe;
	Sprite* m_pSpriteBoomerang;
	//Sprite* m_pSpriteSmallAxe;

	SoundEffect* m_pWhipSoundEffect1;
	SoundEffect* m_pWhipSoundEffect2;
	SoundEffect* m_pWhipSoundEffect3;

	Point2f m_SubWeaponDrawPos;
	Point2f m_SubWeaponCenterPos;
	bool m_SubWeaponDirection;
	bool m_IsSpecial;

	std::vector<Sprite*> m_pAttackingSprites;
	int m_Index;

	ActionState m_CurrentState;
	CurrentSubWeapon m_CurrentSubWeapon;
	CurrentSubWeapon m_LastUsedSubWeapon;
	bool m_SubWeaponIsActive;
	Vector2f m_VelocitySubWeapon;
	Vector2f m_ThrowDirection;
	Rectf m_SubWeaponHitBox;


	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	bool m_IsLookingRight;
	const float m_HorSpeed;
	const float m_JumpSpeed;

	Rectf m_PlayerHitBox;
	Rectf m_PlayerAttackHitBox;

	bool m_IsOnGround;
	bool m_IsOnStairs;
	bool m_IsGoingUpStairs;
	bool m_HasControl;
	bool m_IsCrouched;
	bool m_CurrentStairDirection;
	bool m_AreThereStairs;

	bool m_IsInvincible;
	float m_InvincibleTimer;
	float m_MaximumInvincibleTimer;
	float m_OriginalMaximumInvincibleTimer;

	float m_BaseAttackTimer;
	float m_LongerAttackTimer;
	float m_AttackAccumulatedTime;

	float m_DamagedTimer;
	float m_DamageMaxTimer;

	int m_CurrentLevel;

	int m_AmountOfGems;
	float m_CurrentHealth;
	float m_MaximumHealth;
	int m_CurrentScore;
	int m_HighScore;

	int m_AmountOfLives;
	int m_AmountOfContinues;

	bool m_IsPlayerDead;

	float m_SubWeaponAngle;
	float m_SubWeaponAngleIncrement;
};

