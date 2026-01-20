#pragma once

#include "pch.h"
#include "../Object/Component.h"
#include "..\\..\\Externals\\AudioModule_FMOD\\include\\AudioSource.h"
#include "..\\..\\Externals\\AudioModule_FMOD\\include\\AudioTransform.h"

class AudioSystem;
class AudioClip;

class AudioSourceComponent : public Component
{
    RTTR_ENABLE(Component)
public:
    AudioSourceComponent() { SetName("AudioSource"); }
    ~AudioSourceComponent() override = default;

    void OnInitialize() override;
    void OnUpdate(float delta) override;
    void OnDestory() override;

    void Init(AudioSystem* system);
    void BindTransform(const AudioTransformRef& ref);
    void SetFallback(const AudioTransform& transform);

    void SetClip(std::shared_ptr<AudioClip> clip);
    void SetLoop(bool loop);
    void SetVolume(float volume);
    void SetPitch(float pitch);
    void Set3DMinMaxDistance(float minDist, float maxDist);

    void Play(bool restart = true);
    void PlayOneShot();
    void Stop();
    void Update3D();
    bool IsPlaying() const;

private:
    AudioSystem* m_System = nullptr;
    AudioSource m_Source{};
    AudioTransform m_Fallback{};
    AudioTransformRef m_Bind{};
};
