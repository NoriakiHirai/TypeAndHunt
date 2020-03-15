/**
 *  @file		SoundManager.cpp
 *  @brief		サウンドマネージャクラス
 */

// インクルード
#include	"SoundManager.h"


//!	コンストラクタ
CSoundManager::CSoundManager(void)
{
	m_pBgmList           = NULL;
	m_dwBgmListNum       = 0;
	m_iNowPlayingBgmSlot = -1;
	m_iNextPlayBgmSlot   = -1;
	ZeroMemory(m_stBgmInfo, sizeof(m_stBgmInfo));

	for(INT32 i = 0; i < cnSndMgr_MasterVolumeNum; i++) {
		m_nMasterVolume[i] = cnVoiceElm_VolumeMax;
	}
	CVoiceElement::SetGlobalVolume(cnVoiceElm_VolumeMax);

	for(INT32 i = 0; i < cnSndMgr_SeListSlotNum; i++) {
		m_pSeList[i]     = NULL;
		m_dwSeListNum[i] = 0;
	}
	ZeroMemory(m_pSeVoice, sizeof(m_pSeVoice));
}


//!	デストラクタ
CSoundManager::~CSoundManager(void)
{
	ResetSoundSystem();
}


bool	CSoundManager::SetData(CVoiceElement* pVoice, const SOUND_LIST* pList)
{
	if (pList->bOnMemory == true) {
		unsigned long	nSize;
		void*	pData = GetDataPtr(pList->szFileName, nSize);
		return (pData == NULL)? false : pVoice->Load((char*)pData, nSize, pList->nFlag);
	} else {
		return pVoice->Load(pList->szFileName, pList->nFlag, 0, 0);
	}
}


//!	リセット
/**
 *	サウンドマネージャのリセットを行います。<br>
 */
void CSoundManager::ResetSoundSystem(void)
{
	// 全SEの停止
	for(INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		delete m_pSeVoice[i];
		m_pSeVoice[i] = NULL;
	}

	// BGMリストの消去
	SetBgmList(NULL, 0);

	// SEリストの消去
	for(INT32 i = 0; i < cnSndMgr_SeListSlotNum; i++) {
		SetSeList(i, NULL, 0);
	}
}


//!	定例処理
/**
 *	毎フレーム行う処理です。<br>
 *
 *	@param	fSyncTime	[in] 前回からの経過時間[sec]
 */
