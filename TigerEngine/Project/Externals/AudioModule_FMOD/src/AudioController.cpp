#include "..\\include\\AudioController.h"
#include "..\\include\\AudioSystem.h"

void AudioController::Init(AudioSystem* system)
{
    m_listener.Init(system);
    m_source.Init(system);
}

void AudioController::BindListener(const AudioTransformRef& ref)
{
    m_listener.BindTransform(ref);
}

void AudioController::BindSource(const AudioTransformRef& ref)
{
    m_source.BindTransform(ref);
}

void AudioController::SetListenerFallback(const AudioTransform& transform)
{
    m_listener.SetFallback(transform);
}

void AudioController::SetSourceFallback(const AudioTransform& transform)
{
    m_source.SetFallback(transform);
}

void AudioController::SetClip(std::shared_ptr<AudioClip> clip)
{
    m_clip = std::move(clip);
    m_source.SetClip(m_clip);
}

void AudioController::SetLoop(bool loop)
{
    m_source.SetLoop(loop);
}

void AudioController::SetVolume(float volume)
{
    m_source.SetVolume(volume);
}

void AudioController::SetPitch(float pitch)
{
    m_source.SetPitch(pitch);
}

void AudioController::Set3DMinMaxDistance(float minDist, float maxDist)
{
    m_source.Set3DMinMaxDistance(minDist, maxDist);
}

void AudioController::Update()
{
    m_listener.Update();
    m_source.Update3D();
}

void AudioController::Update3D()
{
    m_source.Update3D();
}

void AudioController::Play(bool restart)
{
    m_source.Play(restart);
}

void AudioController::PlayOneShot()
{
    m_source.PlayOneShot();
}

void AudioController::Stop()
{
    m_source.Stop();
}

bool AudioController::IsPlaying() const
{
    return m_source.IsPlaying();
}
