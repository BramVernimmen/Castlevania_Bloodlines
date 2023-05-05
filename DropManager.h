#pragma once
#include "Drop.h"
#include <vector>
#include "structs.h"
class Sprite;
class SoundEffect;
class TextureManager;
class SoundManager;


class DropManager final
{
public:
	
	DropManager();
	~DropManager( );
	DropManager(const DropManager& other) = delete;
	DropManager& operator=(const DropManager& rhs) = delete; 
	DropManager(DropManager&& other) = delete;
	DropManager& operator=(DropManager&& rhs) = delete;


	Drop* AddItem( const Point2f &pos, Drop::DropType type, int value, int playerLevel, TextureManager* pTextureManager, SoundManager* pSoundManager);
	void Update( float elapsedSec, const Level* level);
	void Draw( ) const;

	size_t Size( ) const;
	void HitItem( const Rectf& playerHitBox, Player* player ) ;

private:
	std::vector<Drop*> m_pDropManager;
	//SoundEffect* m_pPowerUpSound;
};

