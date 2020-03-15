/**
 *	Ayameマネージャ
 */

#ifndef ___AYAMEMANAGER_H___
#define ___AYAMEMANAGER_H___


//====================================================================
// INCLUDE
//====================================================================
#include <windows.h>
#include "Ayame.h"
//#include <string>

#ifdef _UNICODE
	//#define	DEFAULT_AYAME_PATH	L("ayame.dll")
static TCHAR DEFAULT_AYAME_PATH[] = { _T("ayame.dll") };
#else
	#define	DEFAULT_AYAME_PATH	"ayame.dll"
#endif

//! Ayameマネージャ
class CAyameManager
{
  private:
	CAyameManager& operator = ( const CAyameManager& cls );

  protected:
	HINSTANCE						m_hAyame;
	AYAME_GETVERSION				m_pAyameGetVersion;
	AYAME_DIRECTSOUNDENUMERATE_PROC	m_pAyameDeviceEnumerate;
	AYAME_INITIALIZE_PROC			m_pAyameInitialize;
	AYAME_UNINITIALIZE_PROC			m_pAyameUninitialize;
	AYAME_GETDIRECTSOUNDCAPS_PROC	m_pAyameGetDirectSoundCaps;
	AYAME_CREATE_FROM_FILE_PROC		m_pAyameCreateFromFile;
	AYAME_CREATE_FROM_MEMORY_PROC	m_pAyameCreateFromMemory;
	AYAME_CREATE_FROM_URL_PROC		m_pAyameCreateFromUrl;
	AYAME_GETERROR_PROC				m_pAyameGetError;

  public:
	//! コンストラクタ
	CAyameManager() {
		m_hAyame = NULL;
		m_pAyameGetVersion = NULL;
		m_pAyameDeviceEnumerate = NULL;
		m_pAyameInitialize = NULL;
		m_pAyameUninitialize = NULL;
		m_pAyameGetDirectSoundCaps = NULL;
		m_pAyameCreateFromFile = NULL;
		m_pAyameCreateFromMemory = NULL;
		m_pAyameCreateFromUrl = NULL;
		m_pAyameGetError = NULL;
	}
	//! デストラクタ
	~CAyameManager() {
		Release();
	}

	//! DLLの読み込み
	bool LoadDLL( TCHAR* pszPath = DEFAULT_AYAME_PATH) {
		if ( m_hAyame != NULL )
		{
			return false;
		}
		// DLLのロード
		m_hAyame = ::LoadLibrary( pszPath );
		if ( m_hAyame == NULL ) return false;

		// 関数のロード
		m_pAyameGetVersion			= (AYAME_GETVERSION)::GetProcAddress( m_hAyame, "Ayame_GetVersion" );
		m_pAyameDeviceEnumerate     = (AYAME_DIRECTSOUNDENUMERATE_PROC)::GetProcAddress( m_hAyame, "Ayame_DirectSoundEnumerate" );
		m_pAyameInitialize			= (AYAME_INITIALIZE_PROC)::GetProcAddress( m_hAyame, "Ayame_Initialize" );
		m_pAyameUninitialize		= (AYAME_UNINITIALIZE_PROC)::GetProcAddress( m_hAyame, "Ayame_Uninitialize" );
		m_pAyameGetDirectSoundCaps	= (AYAME_GETDIRECTSOUNDCAPS_PROC)::GetProcAddress( m_hAyame, "Ayame_GetDirectSoundCaps" );
		m_pAyameCreateFromFile		= (AYAME_CREATE_FROM_FILE_PROC)::GetProcAddress( m_hAyame, "Ayame_CreateInstanceFromFile" );
		m_pAyameCreateFromMemory	= (AYAME_CREATE_FROM_MEMORY_PROC)::GetProcAddress( m_hAyame, "Ayame_CreateInstanceFromMemory" );
		m_pAyameCreateFromUrl       = (AYAME_CREATE_FROM_URL_PROC)::GetProcAddress( m_hAyame, "Ayame_CreateInstanceFromUrl" );
		m_pAyameGetError			= (AYAME_GETERROR_PROC)::GetProcAddress( m_hAyame, "Ayame_GetLastError" );

		if ( m_pAyameGetVersion			== NULL ||
			 m_pAyameDeviceEnumerate	== NULL ||
			 m_pAyameInitialize			== NULL ||
			 m_pAyameUninitialize		== NULL ||
			 m_pAyameGetDirectSoundCaps	== NULL ||
			 m_pAyameCreateFromFile		== NULL ||
			 m_pAyameCreateFromMemory	== NULL ||
			 m_pAyameCreateFromUrl		== NULL ||
			 m_pAyameGetError			== NULL )
		{
			::FreeLibrary(m_hAyame);
			m_hAyame = NULL;
			return false;
		}

		return true;
	}

	//! DLLの開放
	void Release() {
		if ( m_hAyame != NULL )
		{
			m_pAyameUninitialize();
			::FreeLibrary(m_hAyame);
			m_hAyame = NULL;
		}
	}

	// バージョンの取得
	DWORD GetVersion( void )
	{
		return (m_hAyame == NULL)? 0 : m_pAyameGetVersion();
	}

	// デバイスの列挙
	void SoundDeviceEnumerate( LPDSENUMCALLBACK callback, LPVOID  lpContext )
	{
		if (m_hAyame != NULL)
			m_pAyameDeviceEnumerate(callback, lpContext);
	}

	// ドライバーの初期化
	bool Initialize( HWND hWnd, void **ppDirectSound = NULL, LPCGUID lpcGuid = NULL )
	{
		return m_pAyameInitialize( hWnd, ppDirectSound, lpcGuid );
	}

	// ドライバーの解放
	void Uninitialize( void )
	{
		m_pAyameUninitialize();
	}

	bool GetDirectSoundCaps( DSCAPS *pCaps )
	{
		if ( m_hAyame == NULL || pCaps == NULL )
			return false;
		return m_pAyameGetDirectSoundCaps( pCaps );
	}

	CAyame *CreateInstanceFromFile( const TCHAR *pFileName, unsigned long Start = 0, unsigned long Size = 0, unsigned long Flag = AYAME_LOADFLAG_GLOBAL )
	{
		if ( m_hAyame == NULL || pFileName == NULL )
			return NULL;
		return m_pAyameCreateFromFile( pFileName, Start, Size, Flag );
	}

	CAyame *CreateInstanceFromMemory( const void *pData, unsigned long Size = 0, unsigned long Flag = AYAME_LOADFLAG_GLOBAL )
	{
		if ( m_hAyame == NULL || pData == NULL )
			return NULL;
		return m_pAyameCreateFromMemory( pData, Size, Flag );
	}

	CAyame *CreateInstanceFromUrl( const TCHAR *pUrl, unsigned long Start = 0, unsigned long Size = 0, unsigned long Flag = AYAME_LOADFLAG_GLOBAL )
	{
		if ( m_hAyame == NULL || pUrl == NULL )
			return NULL;
		return m_pAyameCreateFromUrl( pUrl, Start, Size, Flag );
	}

	bool GetLastError( TCHAR *pErrStr, unsigned long Size )
	{
		if ( m_hAyame == NULL )
			return false;
		return m_pAyameGetError( pErrStr, Size );
	}
};

#endif // ___AYAMEMANAGER_H___