void CSoundManager::Run(FLOAT fSyncTime)
{
	// ＢＧＭ定例処理
	SNDMGR_BGMINFO*		pInfo = m_stBgmInfo;
	for (INT32 i = 0; i < cnSndMgr_BgmSlotNum; i++, pInfo++) {
		for (INT32 j = 0; j < 2; j++) {
			if (pInfo->pVoice[j] != NULL) {
				pInfo->pVoice[j]->Run((INT32)(fSyncTime * 480.0f));
				if (pInfo->pVoice[j]->IsFinished() == true) {
					delete pInfo->pVoice[j];
					pInfo->pVoice[j] = NULL;
				}
			}
		}
		if (pInfo->pVoice[0] == NULL && pInfo->pVoice[1] != NULL) {
			pInfo->pVoice[0] = pInfo->pVoice[1];
			pInfo->pVoice[1] = NULL;
		}
	}

	if (m_iNextPlayBgmSlot >= 0) {
		if (m_iNowPlayingBgmSlot >= 0) {
			// 現在のスロットのＢＧＭを停止させる
			PauseBgm(true, m_iNowPlayingBgmSlot);
		}
		m_iNowPlayingBgmSlot = m_iNextPlayBgmSlot;
		m_iNextPlayBgmSlot   = -1;
		SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[m_iNowPlayingBgmSlot];
		if (pInfo->bPause == true) {
			PauseBgm(false, m_iNowPlayingBgmSlot);
		} else if (pInfo->dwNextPlayBgmNum == 0) {
			pInfo->dwNextPlayBgmNum = pInfo->dwPlayingBgmNum;
			pInfo->nFadeOutTick[1]  = pInfo->nFadeOutTick[0];
		}
	}

	if (m_iNowPlayingBgmSlot >= 0) {
		SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[m_iNowPlayingBgmSlot];
		UINT32				dwNextBgmNum = pInfo->dwNextPlayBgmNum;
		if (dwNextBgmNum != 0) {
			// 次のBGMに切り替える
			if (pInfo->pVoice[1] == NULL) {
				if (pInfo->pVoice[0] != NULL) {
					pInfo->pVoice[0]->Stop(pInfo->nFadeOutTick[0]);
				}
				const	SOUND_LIST*		pList = GetListData(m_pBgmList, m_dwBgmListNum, dwNextBgmNum);
				if (pList != NULL) {
					pInfo->pVoice[1] = CreateVoiceElement(pList->cWavType);
					if (pInfo->pVoice[1] != NULL) {
						if (SetData(pInfo->pVoice[1], pList) == false) {
							delete pInfo->pVoice[1];
							pInfo->pVoice[1] = NULL;
						} else {
							pInfo->pVoice[1]->Play(pList->nLoop, pInfo->nVolume, pInfo->nFadeInTick, pList->nLoopInSample);
						}
					}
				}
				pInfo->dwPlayingBgmNum  = dwNextBgmNum;
				pInfo->dwNextPlayBgmNum = 0;
				pInfo->nFadeOutTick[0]  = pInfo->nFadeOutTick[1];
				pInfo->nFadeOutTick[1]  = 0;
			}
		} else {
			if (pInfo->pVoice[0] == NULL && pInfo->pVoice[1] == NULL) {
				m_iNowPlayingBgmSlot = -1;
			}
		}
	}

	// ＳＥの定例処理
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)		continue;

		m_pSeVoice[i]->Run((INT32)(fSyncTime * 480.0f));
		if (m_pSeVoice[i]->IsFinished() == true) {
			delete m_pSeVoice[i];
			m_pSeVoice[i] = NULL;
		}
	}
}


//! マスターボリュームの取得
/**
 *	最終的なマスターボリューム値を取得します。
 *
 *	@return	最終的なマスターボリューム値
 */
INT32 CSoundManager::GetMasterVolume()	const
{
	INT32	nVol = m_nMasterVolume[0];
	for (INT32 i=1; i < cnSndMgr_MasterVolumeNum; i++) {
		nVol *= m_nMasterVolume[i];
		nVol /= cnVoiceElm_VolumeRange;
	}

	return nVol;
}

//! マスターボリュームの取得
/**
 *	指定したマスターボリューム値を取得します。
 *
 *	@param	nChannel	[in] マスターボリュームチャンネル
 *
 *	@return	マスターボリューム値
 */
INT32 CSoundManager::GetMasterVolume(UINT32 nChannel)	const
{
	return (nChannel >= cnSndMgr_MasterVolumeNum)? cnVoiceElm_VolumeMin : m_nMasterVolume[nChannel];
}


//! マスターボリュームの設定
/**
 *	マスターボリュームの設定を行います。
 *
 *	@param	nChannel	[in] マスターボリュームチャンネル
 *	@param	nVolume		[in] マスターボリューム値
 */
void CSoundManager::SetMasterVolume(UINT32 nChannel, INT32 nVolume)
{
	if (nChannel >= cnSndMgr_MasterVolumeNum)	return;

	nVolume = (nVolume < cnVoiceElm_VolumeMin)? cnVoiceElm_VolumeMin : ((nVolume > cnVoiceElm_VolumeMax)? cnVoiceElm_VolumeMax : nVolume);
	m_nMasterVolume[nChannel] = nVolume;

	// ボリュームの再計算
	CVoiceElement::SetGlobalVolume(GetMasterVolume());

	// ボリュームを反映する
	SNDMGR_BGMINFO*		pInfo = m_stBgmInfo;
	for (INT32 i = 0; i < cnSndMgr_BgmSlotNum; i++, pInfo++) {
		for (INT32 j = 0; j < 2; j++) {
			if (pInfo->pVoice[j] != NULL) {
				pInfo->pVoice[j]->ReflectMasterVolume();
			}
		}
	}
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] != NULL) {
			m_pSeVoice[i]->ReflectMasterVolume();
		}
	}
}


