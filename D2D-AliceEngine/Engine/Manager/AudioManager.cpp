#include "pch.h"
#include "AudioManager.h"
#include <Helpers/StringHelper.h>

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
	for (auto& pair : m_SoundMap)
	{
		SoundData* data = pair.second;
		if (data)
		{
			if (data->sound)
				data->sound->release();

			delete data;
		}
	}

	m_SoundMap.clear();

	if (m_System)
	{
		m_System->close();
		m_System->release();
		m_System = nullptr;
	}
}


void AudioManager::Initialize()
{
	if (m_System) return;

	FMOD_RESULT fr = FMOD::System_Create(&m_System);
	if (fr != FMOD_OK) return;

	fr = m_System->init(512, FMOD_INIT_NORMAL, nullptr);
	if (fr != FMOD_OK) return;

	m_System->getMasterChannelGroup(&m_MasterGroup);

	// 하위 그룹 생성
	m_System->createChannelGroup("BGM", &m_BGMGroup);
	m_System->createChannelGroup("SFX", &m_SFXGroup);
	//m_System->createChannelGroup("UI", &m_UIGroup);

	// 마스터 그룹에 각각 붙임
	m_MasterGroup->addGroup(m_BGMGroup);
	m_MasterGroup->addGroup(m_SFXGroup);
	//m_MasterGroup->addGroup(m_UIGroup);
}

void AudioManager::Update()
{
	m_System->update();
}

void AudioManager::LoadSound(const std::wstring& name, const std::wstring& path, AudioMode audioMode, SoundType type, SoundData** sound)
{
	if (m_SoundMap.find(name) != m_SoundMap.end())
	{
		*sound = m_SoundMap[name];
		return;
	}

	std::string pathStr = StringHelper::wstring_to_string(path);
	FMOD::Sound* newSound = nullptr;

	FMOD_RESULT fr = m_System->createSound(pathStr.c_str(), static_cast<FMOD_MODE>(audioMode), 0, &newSound);

	if (fr != FMOD_OK)
	{
		return;
	}

	SoundData* data = new SoundData();
	data->sound = newSound;
	data->type = type;

	m_SoundMap[name] = data;
	*sound = data;
}

//void AudioManager::LoadSound(const std::wstring& path, AudioMode audioMode, FMOD::Sound** sound)
//{
//	std::string ss = StringHelper::wstring_to_string(path);
//
//	auto it = m_SoundMap.find(ss);
//	if (it != m_SoundMap.end())
//	{
//		*sound = it->second;
//		return;
//	}
//
//	FMOD::Sound* newSound = nullptr;
//
//	FMOD_RESULT fr = m_System->createSound(ss.c_str(), static_cast<FMOD_MODE>(audioMode), nullptr, &newSound);
//	if (fr != FMOD_OK)
//	{
//		return;
//	}
//
//	m_SoundMap[ss] = newSound;
//
//	*sound = newSound;
//}

void AudioManager::PlaySound(FMOD::Sound* sound, FMOD::Channel** channel, float volume, float startTime, bool paused)
{
	if (!sound || !channel) return;

	if (volume > 1.0f) volume = 1.0f;
	if (volume < 0.0f) volume = 0.0f;

	FMOD_RESULT fr = m_System->playSound(sound, nullptr, paused, channel);

	if (fr != FMOD_OK) return;

	if (*channel)
	{
		(*channel)->setVolume(volume);
		(*channel)->setPosition(startTime, FMOD_TIMEUNIT_MS);
	}
}

void AudioManager::PlaySoundByName(
	const std::wstring& name,
	FMOD::Channel** outChannel,
	float volume,
	float startTime,
	bool paused)
{
	auto it = m_SoundMap.find(name);
	if (it == m_SoundMap.end()) return;

	SoundData* data = it->second;
	if (!data || !data->sound) return;

	FMOD::ChannelGroup* group = nullptr;

	switch (data->type)
	{
	case SoundType::BGM: group = m_BGMGroup; break;
	case SoundType::SFX: group = m_SFXGroup; break;
	}

    // 기존 채널이 살아있으면 중복 채널 생성을 막기 위해 정지
    if (data->channel)
    {
        bool isPlaying = false;
        data->channel->isPlaying(&isPlaying);
        if (isPlaying)
        {
            data->channel->stop();
        }
    }

    FMOD::Channel* channel = nullptr;
    FMOD_RESULT fr = m_System->playSound(data->sound, group, true, &channel); // 일단 pause 상태로 생성

	if (fr != FMOD_OK) return;

	if (channel)
	{
		channel->setVolume(volume);
        channel->setPosition(static_cast<unsigned int>(startTime), FMOD_TIMEUNIT_MS);
        channel->setPaused(paused);
        // 내부에 채널 저장해 Pause/StopByName이 동작하도록 함
        data->channel = channel;
	}

	if (outChannel)
        *outChannel = channel;
}

