/**
 *  @file		SoundManager.h
 *  @brief		サウンドマネージャクラス
 */
#ifndef __SOUNDMANAGER_H__
#define	__SOUNDMANAGER_H__

// インクルード
#include	"VoiceElement.h"


const	INT32		cnSndMgr_MasterVolumeNum	= 2;			//!< マスターボリューム数
const	INT32		cnSndMgr_BgmSlotNum			= 4;			//!< ＢＧＭスロット数
const	INT32		cnSndMgr_SeListSlotNum		= 4;			//!< ＳＥリストスロット数
const	INT32		cnSndMgr_MaxSeNum			= 64;			//!< 最大同時ＳＥ発音数

#define	gSoundManager		CSoundManager::GetInstance()

const	UINT32		cnSndMgr_LoopInfinity		= 0xffffffff;	//!< 無限ループ
const	UINT32		cnSndMgr_PlayOnce			= 0;			//!< 1回再生


// サウンドリスト構造体定義
typedef struct
{
			UINT16				wNumber;						//!< ナンバー
			UINT16				cWavType;						//!< WAVタイプ
			UINT32				nLoop;							//!< ループするかどうか
			TCHAR*				szFileName;						//!< ファイル名
			bool				bOnMemory;						//!< オンメモリ再生
			UINT32				nLoopInSample;					//!< ループインポイント
			UINT32				nFlag;							//!< フラグ
}SOUND_LIST;


// ＢＧＭ情報構造体定義
typedef struct
{
			CVoiceElement*		pVoice[2];						//!< Voiceエレメント
			UINT32				dwPlayingBgmNum;				//!< 現在再生中のＢＧＭナンバー
			UINT32				dwNextPlayBgmNum;				//!< 次に再生するＢＧＭナンバー
			INT32				nVolume;						//!< 再生ボリューム
			INT32				nFadeInTick;					//!< 再生を行う際のフェードインタイム
			INT32				nFadeOutTick[2];				//!< 再生をやめる際のフェードアウトタイム
			bool				bPause;							//!< ポーズ中フラグ
}SNDMGR_BGMINFO;


//! CSoundManagerクラス
class CSoundManager
{
  private:
			INT32				m_nMasterVolume[cnSndMgr_MasterVolumeNum];
	  
	  const	SOUND_LIST*			m_pBgmList;
			UINT32				m_dwBgmListNum;
			SNDMGR_BGMINFO		m_stBgmInfo[cnSndMgr_BgmSlotNum];
			INT32				m_iNowPlayingBgmSlot;
			INT32				m_iNextPlayBgmSlot;

	const	SOUND_LIST*			m_pSeList[cnSndMgr_SeListSlotNum];
			UINT32				m_dwSeListNum[cnSndMgr_SeListSlotNum];
			CVoiceElement*		m_pSeVoice[cnSndMgr_MaxSeNum];

  private:
			void				ReleaseBgmData(void);
			void				ReleaseSeData(UINT32 nSlot);
	const	SOUND_LIST*			GetListData(const SOUND_LIST* pList, UINT32 nListNum, UINT32 dwNum)	const	{
									for (UINT32 i = 0; i < nListNum; i++, pList++) {
										if (pList->wNumber == dwNum)		return pList;
									}
									return NULL;
								}

  protected:
								CSoundManager(void);
	virtual						~CSoundManager(void);

								//!	ボイスエレメント作成
								/**
								 *	ボイスエレメントを作成します。<br>
								 *	独自のボイスエレメントを生成したい場合は、派生してください。<br>
								 *
								 *	@return	生成したボイスエレメントポインタ
								 */
	virtual	CVoiceElement*		CreateVoiceElement(UINT32 nType) {
									return new CVoiceElement;
								}
	virtual	void*				GetDataPtr(const TCHAR* szName, unsigned long& nSize)	const	{
									nSize = 0;
									return NULL;
								}
	virtual	void				ReleaseDataPtr(const TCHAR* szName) {};
	virtual	bool				SetData(CVoiceElement* pVoice, const SOUND_LIST* pList);

  public:
	virtual	BOOL				IsEnable(void)	const	{
									return TRUE;
								}
	virtual	void				Run(FLOAT fSyncTime);
	virtual	void				DeleteThis(void) {}