//!	BGMリストの登録
/**
 *	サウンドマネージャにBGMリストを登録します。<br>
 *	以前に登録されていたリストを上書きします。<br>
 *
 *	@param pList		[in] BGMリストのポインタ
 *	@param nListNum		[in] リスト数
 */
void CSoundManager::SetBgmList(const SOUND_LIST* pList, UINT32 nListNum)
{
	ReleaseBgmData();
	m_pBgmList     = pList;
	m_dwBgmListNum = nListNum;
}


void CSoundManager::ReleaseBgmData(void)
{
	m_iNextPlayBgmSlot = -1;
	for (INT32 i = 0; i < cnSndMgr_BgmSlotNum; i++) {
		delete m_stBgmInfo[i].pVoice[0];
		delete m_stBgmInfo[i].pVoice[1];
		m_stBgmInfo[i].pVoice[0]        = NULL;
		m_stBgmInfo[i].pVoice[1]        = NULL;
		m_stBgmInfo[i].dwPlayingBgmNum  = 0;
		m_stBgmInfo[i].dwNextPlayBgmNum = 0;
	}

	UINT32 nListNum = m_dwBgmListNum;
	if (nListNum != 0) {
		const	SOUND_LIST* pList = m_pBgmList;
		for (UINT32 i = 0; i < nListNum; i++, pList++) {
			if (pList->bOnMemory == true) {
				ReleaseDataPtr(pList->szFileName);
			}
		}
	}
}


//!	BGMナンバーの設定
/**
 *	指定したスロットにＢＧＭナンバーを設定します。<br>
 *	フェードタイムは、実際に再生・停止を行う際に、デフォルトを指定された場合に使用されます。<br>
 *	再生・停止時に明確な指定を行われた場合は、そちらが優先されます。<br>
 *
 *	@param nSlot		[in] BGMスロット
 *	@param dwBgmNum		[in] BGMナンバー
 *	@param nVolume		[in] ボリューム
 *	@param nFadeInTick	[in] このBGMが再生する際のフェードインタイム
 *	@param nFadeOutTick	[in] このBGMが停止する際のフェードアウトタイム
 */
void CSoundManager::SetBgm(UINT32 nSlot, UINT32 dwBgmNum, INT32 nVolume, INT32 nFadeInTick, INT32 nFadeOutTick)
{
	if (nSlot < cnSndMgr_BgmSlotNum) {
		SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
		if (pInfo->dwPlayingBgmNum != dwBgmNum && pInfo->dwNextPlayBgmNum != dwBgmNum) {
			pInfo->dwNextPlayBgmNum = dwBgmNum;
			pInfo->nVolume          = nVolume;
			pInfo->nFadeInTick      = nFadeInTick;
			pInfo->nFadeOutTick[1]  = nFadeOutTick;
		}
	}
}


//!	BGMの再生
/**
 *	BGMの再生を行います。<br>
 *
 *	@param nSlot		[in] BGMスロット
 *	@param nVolume		[in] ボリューム(マイナスの場合は指定なし)
 *	@param nFadeInTick	[in] 再生する際のフェードインタイム(マイナスの場合は指定なし)
 */
void CSoundManager::PlayBgm(UINT32 nSlot, INT32 nVolume, INT32 nFadeInTick)
{
	if (nSlot >= cnSndMgr_BgmSlotNum)		return;
	if (m_iNowPlayingBgmSlot == nSlot)		return;

	SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
	if (nVolume >= 0) {
		pInfo->nVolume = nVolume;
	}
	if (nFadeInTick >= 0) {
		pInfo->nFadeInTick = nFadeInTick;
	}
	m_iNextPlayBgmSlot = nSlot;
}


