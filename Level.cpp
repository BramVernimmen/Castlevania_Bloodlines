#include "pch.h"
#include "Level.h"
//#include "Sprite.h"
#include "utils.h"
#include "SVGParser.h"
#include "Object.h"
#include "Player.h"

Level::Level(std::vector<Sprite*> levelSprites,  std::vector<std::string> svgPaths, std::vector<std::string> svgStairPaths, std::vector<bool> stairsDirections
			, Transition transitions, int stageNumber)
	: m_Sprites{ levelSprites }
	, m_Transitions{transitions}
	, m_IsLevelInTransition{false}
	, m_TransitionIndex{0}
	, m_StageNumber{stageNumber}
{
	std::vector<std::vector<Point2f>> vertices;

	for (size_t i{0}; i < svgPaths.size(); ++i)
	{
		//vertices.clear();

		SVGParser::GetVerticesFromSvgFile(svgPaths[i], vertices);
		// it is possible to store multiple paths in 1 SVG
		// we can use this to store all platforms in 1 SVG per level, all stairs going right and left aswell...
		if (vertices.size() >= m_Vertices.size())
		{
			int originalSize{ (int)m_Vertices.size() };
			for (size_t j{0} ; j < vertices.size() - originalSize; ++j) // using this for when multiple paths have been added to a SVG
			{
				m_Vertices.push_back(vertices[m_Vertices.size()]); // using m_Vertices.size() as index is the same as using originalSize + j
			}
		}
		// adding multiple SVG files flips the y-axis of the previous index for some reason
		// decided to make another vector to just store the right values in, and use that one for calculations
	}
	vertices.clear();
	for (size_t i{ 0 }; i < svgStairPaths.size(); ++i)
	{
		SVGParser::GetVerticesFromSvgFile(svgStairPaths[i], vertices);
		if (vertices.size() >= m_VerticesStairs.size())
		{
			int originalSize{ (int)m_VerticesStairs.size() };
			for (size_t j{0}; j < vertices.size() - originalSize; ++j)
			{
				m_VerticesStairs.push_back(vertices[m_VerticesStairs.size()]);
				m_StairsDirections.push_back(stairsDirections[i]);

				//m_VerticesStairs.push_back(vertices[i]);
			}
		}

		// same stuff as before, but for stairs
	}
}

Level::~Level()
{
	// the sprites have been moved here, so need to be deleted here aswell
	for (Sprite* ptr: m_Sprites)
	{
		delete ptr;
		ptr = nullptr;
	}
}




void Level::DrawLayers(Sprite::SpriteType typeOfLayer) const
{
	for (size_t spriteIndex{0}; spriteIndex < m_Sprites.size(); ++spriteIndex)
	{
		if (m_Sprites[spriteIndex]->GetSpriteType() == typeOfLayer)
		{
			m_Sprites[spriteIndex]->Draw();
		}
	}
	
	switch (typeOfLayer)
	{
	case Sprite::SpriteType::BackLayer:
		for (size_t spriteIndex{ 0 }; spriteIndex < m_Sprites.size(); ++spriteIndex)
		{
			if (m_Sprites[spriteIndex]->GetSpriteType() == Sprite::SpriteType::Cloud)
			{
				m_Sprites[spriteIndex]->Draw();
			}
		}
		break;
	}
}

Rectf Level::GetLayerBoundaries(Sprite::SpriteType typeOfLayer) const
{
	Rectf boundaries{ 0.f, 0.f, 0.f, 0.f };
	for (size_t spriteIndex{0}; spriteIndex < m_Sprites.size(); ++spriteIndex)
	{
		if (m_Sprites[spriteIndex]->GetSpriteType() == typeOfLayer)
		{
			boundaries.width = m_Sprites[spriteIndex]->GetFrameWidth();
			boundaries.height = m_Sprites[spriteIndex]->GetFrameHeight();
			return boundaries;
		}
	}

	return boundaries;
}

void Level::Update(float elapsedSec, Player* pPlayer)
{
	//update player
	pPlayer->CheckForStairs(CheckForStairs(pPlayer->GetPlayerHitBox(), pPlayer->GetPlayerVelocity(), pPlayer->IsPlayerOnStairs()));
	if (pPlayer->IsPlayerOnStairs() == false)
	{
		pPlayer->SetGrounded(IsOnGround(pPlayer->GetPlayerHitBox(), pPlayer->GetPlayerVelocity()));
		HandleCollision(pPlayer->GetPlayerHitBox(), pPlayer->GetPlayerVelocity());

	}
	else
	{
		pPlayer->SetStairDirection(CheckForStairDirection(pPlayer->GetPlayerHitBox(), pPlayer->GetPlayerVelocity()));
	}

	for (Sprite* ptr : m_Sprites)
	{
		ptr->Update(elapsedSec);
	}

	// use for transitions
	for (size_t i{0}; i < m_Transitions.transitionTriggerPositions.size() ; ++i)
	{
		if (utils::GetDistance(pPlayer->GetPlayerPos(), m_Transitions.transitionTriggerPositions[i]) < 20.f) // 20.f = triggerdistance
		{
			m_TransitionIndex = int(i);
			m_IsLevelInTransition = true;
		}
	}
}


bool Level::IsLevelInTransition() const
{
	return m_IsLevelInTransition;
}

