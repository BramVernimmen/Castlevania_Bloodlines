#pragma once
#include <vector>
//#include "Object.h"
class Player;
class TextureManager;
class SoundManager;
class DropManager;
class Object;
class ObjectManager final
{
public:
	ObjectManager();
	ObjectManager(const ObjectManager& other) = delete;
	ObjectManager& operator=(const ObjectManager& other) = delete;
	ObjectManager(ObjectManager&& other) = delete;
	ObjectManager& operator=(ObjectManager&& other) = delete;
	~ObjectManager();

	void Update(float elapsedSec, int currentLevel, int currentStage, Player* pPlayer, DropManager* pDropManager, TextureManager* pTextureManager, SoundManager* pSoundManager, bool isPlayerInBattle);

	void Draw(int currentLevel, int currentStage) const;

	void InitializeObjects(TextureManager* pTextureManager, SoundManager* pSoundManager);

private:
	std::vector<std::vector<std::vector<Object*>>> m_pObjects;

};