//!	BGMの停止
/**
 *	BGMの停止を行います。<br>
 *
 *	@param nSlot		[in] BGMスロット
 *	@param nFadeOutTick	[in] フェードアウトタイム
 */
void CSoundManager::StopBgm(UINT32 nSlot, INT32 nFadeOutTick)
{
	if (nSlot >= cnSndMgr_BgmSlotNum)		return;

	SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
	pInfo->bPause = false;
	if (m_iNextPlayBgmSlot == nSlot) {
		m_iNextPlayBgmSlot = -1;
		return;
	}
	if (m_iNowPlayingBgmSlot == nSlot) {
		if (nFadeOutTick < 0) {
			nFadeOutTick = pInfo->nFadeOutTick[0];
		}
		if (pInfo->pVoice[1] != NULL) {
			pInfo->pVoice[1]->Stop(nFadeOutTick);
		}
		if (pInfo->pVoice[0] != NULL) {
			if (pInfo->pVoice[1] != NULL) {
				delete pInfo->pVoice[0];
				pInfo->pVoice[0] = pInfo->pVoice[1];
				pInfo->pVoice[1] = NULL;
			} else {
				pInfo->pVoice[0]->Stop(nFadeOutTick);
			}
		}
		pInfo->dwNextPlayBgmNum =  0;
		m_iNowPlayingBgmSlot    = -1;
	}
}


//!	BGMの一時停止・再開
/**
 *	BGMの一時停止および再開を行います。<br>
 *
 *	@param bPause		[in] 一時停止する場合はtrue
 *	@param nSlot		[in] BGMスロット(マイナスで再生中スロットを選択)
 *	@param nFadeTick	[in] フェードタイム
 */
void CSoundManager::PauseBgm(bool bPause, INT32 nSlot, INT32 nFadeTick)
{
	if (nSlot >= cnSndMgr_BgmSlotNum) {
		return;
	}
	if (nSlot < 0) {
		nSlot = m_iNowPlayingBgmSlot;
	} else if (m_iNowPlayingBgmSlot != nSlot) {
		return;
	}
	if (nSlot == -1)		return;
	SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
	if (nFadeTick < 0) {
		if (bPause == false) {
			nFadeTick = pInfo->nFadeInTick;
		} else {
			nFadeTick = pInfo->nFadeOutTick[0];
		}
	}
	if (pInfo->pVoice[1] != NULL) {
		pInfo->pVoice[1]->Pause(bPause, nFadeTick);
	} else if (pInfo->pVoice[0] != NULL) {
		pInfo->pVoice[0]->Pause(bPause, nFadeTick);
	}
	pInfo->bPause = bPause;
}


//!	BGMが一時停止かどうか
/**
 *	指定したスロットのBGMが一時停止中かどうかを取得します。<br>
 *
 *	@param nSlot		[in] BGMスロット(マイナスで再生中スロットを選択)
 *
 *	@return		一時停止中の場合はtrueを返す
 */
bool CSoundManager::IsPauseBgm(INT32 nSlot)	const
{
	if (nSlot < 0) {
		nSlot = m_iNowPlayingBgmSlot;
	}
	if (nSlot < 0 || nSlot >= cnSndMgr_BgmSlotNum) {
		return false;
	}

	return m_stBgmInfo[nSlot].bPause;
}


//! BGMボリュームの取得
INT32 CSoundManager::GetBgmVolume(UINT32 nSlot)	const
{
	if (nSlot >= cnSndMgr_BgmSlotNum) {
		return 0;
	}

	const	SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
	if (pInfo->pVoice[1] != NULL) {
		return pInfo->pVoice[1]->GetVolume();
	} else if (pInfo->pVoice[0] != NULL) {
		return pInfo->pVoice[0]->GetVolume();
	}

	return pInfo->nVolume;
}


