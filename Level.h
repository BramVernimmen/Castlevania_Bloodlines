#pragma once
#include <vector>
#include "Vector2f.h"
#include "Sprite.h"
class Object;
class Player;




class Level final
{
public:
	enum class TransitionType
	{
		fadeToBlack,
		door,
		none
	};
	struct Transition
	{
		Transition();
		explicit Transition(std::vector<Point2f> transitionTriggerPositions,
			std::vector<TransitionType> transitionTypes,
			std::vector<Point2f> newPosition,
			std::vector<int> nextStageIndex);


		std::vector<Point2f> transitionTriggerPositions;
		std::vector<TransitionType> transitionTypes;
		std::vector<Point2f> newPosition;
		std::vector<int> nextStageIndex;

	};

	Level(std::vector<Sprite*> levelSprites,  std::vector<std::string> svgPaths, std::vector<std::string> svgStairPaths, std::vector<bool> stairsDirections
		, Transition transitions, int stageNumber);
	Level(const Level& other) = delete;
	Level& operator=(const Level& rhs) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& rhs) = delete;
	~Level();


	void DrawLayers(Sprite::SpriteType typeOfLayer) const;

	Rectf GetLayerBoundaries(Sprite::SpriteType typeOfLayer) const;


	void HandleCollision(Rectf& actorHitBox, Vector2f& actorVelocity) const;
	bool IsOnGround( Rectf& actorHitBox,  Vector2f& actorVelocity) const;

	bool CheckForStairs(Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerOnStairs) const;
	bool CheckForStairDirection(Rectf& actorHitBox, Vector2f& actorVelocity) const;

	void Update(float elapsedSec, Player* pPlayer);

	
	bool IsLevelInTransition() const;
	void ResetTransition();


	TransitionType GetTransitionType() const;
	Point2f GetTransitionNewPosition() const;
	int GetTransitionNextStage() const;

	int GetStageNumber();

private:
	std::vector<std::vector<Point2f>> m_Vertices; // first element will have main layer, the others will have platforms (platform can be jumped through from below)
	std::vector<std::vector<Point2f>> m_VerticesStairs;
	std::vector<bool> m_StairsDirections;

	std::vector<Sprite*> m_Sprites;

	Transition m_Transitions;

	bool m_IsLevelInTransition;
	int m_TransitionIndex;
	int m_StageNumber;
};

