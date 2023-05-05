#include "pch.h"
#include "Sprite.h"
#include <iostream>
#include "TextureManager.h"

Sprite::Sprite(const std::string& filename, int nrCols, int nrRows, int framesPerSec, const Point2f& drawPos, SpriteType type, TextureManager* pTextureManager)
	: m_Columns{ nrCols }
	, m_Rows{ nrRows }
	, m_NrFramesPerSec{ framesPerSec }
	, m_TotalNrFrames{ nrRows * nrCols }
	//, m_pTexture{ new Texture{filename} }
	, m_FileName{filename}
	, m_DrawPos{ drawPos }
	, m_IsCloud{ false }
	, m_CloudSpeed{ 0.f }
	, m_CurrentFrame{ 0 }
	, m_AccumulateSec{ 0.f }
	, m_SpriteType{ type }
{

	m_pTexture = pTextureManager->GetTexturePointer(filename);

	m_FrameWidth = m_pTexture->GetWidth() / m_Columns;
	m_FrameHeight = m_pTexture->GetHeight() / m_Rows;

	m_SecondCloudPos.x = 0.f;
	m_SecondCloudPos.y = 0.f;
}
Sprite::Sprite(const std::string& filename, int nrCols,  int nrRows, int framesPerSec, const Point2f& drawPos, SpriteType type, float cloudSpeed, TextureManager* pTextureManager)
	: m_Columns{ nrCols }
	, m_Rows{ nrRows }
	, m_NrFramesPerSec{ framesPerSec }
	, m_TotalNrFrames{ nrRows * nrCols }
	//, m_pTexture{new Texture{filename}}
	, m_FileName{ filename }
	, m_DrawPos{drawPos}
	, m_IsCloud{true}
	, m_CloudSpeed{cloudSpeed}
	, m_CurrentFrame{0}
	, m_AccumulateSec{0.f}
	, m_SpriteType{type}
{
	
	m_pTexture = pTextureManager->GetTexturePointer(filename);


	m_FrameWidth = m_pTexture->GetWidth() / m_Columns;
	m_FrameHeight = m_pTexture->GetHeight() / m_Rows;
	
	m_SecondCloudPos.x = m_DrawPos.x + m_pTexture->GetWidth();
	m_SecondCloudPos.y = m_DrawPos.y;
	


}




void Sprite::Update(float elapsedSec)
{
	m_AccumulateSec += elapsedSec;
	if (m_AccumulateSec >= 1.f/ m_NrFramesPerSec)
	{
		++m_CurrentFrame;
		m_AccumulateSec -= 1.f / m_NrFramesPerSec;
	}
	if (m_CurrentFrame > m_TotalNrFrames)
	{
		m_CurrentFrame = 0;
	}


	if (m_IsCloud == true)
	{
		m_DrawPos.x -= m_CloudSpeed * elapsedSec;
		m_SecondCloudPos.x = m_DrawPos.x + m_pTexture->GetWidth();
		if (m_DrawPos.x + m_pTexture->GetWidth() <= 0.f)
		{
			m_DrawPos.x =0.f;
		}
	}

}



void  Sprite::Draw() const
{
	
	Rectf sourceRect{};
	sourceRect.left		= (m_CurrentFrame % m_Columns) * m_FrameWidth;
	sourceRect.bottom	= (m_CurrentFrame / m_Columns + 1) * m_FrameHeight;
	sourceRect.width	= m_FrameWidth;
	sourceRect.height	= m_FrameHeight;

	m_pTexture->Draw(m_DrawPos, sourceRect);
	if (m_IsCloud == true)
	{
		m_pTexture->Draw(m_SecondCloudPos, sourceRect);
	}
}



float Sprite::GetFrameWidth() const
{
	return m_FrameWidth;
}

float Sprite::GetFrameHeight() const
{
	return m_FrameHeight;
}

Sprite::SpriteType Sprite::GetSpriteType() const
{
	return m_SpriteType;
}

void Sprite::SetDrawPos(const Point2f& newDrawPos)
{
	m_DrawPos = newDrawPos;
}

int Sprite::GetCurrentFrame()
{
	return m_CurrentFrame;
}

int Sprite::GetTotalAmountOfFrames()
{
	return m_Columns * m_Rows;
}

void Sprite::SetCurrentFrame(int newFrame)
{
	m_CurrentFrame = newFrame;
}

void Sprite::SetFrameRate(int newFrameRate)
{
	m_NrFramesPerSec = newFrameRate;
}
