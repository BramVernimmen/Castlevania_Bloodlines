#pragma once
#include <vector>
#include "Sprite.h"
#include "Level.h"
#include "SoundManager.h"
class Player;
class DropManager;
class Camera;
class EnemyManager;
class BossManager;
class ObjectManager;
class TextureManager;

class LevelManager final
{
public:
	LevelManager();
	LevelManager(const LevelManager& other) = delete;
	LevelManager& operator=(const LevelManager& other) = delete;
	LevelManager(LevelManager&& other) = delete;
	LevelManager& operator=(LevelManager&& other) = delete;
	~LevelManager();


	void Update(float elapsedSec, Player* player, DropManager* pDropManager, EnemyManager* pEnemyManager, BossManager* pBossManager, ObjectManager* pObjectManager, int currentLevel, int currentStage, TextureManager* pTextureManager, SoundManager* pSoundManager);
	bool CheckTransitionTriggered(int currentLevel, int currentStage);
	Level::TransitionType GetTransitionType(int currentLevel, int currentStage);
	Point2f GetNewPlayerPosition(int currentLevel, int currentStage);
	int GetNextStageIndex(int currentLevel, int currentStage);
	void ResetTransition(int currentLevel, int currentStage);

	void DrawLevelLayers(int level, int stage, Sprite::SpriteType typeOfLayer) const;

	void DrawCurrentLevel(int currentLevel, int currentStage, Camera* camera, Player* player, DropManager* pDropManager, EnemyManager* pEnemyManager, BossManager* pBossManager, ObjectManager* pObjectManager) const;
	void DrawDoorTransition(int currentLevel, int currentStage, Camera* camera, Player* player, DropManager* pDropManager, EnemyManager* pEnemyManager, ObjectManager* pObjectManager, float camShift, int nextStage) const;

	Rectf GetBoundaries(int level, int stage, Sprite::SpriteType typeOfLayer) const;
	Level* GetCurrentLevel(int currentLevel, int currentStage);

	void StopLevelMusic(int currentLevel, int currentStage, BossManager* pBossManager);

	bool IsLevelInBattle() const;
	void InitializeLevels(TextureManager* pTextureManager, SoundManager* pSoundManager);

private:

	std::vector<std::vector<Level*>> m_pLevels;
	std::vector<SoundStream*> m_pLevelBackgroundMusic;
	bool m_IsLevelInBossBattle;

	bool m_CanPlayLevelMusic;
	bool m_HasStoppedMusic;
	

};