//! BGMボリュームの設定
void CSoundManager::SetBgmVolume(UINT32 nSlot, INT32 nVolume, INT32 nFadeTick, INT32 nStartVolume)
{
	if (nSlot >= cnSndMgr_BgmSlotNum) {
		return;
	}

	SNDMGR_BGMINFO*		pInfo = &m_stBgmInfo[nSlot];
	if (pInfo->pVoice[1] != NULL) {
		pInfo->pVoice[1]->SetVolume(nVolume, nFadeTick, nStartVolume);
	} else if (pInfo->pVoice[0] != NULL) {
		pInfo->pVoice[0]->SetVolume(nVolume, nFadeTick, nStartVolume);
	}
}


//!	SEリストの登録
/**
 *	サウンドマネージャにSEリストを登録します。<br>
 *	以前に登録されていたリストを上書きします。<br>
 *
 *	@param nSlot		[in] SEスロット
 *	@param pList		[in] SEリストのポインタ
 *	@param nListNum		[in] リスト数
 */
void CSoundManager::SetSeList(UINT32 nSlot, const SOUND_LIST* pList, UINT32 nListNum)
{
	if (nSlot >= cnSndMgr_SeListSlotNum) {
		return;
	}
	ReleaseSeData(nSlot);
	m_pSeList[nSlot]     = pList;
	m_dwSeListNum[nSlot] = nListNum;
}


void CSoundManager::ReleaseSeData(UINT32 nSlot)
{
	UINT32 nListNum = m_dwSeListNum[nSlot];
	StopSeNum(nSlot);
	if (nListNum != 0) {
		const	SOUND_LIST* pList = m_pSeList[nSlot];
		nSlot <<= 24;
		for (UINT32 i = 0; i < nListNum; i++, pList++) {
			if (pList->bOnMemory == true) {
				ReleaseDataPtr(pList->szFileName);
			}
		}
	}
}


//! 識別IDの取得
/**
 *	@param	nSlot		[in] SEスロット
 *	@param	nUnique		[out] 識別ID
 *
 *	@return	正しく取得出来ればtrueを返す
 */
bool CSoundManager::GetUnique(UINT32 nSlot, UINT32& nUnique)	const
{
	if (nSlot >= cnSndMgr_MaxSeNum)		return false;
	if (m_pSeVoice[nSlot] == NULL)		return false;

	nUnique = m_pSeVoice[nSlot]->GetUnique();

	return true;
}


//! 指定した識別IDのSEが再生中かどうか
/**
 *	@param	nSlot		[in] SEスロット
 *
 *	@return	再生していればtrueを返す
 */
bool CSoundManager::IsPlayingBySlot(UINT32 nSlot)	const
{
	if (nSlot >= cnSndMgr_MaxSeNum)		return false;
	if (m_pSeVoice[nSlot] == NULL)		return false;

	return m_pSeVoice[nSlot]->IsPlaying();
}


//! 指定した識別IDのSEが再生中かどうか
/**
 *	@param	nUnique		[in] 識別ID
 *	@param	nUniqueMask	[in] 識別IDマスク
 *
 *	@return	１つでも再生していればtrueを返す
 */
bool CSoundManager::IsPlayingByUnique(UINT32 nUnique, UINT32 nUniqueMask)	const
{
	nUnique &= nUniqueMask;
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)		continue;
		if ((m_pSeVoice[i]->GetUnique() & nUniqueMask) == nUnique && m_pSeVoice[i]->IsPlaying() == true) {
			return true;
		}
	}

	return false;
}


//! 指定したSEナンバーのSEが再生中かどうか
/**
 *	@param	nSlot	[in] SEスロットナンバー
 *	@param	nSeNum	[in] SEナンバー(指定しない場合は0xffffffff)
 *
 *	@return	１つでも再生していればtrueを返す
 */