void Level::ResetTransition()
{
	if (m_IsLevelInTransition == true)
	{
		m_IsLevelInTransition = false;
	}
}

Level::TransitionType Level::GetTransitionType() const
{
	return m_Transitions.transitionTypes[m_TransitionIndex];
}

Point2f Level::GetTransitionNewPosition() const
{
	return m_Transitions.newPosition[m_TransitionIndex];
}

int Level::GetTransitionNextStage() const
{
	return m_Transitions.nextStageIndex[m_TransitionIndex];
}

int Level::GetStageNumber()
{
	return m_StageNumber;
}



void Level::HandleCollision(Rectf& actorHitBox, Vector2f& actorVelocity) const
{
	// vertical collision
	Point2f p1{ actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom - 1.f};
	Point2f p2{ actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom + actorHitBox.height };
	utils::HitInfo hitInfo{};

	if (utils::Raycast(m_Vertices[0], p1, p2, hitInfo) == true)
	{
		actorVelocity.y = 0.f;
		actorHitBox.bottom = hitInfo.intersectPoint.y;
	}

	// check for horizontal collision
	// we only want to check horizontal collision when the level isn't in a trasition
	// when in a transition, we want the player to be able to move out of bounds

	if (m_IsLevelInTransition == false)
	{
		Point2f p3{ actorHitBox.left + 1.f , actorHitBox.bottom + actorHitBox.height / 2 };
		Point2f p4{ actorHitBox.left + actorHitBox.width - 1.f, actorHitBox.bottom + actorHitBox.height / 2 };
		utils::HitInfo hitInfo2{};

		if (utils::Raycast(m_Vertices[0], p3, p4, hitInfo2) == true)
		{
			if (actorVelocity.x > 0.f) // player is going right
			{
				actorHitBox.left = hitInfo2.intersectPoint.x - actorHitBox.width;

			}
			else // player is going left
			{
				actorHitBox.left = hitInfo2.intersectPoint.x;

			}
			actorVelocity.x = 0.f;

		}
	}
	

	if (actorVelocity.y < 0)
	{
		IsOnGround(actorHitBox, actorVelocity);
		
	}
}


bool Level::IsOnGround( Rectf& actorHitBox,  Vector2f& actorVelocity) const
{

	Point2f p1{ actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom - 1.f };
	Point2f p2{ actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom + 15.f}; //actorHitBox.height  10.f
	utils::HitInfo hitInfo{};
	if (utils::Raycast(m_Vertices[0], p1, p2, hitInfo) == true)
	{
		return true;
	}
	else
	{
		if (m_Vertices.size() > 0)
		{
			// check for onGround on any platforms 
			for (size_t i{1}; i < m_Vertices.size(); ++i)
			{
				if (utils::Raycast(m_Vertices[i], p1, p2, hitInfo) == true && actorVelocity.y <= 0.f)
				{
					actorVelocity.y = 0.f;
					actorHitBox.bottom = hitInfo.intersectPoint.y;
					return true;
				}
			}
		}
		else
		{
			return false;
		}
		return false;
	}
}



bool Level::CheckForStairs(Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerOnStairs) const
{
	if (m_VerticesStairs.size() > 0)
	{
		Point2f p1{ actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom - 5.f};
		Point2f p2{ actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom + 5.f }; //actorHitBox.height 10.f
		utils::HitInfo hitInfo{};
		// check for onGround on any platforms 
		for (size_t i{ 0 }; i < m_VerticesStairs.size(); ++i)
		{
			if (utils::Raycast(m_VerticesStairs[i], p1, p2, hitInfo) == true)
			{
				if (isPlayerOnStairs == true)
				{
					actorVelocity.x = 0.f;
					actorVelocity.y = 0.f;
					actorHitBox.bottom = hitInfo.intersectPoint.y;
				}
			
				return true;
			}
		}
	}
	else
	{
		return false;
	}
	return false;
}

bool Level::CheckForStairDirection(Rectf& actorHitBox, Vector2f& actorVelocity) const
{
	// same check as the prevous, yet now we return the index of directions vector
	Point2f p1{ actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom - 5.f };
	Point2f p2{ actorHitBox.left + actorHitBox.width / 2, actorHitBox.bottom + 10.f }; //actorHitBox.height 10.f
	utils::HitInfo hitInfo{};
	// check for onGround on any platforms 
	for (size_t i{ 0 }; i < m_VerticesStairs.size(); ++i)
	{
		if (utils::Raycast(m_VerticesStairs[i], p1, p2, hitInfo) == true)
		{
			return m_StairsDirections[i];
		}
	}
	return false;
}



Level::Transition::Transition()
	: Transition(	std::vector<Point2f>{Point2f{ 0.f,0.f }},
					std::vector<Level::TransitionType>{Level::TransitionType::fadeToBlack},
					std::vector<Point2f>{Point2f{ 0.f,0.f }},
					std::vector<int>{0})
{
}

Level::Transition::Transition(std::vector<Point2f> transitionTriggerPositions, std::vector<Level::TransitionType> transitionTypes, std::vector<Point2f> newPosition, std::vector<int> nextStageIndex)
	: transitionTriggerPositions{transitionTriggerPositions}
	, transitionTypes{transitionTypes}
	, newPosition{newPosition}
	, nextStageIndex{nextStageIndex}
{

}