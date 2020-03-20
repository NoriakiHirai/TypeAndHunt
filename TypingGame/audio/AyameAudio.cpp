#include "AyameAudio.h"
#include <cstring>

AyameAudio::~AyameAudio()
{
}

void AyameAudio::SetClip(const char* fileName)
{
	// tchar‚É•ÏŠ·
#ifdef _UNICODE
	TCHAR buf[256];
	MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, fileName, strlen(fileName),
		buf, (sizeof buf) / 2);
	m_Sound.Load(buf, 0, 0, 0);
#else
	m_Sound.Load(fileName, 0, 0, 0);
#endif
}

bool AyameAudio::isPlaying() const
{
	return m_Sound.IsPlaying();
}

void AyameAudio::SetLoop(bool loop)
{
	m_Loop = loop;
}

bool AyameAudio::GetLoop() const
{
	return m_Loop;
}

void AyameAudio::SetVolume(float volume)
{
	m_Sound.SetVolume((int)(volume * 100));
}

float AyameAudio::GetVolume() const
{
	return m_Sound.GetVolume() / 100.0f;
}

void AyameAudio::Play()
{
	m_Sound.Play(m_Loop ? 1 : 0, 100);
}

void AyameAudio::Stop()
{
	m_Sound.Stop();
}

void AyameAudio::Pause()
{
	m_Sound.Pause(true);
}

void AyameAudio::UnPause()
{
	m_Sound.Pause(false);
}
