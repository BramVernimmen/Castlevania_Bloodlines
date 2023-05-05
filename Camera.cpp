#include "pch.h"
#include "Camera.h"
#include "utils.h"

Camera::Camera(float width, float height, float scale)
	: m_Width{ width }
	, m_Height{ height }
	, m_LevelBoundaries{ Rectf{0.f, 0.f, m_Width, m_Height} }
	, m_CurrentCamPos{}
	, m_Scale{ scale }
{

}


void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}


void Camera::TransformMid() const
{
	//m_CurrentCamPos = Track(target);
	//Point2f cameraPos{ Track(target) };
	//Clamp(cameraPos);

	glScalef(m_Scale, m_Scale, 1.f);
	glTranslatef(-m_CurrentCamPos.x, -m_CurrentCamPos.y, 0.f);
	//utils::DrawRect(drawPos, m_Width, m_Height, 2.f);
}

void Camera::TransformParallax(const Rectf& parallaxBoundries, bool canYaxisParallax) const
{
	Point2f parallaxPos{};
	parallaxPos.x = m_CurrentCamPos.x * (parallaxBoundries.width - m_Width) / (m_LevelBoundaries.width - m_Width);
	if (canYaxisParallax == true)
	{
		parallaxPos.y = m_CurrentCamPos.y * (parallaxBoundries.height - m_Height) / (m_LevelBoundaries.height - m_Height);
	}
	else
	{
		parallaxPos.y = m_CurrentCamPos.y;
	}

	glScalef(m_Scale, m_Scale, 1.f);
	glTranslatef(-parallaxPos.x, -parallaxPos.y, 0.f);
}

void Camera::Update(const Rectf& target)
{
	m_CurrentCamPos = Track(target);
	Clamp(m_CurrentCamPos);
}

void Camera::SnapCamToPlayer(const Rectf& target)
{
	m_CurrentCamPos.x = (target.left + target.width / 2) - m_Width / 2;
	m_CurrentCamPos.y = (target.bottom + target.height / 2) - m_Height / 2;
	Clamp(m_CurrentCamPos);
}


Point2f Camera::Track(const Rectf& target)
{
	//Point2f newPoint{};
	//
	//newPoint.x = (target.left + target.width / 2) - m_Width / 2;
	//newPoint.y = (target.bottom + target.height / 2) - m_Height / 2;
	//
	//
	//return newPoint;
	// 

	Point2f middleOfCam{ m_CurrentCamPos.x + m_Width / 2.f, m_CurrentCamPos.y + m_Height / 2.f };
	Point2f targetMiddlePoint{ target.left + target.width / 2 , target.bottom + target.height / 2 };

	if (utils::GetDistance(Point2f{ middleOfCam.x, 0.f }, Point2f{ targetMiddlePoint.x, 0.f }) > 5.f)
	{
		float camSpeed{ 2.5f };
		if (middleOfCam.x < targetMiddlePoint.x)
		{
			//++m_CurrentCamPos.x;
			m_CurrentCamPos.x += camSpeed;
		}
		else if (middleOfCam.x > targetMiddlePoint.x)
		{
			//--m_CurrentCamPos.x;
			m_CurrentCamPos.x -= camSpeed;
		}

		//if (middleOfCam.y < targetMiddlePoint.y)
		//{
		//	//++m_CurrentCamPos.y;
		//	m_CurrentCamPos.y += camSpeed;
		//
		//}
		//else if (middleOfCam.y > targetMiddlePoint.y)
		//{
		//	//--m_CurrentCamPos.y;
		//	m_CurrentCamPos.y -= camSpeed;
		//
		//}
	}
	else
	{
		m_CurrentCamPos.x = targetMiddlePoint.x - m_Width / 2;
		m_CurrentCamPos.y = targetMiddlePoint.y - m_Height / 2;

	}
	// if middle of cam distance > 5 px, smooth transition...
	
	
	//m_CurrentCamPos.x = (target.left + target.width / 2) - m_Width / 2;
	//m_CurrentCamPos.y = (target.bottom + target.height / 2) - m_Height / 2;


	return m_CurrentCamPos;
}


void Camera::Clamp(Point2f& bottomLeftPos)
{
	if (bottomLeftPos.x < 0)
	{
		bottomLeftPos.x = 0;
	}
	if (bottomLeftPos.y < 0)
	{
		bottomLeftPos.y = 0;
	}
	if (bottomLeftPos.x > m_LevelBoundaries.width - m_Width)
	{
		bottomLeftPos.x = m_LevelBoundaries.width - m_Width;
	}
	if (bottomLeftPos.y > m_LevelBoundaries.height - m_Height)
	{
		bottomLeftPos.y = m_LevelBoundaries.height - m_Height;
	}
}

