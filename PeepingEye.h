#pragma once
#include "Enemy.h"
//#include <vector>

class Sprite;
class TextureManager;
class PeepingEye final : public Enemy
{

public:
	PeepingEye(const Point2f& drawPos, TextureManager* pTextureManager, SoundManager* pSoundManager);
	virtual ~PeepingEye();
	PeepingEye(const PeepingEye& other) = delete;
	PeepingEye& operator=(const PeepingEye& rhs) = delete;
	PeepingEye(PeepingEye&& other) = delete;
	PeepingEye& operator=(PeepingEye&& rhs) = delete;

	virtual void Update(float elapsedSec, Point2f playerPos, float levelWidth) override;
	virtual void Draw() const override;



private:
	void UpdatePos();

	Point2f m_DrawPosEye;
	Point2f m_DrawPosChain1;
	Point2f m_DrawPosChain2;
	Point2f m_DrawPosChain3;
	bool m_IsDestroyed;




	Sprite* m_pSpriteEyeSpawning;
	Sprite* m_pSpriteEyeTracker;
	Sprite* m_pSpriteMoving;
	Sprite* m_pSpriteChain1;
	Sprite* m_pSpriteChain2;
	Sprite* m_pSpriteChain3;

	

	float m_CurrentTimer;
	float m_RandomTimerMax;
	float m_DestroyingTimer;

	float m_Angle;
	float m_AngleIncrement;

	bool m_IsLockedOnPlayer;

};

