#include "pch.h"
#include "InvisibleWall.h"
#include "utils.h"

InvisibleWall::InvisibleWall(const Point2f& bottomPosition, const Point2f& topPosition)
	: Object(bottomPosition)
{
	m_WallPositions.push_back(bottomPosition);
	m_WallPositions.push_back(topPosition);
}

void InvisibleWall::Draw() const
{
	// DEBUG ONLY
	//utils::SetColor(Color4f{ 1.f,1.f,0.f,1.f });
	//utils::DrawLine(m_WallPositions[0], m_WallPositions[1], 2.f);
}

void InvisibleWall::Update(float elapsedSec, Rectf& actorHitBox, Vector2f& actorVelocity, bool isPlayerInBattle)
{
	if (isPlayerInBattle == true)
	{
		HandleCollision(actorHitBox, actorVelocity);
	}
}

void InvisibleWall::HandleCollision(Rectf& actorHitBox, Vector2f& actorVelocity) const
{
	// horizontal collision  -> walls are vertical so only horizontal needed
	Point2f p1{ actorHitBox.left + 1.f , actorHitBox.bottom + actorHitBox.height / 2 };
	Point2f p2{ actorHitBox.left + actorHitBox.width - 1.f, actorHitBox.bottom + actorHitBox.height / 2 };
	utils::HitInfo hitInfo2{};

	if (utils::Raycast(m_WallPositions, p1, p2, hitInfo2) == true)
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



