
#include "Audio.h"
#include <atlstr.h>

#pragma comment(lib, "winmm.lib")

#include <mmsystem.h>

CString Audio::bgmName;
CString Audio::nowPlayingSoundName;

void Audio::SetBGMName(const char* name)
{
    bgmName = name;
}

void Audio::PlayAsync(const char* filePath, bool isLoop)
{
    if (isLoop) {
        PlaySound(CString(filePath), nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
    else {
        PlaySound(CString(filePath), nullptr, SND_FILENAME | SND_ASYNC);
    }
}

void Audio::PlaySync(const char* filePath, bool isLoop)
{
    if (isLoop) {
        PlaySound(CString(filePath), nullptr, SND_FILENAME | SND_SYNC | SND_LOOP);
    }
    else {
        PlaySound(CString(filePath), nullptr, SND_FILENAME | SND_SYNC);
    }
}

void Audio::RePlayBGM()
{
    if (!bgmName.IsEmpty()) {
        PlaySound(bgmName, nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
}

void Audio::PlayOneShotInit()
{
    nowPlayingSoundName.Empty();
}

void Audio::PlayOneShot(const char* filePath)
{
    if (nowPlayingSoundName.IsEmpty()) {
        PlaySound(CString(filePath), nullptr, SND_FILENAME | SND_ASYNC);
        nowPlayingSoundName = filePath;
    }
}
