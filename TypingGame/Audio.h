#pragma once
#include <atlstr.h>
#include <map>

class Audio
{
private:
    static CString bgmName;
    static CString nowPlayingSoundName;

public:
    static void SetBGMName(const char* name);
    static void PlayAsync(const char* filePath, bool isLoop);
    static void PlaySync(const char* filePath, bool isLoop);
    static void RePlayBGM();
    static void PlayOneShotInit();
    static void PlayOneShot(const char* filePath);
};

