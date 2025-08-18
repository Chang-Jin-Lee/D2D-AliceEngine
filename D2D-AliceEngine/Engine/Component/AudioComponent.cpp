#include "pch.h"
#include "AudioComponent.h"
#include <Helpers/StringHelper.h>

AudioComponent::AudioComponent(const std::wstring& name)
{
	m_name = name;
}

AudioComponent::~AudioComponent()
{
	//Stop();
}

void AudioComponent::Initialize()
{
	AudioManager::GetInstance().Initialize();

	m_Channel = nullptr;
}

void AudioComponent::LoadData(const std::wstring& audioPath, AudioMode audioMode, SoundType type)
{
	std::wstring filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + L"Sound\\" + audioPath);

	AudioManager::GetInstance().LoadSound(m_name, filePath, audioMode, type, &m_Sound);
}

void AudioComponent::Play(float sec, float volume, bool paused)
{
	//m_volume = volume;

	int startSec = sec * 1000;	// s -> ms

	if (!m_Sound) return;

	//AudioManager::GetInstance().PlaySound(&m_Sound, &m_Channel, volume, startSec, paused);
}

void AudioComponent::PlayByName(const std::wstring& name, float sec, float volume, bool paused)
{
	int startSec = sec * 1000;	// s -> ms

	if (!m_Sound) return;

	AudioManager::GetInstance().PlaySoundByName(name, &m_Channel, volume, startSec, paused);
}

void AudioComponent::PlayByName1(const std::wstring& name, float volume)
{
    PlayByName(name, 0.0f, volume, false);
}

void AudioComponent::RestartByName(const std::wstring& name, float sec, float volume)
{
	StopByName(name);
	PlayByName(name, sec, volume);
}

void AudioComponent::RestartByName1(const std::wstring& name, float volume)
{
    RestartByName(name, 0.0f, volume);
}

void AudioComponent::SetMasterVolume(float volume)
{
	AudioManager::GetInstance().SetMasterVolume(volume);
}

void AudioComponent::AddMasterVolume(float volume)
{
	m_MasterVolume += volume;
	AudioManager::GetInstance().SetMasterVolume(m_MasterVolume);
}

float AudioComponent::GetMasterVolume()
{
	return AudioManager::GetInstance().GetMasterVolume();
}

void AudioComponent::SetVolume(float volume)
{
	if (m_Channel)
	{
		m_volume = volume;
		m_Channel->setVolume(volume);
	}
}

float AudioComponent::GetVolume()
{
	return this->m_volume;
}

void AudioComponent::SetVolumeByType(SoundType type, float volume)
{
	if (volume > 1.0f) volume = 1.0f;
	if (volume < 0.0f) volume = 0.0f;

	switch (type)
	{
	case SoundType::BGM:
		AudioManager::GetInstance().SetBGMVolume(volume);
		break;
	case SoundType::SFX:
		AudioManager::GetInstance().SetSFXVolume(volume);
		break;
	}
}

void AudioComponent::AddVolumeByType(SoundType type, float volume)
{
	float currentVolume = GetVolume(type);
	currentVolume += volume;

	if (currentVolume > 0.99f) currentVolume = 0.99f;
	if (currentVolume < 0.01f) currentVolume = 0.01f;

	switch (type)
	{
	case SoundType::BGM:
		AudioManager::GetInstance().SetBGMVolume(currentVolume);
		break;
	case SoundType::SFX:
		AudioManager::GetInstance().SetSFXVolume(currentVolume);

		break;
	}
}

float AudioComponent::GetVolume(SoundType type)
{
	float volume = 0.0f;

	switch (type)
	{
	case SoundType::BGM:
		volume = AudioManager::GetInstance().GetBGMVolume();
		break;
	case SoundType::SFX:
		volume = AudioManager::GetInstance().GetSFXVolume();
		break;

	default:
		break;
	}

	return volume;
}

void AudioComponent::AddVolume(float volume)
{
	m_volume += volume;

	if (m_volume > 1.0f) m_volume = 1.0f;
	if (m_volume < 0.0f) m_volume = 0.0f;

	if (m_Channel)
		m_Channel->setVolume(m_volume);
}

bool AudioComponent::IsPlaying() const
{
	bool isPlaying = false;

	if (m_Channel)
		m_Channel->isPlaying(&isPlaying);

	return isPlaying;
}

void AudioComponent::PauseByName(const std::wstring& name, bool paused)
{
	AudioManager::GetInstance().PauseSoundByName(name, paused);
}

void AudioComponent::ResumeByName(const std::wstring& name)
{
	PauseByName(name, false);
}

void AudioComponent::StopByName(const std::wstring& name)
{
	AudioManager::GetInstance().StopSoundByName(name);
}

void AudioComponent::PauseByType(SoundType type, bool paused)
{
	switch (type)
	{
	case SoundType::BGM:
		AudioManager::GetInstance().PauseSoundByType(type, paused);
		break;
	case SoundType::SFX:
		AudioManager::GetInstance().PauseSoundByType(type, paused);
		break;

	default:
		break;
	}
}

void AudioComponent::ResumeByType(SoundType type)
{
	PauseByType(type, false);
}

void AudioComponent::StopByType(SoundType type)
{
	AudioManager::GetInstance().StopSoundByType(type);
}

void AudioComponent::Pause()
{
	AudioManager::GetInstance().PauseAll();
}

void AudioComponent::Resume()
{
	AudioManager::GetInstance().ResumeAll();
}

void AudioComponent::Stop()
{
	AudioManager::GetInstance().StopAll();
}

float AudioComponent::GetPlayTime() const
{
	return AudioManager::GetInstance().GetMusicTime(m_Channel);
}
