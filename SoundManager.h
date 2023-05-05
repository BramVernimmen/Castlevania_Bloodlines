#pragma once
class SoundStream;
class SoundEffect;
#include <unordered_map>

class SoundManager final
{
public:
	SoundManager();
	SoundManager(const SoundManager& other) = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) = delete;
	SoundManager& operator=(SoundManager&& other) = delete;
	~SoundManager();


	SoundStream* GetSoundStreamPointer(const std::string& fileName);
	SoundEffect* GetSoundEffectPointer(const std::string& fileName);

	void PauseAll();
	void ResumeAll();

	void IncreaseVolume();
	void DecreaseVolume();

	void MuteAndUnMute();

	int GetGlobalValue() const;

private:
	std::unordered_map<std::string, SoundStream*> m_SoundStreamMap;
	std::unordered_map<std::string, SoundEffect*> m_SoundEffectMap;

	int m_GlobalVolume; // [0,128] -> change with in/decrements of 4
	// use this to have a global volume for every sound/effect, makes everything easier3

	bool m_IsMuted;
};