void AudioManager::PauseSoundByName(const std::wstring& name, bool paused)
{
	auto it = m_SoundMap.find(name);
	if (it != m_SoundMap.end() && it->second->channel)
		it->second->channel->setPaused(paused);
}

void AudioManager::ResumeSoundByName(const std::wstring& name)
{
	PauseSoundByName(name, false);
}

void AudioManager::StopSoundByName(const std::wstring& name)
{
	auto it = m_SoundMap.find(name);
    if (it != m_SoundMap.end() && it->second->channel)
    {
        it->second->channel->stop();
        it->second->channel = nullptr;
    }
}

void AudioManager::PlaySoundByType(SoundType type, FMOD::Channel** outChannel, float volume, float startTime, bool paused)
{
	if (outChannel)
		*outChannel = nullptr;

	for (auto& pair : m_SoundMap)
	{
		SoundData* data = pair.second;
		if (!data || data->type != type || !data->sound)
			continue;

		// startTime 위치부터 재생
		m_System->playSound(data->sound, nullptr, true, &data->channel); // 먼저 일시정지된 상태로 play

		if (data->channel)
		{
			data->channel->setVolume(volume);
			data->channel->setPosition(static_cast<unsigned int>(startTime * 1000.0f), FMOD_TIMEUNIT_MS);
			data->channel->setPaused(paused);

			if (outChannel && *outChannel == nullptr)
				*outChannel = data->channel;
		}
	}
}

void AudioManager::PauseSoundByType(SoundType type, bool paused)
{
    for (auto& pair : m_SoundMap)
    {
        SoundData* data = pair.second;
        if (!data) continue;
        if (data->type != type) continue;
        if (!data->channel) continue;
        bool alive = false;
        data->channel->isPlaying(&alive);
        if (alive)
            data->channel->setPaused(paused);
    }
}

void AudioManager::ResumeSoundByType(SoundType type)
{
	PauseSoundByType(type, false);
}

void AudioManager::StopSoundByType(SoundType type)
{
	for (auto& pair : m_SoundMap)
	{
		SoundData* data = pair.second;
		if (data->type == type && data->channel)
			data->channel->stop();
	}
}

void AudioManager::PauseAll()
{
	if (m_MasterGroup)
		m_MasterGroup->setPaused(true);
}

void AudioManager::ResumeAll()
{
	if (m_MasterGroup)
		m_MasterGroup->setPaused(false);
}

void AudioManager::StopAll()
{
    if (m_MasterGroup)
        m_MasterGroup->stop();
    // 내부 레퍼런스 정리: 다음 재생 시 nullptr로 안전하게 갱신
    for (auto& pair : m_SoundMap)
    {
        if (pair.second)
            pair.second->channel = nullptr;
    }
}


float AudioManager::GetMusicTime(FMOD::Channel* channel)
{
	if (!channel) return 0.0f;

	unsigned int time = 0;
	FMOD_RESULT result = channel->getPosition(&time, FMOD_TIMEUNIT_MS);

	if (result == FMOD_OK)
	{
		return static_cast<float>(time) / 1000.0f;	// 밀리초 단위 -> 초 단위
	}

	return 0.0f;
}

float AudioManager::GetMasterVolume()
{
	if (m_MasterGroup)
	{
		FMOD_RESULT fr = m_MasterGroup->getVolume(&m_masterVolume);

		if (fr == FMOD_OK)
		{
			return m_masterVolume;
		}
		else
			return 0.0f;
	}
	
	return 0.0f;
}

void AudioManager::SetMasterVolume(float volume)
{
	if (volume > 1.0f) volume = 1.0f;
	if (volume < 0.0f) volume = 0.0f;

	if (m_MasterGroup)
	{
		m_masterVolume = volume;
		m_MasterGroup->setVolume(volume);
	}
}

float AudioManager::GetBGMVolume() const
{
	return m_bgmVolume;
}

void AudioManager::SetBGMVolume(float volume)
{
	if (volume > 1.0f) volume = 1.0f;
	if (volume < 0.0f) volume = 0.0f;

	if (m_BGMGroup)
	{
		m_bgmVolume = volume;
		m_BGMGroup->setVolume(volume);
	}
}

float AudioManager::GetSFXVolume() const
{
	return m_sfxVolume;
}

void AudioManager::SetSFXVolume(float volume)
{
	if (volume > 1.0f) volume = 1.0f;
	if (volume < 0.0f) volume = 0.0f;

	if (m_SFXGroup)
	{
		m_sfxVolume = volume;
		m_SFXGroup->setVolume(volume);
	}
}

//float AudioManager::GetUIVolume() const
//{
//	return m_UIVolume;
//}

//void AudioManager::SetUIVolume(float volume)
//{
//	if (volume > 1.0f) volume = 1.0f;
//	if (volume < 0.0f) volume = 0.0f;
//
//	if (m_UIGroup)
//	{
//		m_UIVolume = volume;
//		m_UIGroup->setVolume(volume);
//	}
//}