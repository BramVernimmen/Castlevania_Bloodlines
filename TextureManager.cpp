#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	// destroy all textures
	for (std::pair<std::string, Texture*> mappedElement : m_Map)
	{
		delete mappedElement.second;
		mappedElement.second = nullptr;
	}
	m_Map.clear();
}

Texture* TextureManager::GetTexturePointer(const std::string& fileName)
{
	Texture* pTex{ m_Map[fileName] };
	if (!pTex) // is een nullptr
	{
		pTex = m_Map[fileName] = new Texture(fileName);
	}
	return pTex;
}
