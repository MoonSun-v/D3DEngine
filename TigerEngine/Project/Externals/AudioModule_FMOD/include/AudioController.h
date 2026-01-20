#pragma once

#include <memory>

#include "AudioClip.h"
#include "AudioListenerComponent.h"
#include "AudioSourceComponent.h"

class AudioSystem;

class AudioController
{
public:
    void Init(AudioSystem* system);

    void BindListener(const AudioTransformRef& ref);
    void BindSource(const AudioTransformRef& ref);
    void SetListenerFallback(const AudioTransform& transform);
    void SetSourceFallback(const AudioTransform& transform);

    void SetClip(std::shared_ptr<AudioClip> clip);
    void SetLoop(bool loop);
    void SetVolume(float volume);
    void SetPitch(float pitch);
    void Set3DMinMaxDistance(float minDist, float maxDist);

    void Update();
    void Update3D();

    void Play(bool restart = true);
    void PlayOneShot();
    void Stop();
    bool IsPlaying() const;

private:
    AudioListenerComponent m_listener{};
    AudioSourceComponent m_source{};
    std::shared_ptr<AudioClip> m_clip{};
};
