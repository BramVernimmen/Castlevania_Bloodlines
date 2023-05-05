#pragma once
class Texture;
#include <unordered_map>

class TextureManager final
{
public:
	TextureManager();

	TextureManager(const TextureManager& other) = delete;
	TextureManager& operator=(const TextureManager& other) = delete;
	TextureManager(TextureManager&& other) = delete;
	TextureManager& operator=(TextureManager&& other) = delete;
	~TextureManager();


	Texture* GetTexturePointer(const std::string& fileName);

private:
	std::unordered_map<std::string, Texture*> m_Map;
};

