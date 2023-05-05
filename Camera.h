#pragma once
class Camera final
{
public:
	Camera(float width, float height, float scale);
	Camera(const Camera& other) = delete;
	Camera& operator=(const Camera& rhs) = delete;
	Camera(Camera&& other) = delete;
	Camera& operator=(Camera&& rhs) = delete;
	~Camera() = default;	

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void TransformMid() const;
	void TransformParallax(const Rectf& parallaxBoundries, bool canYaxisParallax) const;
	void Update(const Rectf& target);

	void SnapCamToPlayer(const Rectf& target);

private:
	Point2f Track(const Rectf& target) ;
	void Clamp(Point2f& bottomLeftPos) ;

	const float m_Width;
	const float m_Height;
	const float m_Scale;

	Rectf m_LevelBoundaries;
	Point2f m_CurrentCamPos;

};