bool CSoundManager::IsPlayingBySeNum(UINT32 nSlot, UINT32 nSeNum)	const
{
	if (nSlot >= cnSndMgr_SeListSlotNum) {
		return 0;
	}

	nSlot <<= 24;
	if (nSeNum != 0xffffffff) {
		nSlot |= nSeNum;
	}
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] != NULL) {
			UINT32	nNum = m_pSeVoice[i]->GetNumber();
			if (nSeNum == 0xffffffff) {
				nNum &= 0xff000000;
			}
			if (nNum == nSlot && m_pSeVoice[i]->IsPlaying()) {
				return true;
			}
		}
	}

	return false;
}


//!	SEの再生
/**
 *	SEの再生を行います。<br>
 *	以後の操作は基本的には識別IDによって行います。<br>
 *
 *	@param nSeSlot		[in] SEスロットナンバー
 *	@param nSeNum		[in] SEナンバー(0～0xffffff)
 *	@param nUnique		[in] 識別ID(32Bit)
 *	@param nVolume		[in] ボリューム
 *	@param nFadeInTick	[in] フェードインタイム
 */
bool CSoundManager::PlaySe(UINT32 nSeSlot, UINT32 nSeNum, UINT32 nUnique, INT32 nVolume, INT32 nFadeInTick)
{
	if (nSeSlot >= cnSndMgr_SeListSlotNum) {
		return false;
	}

	nSeNum &= 0x00ffffff;
	const	SOUND_LIST*		pList = GetListData(m_pSeList[nSeSlot], m_dwSeListNum[nSeSlot], nSeNum);
	if (pList == NULL) {
		return false;
	}

	CVoiceElement*	pVoice = NULL;
	INT32			nSlot;
	for (nSlot = 0; nSlot < cnSndMgr_MaxSeNum; nSlot++) {
		if (m_pSeVoice[nSlot] == NULL) {
			pVoice = CreateVoiceElement(pList->cWavType);
			break;
		}
	}
	if (pVoice == NULL) {
		return false;
	}

	if (SetData(pVoice, pList) == false) {
		delete pVoice;
		return false;
	}
	pVoice->SetNumber((nSeSlot << 24) | nSeNum);
	pVoice->SetUnique(nUnique);
	pVoice->Play(pList->nLoop, nVolume, nFadeInTick, pList->nLoopInSample);
	m_pSeVoice[nSlot] = pVoice;

	return true;
}


//!	SEの停止
/**
 *	SEの停止を行います。<br>
 *	該当する識別IDのSEが、全て停止します。<br>
 *
 *	@param nUnique		[in] 識別ID
 *	@param nUniqueMask	[in] 識別IDマスク
 *	@param nFadeOutTick	[in] フェードアウトタイム
 */
void CSoundManager::StopSe(UINT32 nUnique, UINT32 nUniqueMask, INT32 nFadeOutTick)
{
	nUnique &= nUniqueMask;

	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] != NULL) {
			if ((m_pSeVoice[i]->GetUnique() & nUniqueMask) == nUnique) {
				m_pSeVoice[i]->Stop(nFadeOutTick);
			}
		}
	}
}


//!	SEの停止
/**
 *	SEの停止を行います。<br>
 *	該当するSEナンバーのSEが、全て停止します。<br>
 *
 *	@param nSlot		[in] SEスロット
 *	@param nSeNum		[in] SEナンバー
 *	@param nFadeOutTick	[in] フェードアウトタイム
 */
void CSoundManager::StopSeNum(UINT32 nSlot, UINT32 nSeNum, INT32 nFadeOutTick)
{
	if (nSlot >= cnSndMgr_SeListSlotNum) {
		return;
	}

	nSlot <<= 24;
	if (nSeNum != 0xffffffff) {
		nSlot |= nSeNum;
	}
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] != NULL) {
			UINT32	nNum = m_pSeVoice[i]->GetNumber();
			if (nSeNum == 0xffffffff) {
				nNum &= 0xff000000;
			}
			if (nNum == nSlot) {
				if (nFadeOutTick == 0) {
					delete m_pSeVoice[i];
					m_pSeVoice[i] = NULL;
				} else {
					m_pSeVoice[i]->Stop(nFadeOutTick);
				}
			}
		}
	}
}


