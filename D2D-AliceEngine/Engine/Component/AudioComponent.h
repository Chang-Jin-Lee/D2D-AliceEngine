#pragma once
#include <Component/Component.h>
#include <Manager/AudioManager.h>

class AudioComponent : public Component
{
public:
	AudioComponent() { m_name = L""; }
	AudioComponent(const std::wstring& name);
	~AudioComponent();

	void Initialize() override;

	void LoadData(
		const std::wstring& audioPath,
		AudioMode audioMode, SoundType type);

	void Play(float sec = 0,
		float volume = 0.3f,
		bool paused = false);

	void PlayByName(
		const std::wstring& name,
		float sec = 0,
		float volume = 0.3f,
		bool paused = false);
    
    void PlayByName1(const std::wstring& name, float volume);
    void RestartByName1(const std::wstring& name, float volume);

	void RestartByName(const std::wstring& name, float sec = 0, float volume = 0.3f);
   
	void SetMasterVolume(float volume);
	void AddMasterVolume(float volume);
	float GetMasterVolume();

	void SetVolume(float volume);
	void AddVolume(float volume);
	float GetVolume();

	void SetVolumeByType(SoundType type, float volume);
	void AddVolumeByType(SoundType type, float volume);
	float GetVolume(SoundType type);

	bool IsPlaying() const;

	void PauseByName(const std::wstring& name, bool paused = true);
	void ResumeByName(const std::wstring& name);
	void StopByName(const std::wstring& name);

	void PauseByType(SoundType type, bool paused = true);
	void ResumeByType(SoundType type);
	void StopByType(SoundType type);

	void Pause();
	void Resume();
	void Stop();

	float GetPlayTime() const;

private:
	SoundData* m_Sound = nullptr;
	FMOD::Channel* m_Channel = nullptr;

	bool isLoaded = false;

	float m_volume = 0.3f;
	float m_MasterVolume = 1.0f;

	std::wstring m_name;
};

