/**
 *  @file		SoundManagerWin.h
 *  @brief		サウンドマネージャクラス
 */
#ifndef __SOUNDMANAGERWIN_H__
#define	__SOUNDMANAGERWIN_H__

// インクルード
#include	"SoundManager.h"


#define	gSoundManagerWin	CSoundManagerWin::GetInstance()


//! CSoundManagerWinクラス
class CSoundManagerWin : public CSoundManager
{
  protected:
								CSoundManagerWin(void) {
								}
	virtual						~CSoundManagerWin(void) {
								}

			CVoiceElement*		CreateVoiceElement(UINT32 nType);

  public:
	static	CSoundManagerWin&	GetInstance(void);
};


#endif		// __SOUNDMANAGERWIN_H__
/*  Bottom of SoundManagerWin.h  */
