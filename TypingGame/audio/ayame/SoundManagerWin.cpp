/**
 *  @file		SoundManagerWin.cpp
 *  @brief		サウンドマネージャクラス
 */

// インクルード
#include	"SoundManagerWin.h"
#include	"VoiceElementAyame.h"

CVoiceElement* CSoundManagerWin::CreateVoiceElement(UINT32 nType)
{
	return new CVoiceElementAyame;
}


CSoundManagerWin& CSoundManagerWin::GetInstance(void)
{
	static	CSoundManagerWin	gsSoundManager;

	return gsSoundManager;
}


/*  Bottom of SoundManager.cpp  */
