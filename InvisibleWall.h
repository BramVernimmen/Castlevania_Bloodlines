#pragma once
#include "Object.h"
#include <vector>
class InvisibleWall final : public Object
{
public:
	InvisibleWall(const Point2f& bottomPosition, const Point2f& topPosition);
	virtual ~InvisibleWall() = default;
	InvisibleWall(const InvisibleWall& other) = delete;
	InvisibleWall& operator=(const InvisibleWall& rhs) = delete;
	InvisibleWall(InvisibleWall&& other) = delete;
	InvisibleWall& operator=(InvisibleWall&& rhs) = delete;

	virtual void Draw() const override;
	virtual void Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle) override;

private:
	void HandleCollision(Rectf& actorHitBox, Vector2f& actorVelocity) const;
	std::vector<Point2f> m_WallPositions;


};


