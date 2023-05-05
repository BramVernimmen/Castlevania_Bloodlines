#include "pch.h"
#include "SoundManager.h"
#include "SoundStream.h"
#include "SoundEffect.h"


SoundManager::SoundManager()
	: m_IsMuted{false}
	, m_GlobalVolume{32} // 52
{

}

SoundManager::~SoundManager()
{
	// destroy all SoundStreams
	for (std::pair<std::string, SoundStream*> mappedElement : m_SoundStreamMap)
	{
		delete mappedElement.second;
		mappedElement.second = nullptr;
	}
	m_SoundStreamMap.clear();
	
	// destroy all SoundEffects
	for (std::pair<std::string, SoundEffect*> mappedElement : m_SoundEffectMap)
	{
		delete mappedElement.second;
		mappedElement.second = nullptr;
	}
	m_SoundEffectMap.clear();
}




SoundStream* SoundManager::GetSoundStreamPointer(const std::string& fileName)
{
	SoundStream* pStream{ m_SoundStreamMap[fileName] };
	if (!pStream) // is een nullptr
	{
		pStream = m_SoundStreamMap[fileName] = new SoundStream(fileName);
		if (m_IsMuted == true)
		{
			pStream->SetVolume(0);
		}
		else
		{
			pStream->SetVolume(m_GlobalVolume);
		}
	}
	return pStream;
}


SoundEffect* SoundManager::GetSoundEffectPointer(const std::string& fileName)
{
	SoundEffect* pEffect{ m_SoundEffectMap[fileName] };
	if (!pEffect) // is een nullptr
	{
		pEffect = m_SoundEffectMap[fileName] = new SoundEffect(fileName);
		if (m_IsMuted == true)
		{
			pEffect->SetVolume(0);
		}
		else
		{
			pEffect->SetVolume(m_GlobalVolume);
		}
		
	}
	return pEffect;
}



void SoundManager::PauseAll()
{
	for (std::pair<std::string, SoundStream*> mappedElement : m_SoundStreamMap)
	{
		mappedElement.second->Pause();
	}

	for (std::pair<std::string, SoundEffect*> mappedElement : m_SoundEffectMap)
	{
		mappedElement.second->PauseAll();
	}
}

void SoundManager::ResumeAll()
{
	for (std::pair<std::string, SoundStream*> mappedElement : m_SoundStreamMap)
	{
		mappedElement.second->Resume();
	}

	for (std::pair<std::string, SoundEffect*> mappedElement : m_SoundEffectMap)
	{
		mappedElement.second->ResumeAll();
	}
}

void SoundManager::IncreaseVolume()
{
	if (m_IsMuted == false)
	{

		for (std::pair<std::string, SoundStream*> mappedElement : m_SoundStreamMap)
		{
			int currentVolume{ mappedElement.second->GetVolume() };
			if (currentVolume < 128)
			{
				mappedElement.second->SetVolume(currentVolume + 4);
			}
		}

		for (std::pair<std::string, SoundEffect*> mappedElement : m_SoundEffectMap)
		{
			int currentVolume{ mappedElement.second->GetVolume()};
			if (currentVolume < 128)
			{
				mappedElement.second->SetVolume(currentVolume + 4);

			}
		}

		if (m_GlobalVolume < 128)
		{
			m_GlobalVolume += 4;
		}
	}
}

void SoundManager::DecreaseVolume()
{
	if (m_IsMuted == false)
	{
		for (std::pair<std::string, SoundStream*> mappedElement : m_SoundStreamMap)
		{
			int currentVolume{ mappedElement.second->GetVolume() };
			if (currentVolume > 0)
			{
				mappedElement.second->SetVolume(currentVolume - 4);
				//m_GlobalVolume = mappedElement.second->GetVolume();

			}
		}

		for (std::pair<std::string, SoundEffect*> mappedElement : m_SoundEffectMap)
		{
			int currentVolume{ mappedElement.second->GetVolume() };
			if (currentVolume > 0)
			{
				mappedElement.second->SetVolume(currentVolume - 4);
				//m_GlobalVolume = mappedElement.second->GetVolume();

			}
		}

		if (m_GlobalVolume > 0)
		{
			m_GlobalVolume -= 4;
		}
	}
}

void SoundManager::MuteAndUnMute()
{
	m_IsMuted = !m_IsMuted;
	if (m_IsMuted == true)
	{
		for (std::pair<std::string, SoundStream*> mappedElement : m_SoundStreamMap)
		{
			mappedElement.second->SetVolume(0);
			
		}

		for (std::pair<std::string, SoundEffect*> mappedElement : m_SoundEffectMap)
		{
			mappedElement.second->SetVolume(0);

		}
	}
	else
	{
		for (std::pair<std::string, SoundStream*> mappedElement : m_SoundStreamMap)
		{
			mappedElement.second->SetVolume(m_GlobalVolume);

		}

		for (std::pair<std::string, SoundEffect*> mappedElement : m_SoundEffectMap)
		{
			mappedElement.second->SetVolume(m_GlobalVolume);

		}
	}
}

int SoundManager::GetGlobalValue() const
{
	return m_GlobalVolume;
}
