#pragma once
//using namespace utils;  // using utils::Texture;
#include "Texture.h"
class TextureManager;
class Sprite final
{
public:
	enum class SpriteType
	{
		MidLayer,
		BackLayer,
		FrontLayer,
		Cloud,
		Other
	};
	Sprite(const std::string& filename, int nrCols, int nrRows, int framesPerSec, const Point2f& drawPos, SpriteType type, TextureManager* pTextureManager);
	Sprite(const std::string& filename, int nrCols, int nrRows, int framesPerSec, const Point2f& drawPos, SpriteType type, float cloudSpeed, TextureManager* pTextureManager);
	~Sprite() = default;
	Sprite(const Sprite& other) = delete;
	Sprite& operator=(const Sprite& rhs) = delete;
	Sprite(Sprite&& other) = delete;
	Sprite& operator=(Sprite&& rhs) = delete;

	void Update(float elapsedSec);
	void  Draw() const;
	float GetFrameWidth() const;
	float GetFrameHeight() const;
	SpriteType GetSpriteType() const;
	void SetDrawPos(const Point2f& newDrawPos);

	int GetCurrentFrame();
	int GetTotalAmountOfFrames();
	void SetCurrentFrame(int newFrame);
	void SetFrameRate(int newFrameRate);


private:
	Texture* m_pTexture;
	std::string m_FileName;
	const int m_Columns;
	const int m_Rows;

	int m_NrFramesPerSec;
	float m_AccumulateSec;

	const int m_TotalNrFrames;
	int m_CurrentFrame;

	float m_FrameWidth;
	float m_FrameHeight;

	Point2f m_DrawPos;

	SpriteType m_SpriteType;

	bool m_IsCloud;
	float m_CloudSpeed;
	Point2f m_SecondCloudPos;
};