//!	SEの一時停止・再開
/**
 *	SEの一時停止および再開を行います。<br>
 *	該当する識別IDのSEが、全て反映します。<br>
 *
 *	@param bPause		[in] ポーズする場合はtrue
 *	@param nUnique		[in] 識別ID
 *	@param nUniqueMask	[in] 識別IDマスク
 *	@param nFadeTick	[in] フェードタイム
 */
void CSoundManager::PauseSe(bool bPause, UINT32 nUnique, UINT32 nUniqueMask, INT32 nFadeTick)
{
	nUnique &= nUniqueMask;

	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] != NULL) {
			if ((m_pSeVoice[i]->GetUnique() & nUniqueMask) == nUnique) {
				m_pSeVoice[i]->Pause(bPause, nFadeTick);
			}
		}
	}
}


//!	SEが一時停止中かどうか
/**
 *	該当する識別IDのSEが一時停止中かどうかを取得します。<br>
 *
 *	@param nUnique		[in] 識別ID
 *
 *	@return		停止中の場合はtrueを返す
 */
bool CSoundManager::IsPauseSe(UINT32 nUnique)	const
{
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)	continue;
		if (m_pSeVoice[i]->GetUnique() == nUnique) {
			return m_pSeVoice[i]->IsPause();
		}
	}

	return false;
}


//! SEボリュームの取得
/**
 *	@param nUnique		[in] 識別ID
 *
 *	@return		ボリューム
 */
INT32 CSoundManager::GetSeVolume(UINT32 nUnique)	const
{
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)	continue;
		if (m_pSeVoice[i]->GetUnique() == nUnique) {
			return m_pSeVoice[i]->GetVolume();
		}
	}

	return 0;
}


//! SEボリュームの設定
/**
 *	@param nVolume		[in] ボリューム
 *	@param nUnique		[in] 識別ID
 *	@param nUniqueMask	[in] 識別IDマスク
 *	@param nTick		[in] 遷移時間
 */
void CSoundManager::SetSeVolume(INT32 nVolume, UINT32 nUnique, UINT32 nUniqueMask, INT32 nTick)
{
	nUnique &= nUniqueMask;

	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)	continue;
		if ((m_pSeVoice[i]->GetUnique() & nUniqueMask) == nUnique) {
			m_pSeVoice[i]->SetVolume(nVolume, nTick);
		}
	}
}


//! SEパンの取得
/**
 *	@param nUnique		[in] 識別ID
 *
 *	@return		パンポット
 */
INT32 CSoundManager::GetSePanpot(UINT32 nUnique)	const
{
	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)	continue;
		if (m_pSeVoice[i]->GetUnique() == nUnique) {
			return m_pSeVoice[i]->GetPan();
		}
	}

	return 0;
}


//! SEパンの設定
/**
 *	@param nVolume		[in] パン
 *	@param nUnique		[in] 識別ID
 *	@param nUniqueMask	[in] 識別IDマスク
 *	@param nTick		[in] 遷移時間
 */
void CSoundManager::SetSePanpot(INT32 nPanpot, UINT32 nUnique, UINT32 nUniqueMask, INT32 nTick)
{
	nUnique &= nUniqueMask;

	for (INT32 i = 0; i < cnSndMgr_MaxSeNum; i++) {
		if (m_pSeVoice[i] == NULL)	continue;
		if ((m_pSeVoice[i]->GetUnique() & nUniqueMask) == nUnique) {
			m_pSeVoice[i]->SetPan(nPanpot, nTick);
		}
	}
}


CSoundManager& CSoundManager::GetInstance(void)
{
	static	CSoundManager	gsSoundManager;

	return gsSoundManager;
}


/*  Bottom of SoundManager.cpp  */