			// リセット
	virtual	void				ResetSoundSystem(void);

			// マスターボリューム
			INT32				GetMasterVolume()	const;
			INT32				GetMasterVolume(UINT32 nChannel)	const;
			void				SetMasterVolume(UINT32 nChannel, INT32 nVolume);

			// BGM周り
			void				SetBgmList(const SOUND_LIST* pList, UINT32 nListNum);
			void				SetBgm(UINT32 nSlot, UINT32 nBgmNum, INT32 nVolume = cnVoiceElm_VolumeMax, INT32 fFadeInTick = 0, INT32 fFadeOutTick = 0);
			void				PlayBgm(UINT32 nSlot, INT32 nVolume = -1, INT32 nFadeInTick = -1);
			void				StopBgm(UINT32 nSlot, INT32 nFadeOutTick = -1);
			void				PauseBgm(bool bPause, INT32 nSlot = -1, INT32 nFadeTick = -1);
			bool				IsPauseBgm(INT32 nSlot = -1)	const;

			INT32				GetBgmVolume(UINT32 nSlot)	const;
			void				SetBgmVolume(UINT32 nSlot, INT32 nVolume, INT32 nFadeTick = 0, INT32 nStartVolume = -1);

								//! 指定したスロットが再生中かどうか
								/**
								 *	@param	nSlot	[in] BGMスロットナンバー
								 *
								 *	@return		再生中の場合はtrueを返す
								 */
			bool				IsPlayingBgm(UINT32 nSlot)	const	{
									return (nSlot < cnSndMgr_BgmSlotNum && m_iNowPlayingBgmSlot == nSlot);
								}
								//! 再生中のBGMスロットナンバー
								/**
								 *	@return		再生中のBGMスロットナンバー
								 */
			INT32				GetPlayingBgmSlot(void)	const	{
									return (INT32)m_iNowPlayingBgmSlot;
								}
								//! 再生中のBGMナンバー
								/**
								 *	@return		再生中のBGMナンバー
								 */
			UINT32				GetPlayingBgmNumber(void)	const	{
									if (m_iNowPlayingBgmSlot < 0) {
										return 0;
									}
									return m_stBgmInfo[m_iNowPlayingBgmSlot].dwPlayingBgmNum;
								}

			// SE周り
			void				SetSeList(UINT32 nSlot, const SOUND_LIST* pList, UINT32 nListNum);

			bool				GetUnique(UINT32 nSlot, UINT32& nUnique)	const;

			bool				IsPlayingBySlot(UINT32 nSlot)	const;
			bool				IsPlayingByUnique(UINT32 nUnique, UINT32 nUniqueMask = 0xffffffff)	const;
			bool				IsPlayingBySeNum(UINT32 nSlot, UINT32 nSeNum = 0xffffffff)	const;

			bool				PlaySe(UINT32 nSlot, UINT32 nSeNum, UINT32 nUnique, INT32 nVolume = cnVoiceElm_VolumeMax, INT32 nFadeInTick = 0);
			void				StopSe(UINT32 nUnique, UINT32 nUniqueMask = 0xffffffff, INT32 nFadeOutTick = 0);
			void				StopSeNum(UINT32 nSlot, UINT32 nSeNum = 0xffffffff, INT32 nFadeOutTick = 0);
			void				PauseSe(bool bPause, UINT32 nUnique, UINT32 nUniqueMask = 0xffffffff, INT32 nFadeTick = 0);
			bool				IsPauseSe(UINT32 nUnique)	const;

			INT32				GetSeVolume(UINT32 nUnique)	const;
			void				SetSeVolume(INT32 nVolume, UINT32 nUnique, UINT32 nUniqueMask = 0xffffffff, INT32 nTick = 0);
			INT32				GetSePanpot(UINT32 nUnique)	const;
			void				SetSePanpot(INT32 nPanpot, UINT32 nUnique, UINT32 nUniqueMask = 0xffffffff, INT32 nTick = 0);

	static	CSoundManager&		GetInstance(void);
};


#endif		// __SOUNDMANAGER_H__
/*  Bottom of SoundManager.h  */
