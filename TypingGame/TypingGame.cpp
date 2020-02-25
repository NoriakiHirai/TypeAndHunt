// TypingGame.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "TypingGame.h"
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <stdio.h>
#include <atlstr.h>
#include <iostream>
#include <regex>
#include <string>
#include <strstream>
#include <chrono>
#include <vector>
#include "render.h"
#include "utility.h"
#include "Font.h"
#include "HP.h"
#include "Time.h"
#include "Effect.h"
#include "BoltMotion.h"
#include "ImpactMotion.h"
#include "ReductionMotion.h"
#include "Bolt.h"
#include "Impact.h"

#pragma comment(lib, "winmm.lib")

// シンボル定義及びマクロ
#define WINDOW_WIDTH    1024
#define WINDOW_HEIGHT   768
#define MAX_LOADSTRING 100

enum GAME_PHASE
{
	NONE = 0,
	TITLE,
	LEVELDISP,
	PLAY,
	RESULT,
};

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

//文字列描画用配列
std::vector<std::string> currentProbremSet;
std::string probrems[10];
std::string probrems2[10];
std::string probrems3[10];
char input[61];

// 問題の番号
int probremNum = 0;
const int PROBREM_MAX = 10;
const int HP_MIN = 0;

const float SIGN_INIT_POS_X = WINDOW_WIDTH * 4 / 10;
const float SIGN_INIT_POS_Y = WINDOW_HEIGHT * 3 / 10;

// 背景
HBITMAP g_hbmpBG;	// ビットマップオブジェクト
HDC		g_hMdcBG;	// メモリーデバイスコンテキスト（BMPオブジェクトを格納する）
BITMAP	g_BitmapBG;	// ビットマップの情報

// フォント
HBITMAP g_hbmpString;	// ビットマップオブジェクト
HDC		g_hMdcString;	// メモリーデバイスコンテキスト（BMPオブジェクトを格納する）
BITMAP	g_BitmapString;	// ビットマップの情報
// フォント(赤）
HBITMAP g_hbmpRedString;	// ビットマップオブジェクト
HDC		g_hMdcRedString;	// メモリーデバイスコンテキスト（BMPオブジェクトを格納する）
BITMAP	g_BitmapRedString;	// ビットマップの情報

// 稲妻
HBITMAP	g_hbmpBolt; // ビットマップオブジェクト
HDC		g_hMdcBolt; // メモリーデバイスコンテキスト
BITMAP  g_BitmapBolt; // ビットマップの情報

// 骸骨
HBITMAP	g_hbmpSkull;
HDC		g_hMdcSkull;
BITMAP  g_BitmapSkull;

HBITMAP	g_hbmpSkull2;
HDC		g_hMdcSkull2;
BITMAP  g_BitmapSkull2;

HBITMAP	g_hbmpSkull3;
HDC		g_hMdcSkull3;
BITMAP  g_BitmapSkull3;

// ゲージ
HBITMAP	g_hbmpGauge;
HDC		g_hMdcGauge;
BITMAP  g_BitmapGauge;

// ダメージエフェクト
HBITMAP	g_hbmpSpike;
HDC		g_hMdcSpike;
BITMAP  g_BitmapSpike;

// メッセージ
HBITMAP	g_hbmpMsg;
HDC		g_hMdcMsg;
BITMAP  g_BitmapMsg;

HBITMAP	g_hbmpSign;
HDC		g_hMdcSign;
BITMAP  g_BitmapSign;

Sample* Bgo;
Sample* Skull;
Sample* Msg;
Effect* Sign;
Bolt* g_Bolt;
Impact* SpikyEffect;
HP* Hp;
Time* Timer;
Font* ProbremStr;
Font* InputStr;

// エフェクトモーション
BoltMotion* BoltMot;
ImpactMotion* ImpactMot;
ReductionMotion* ReducMot;

// ダブルバッファリング用変数
HBITMAP g_hBitmap;		// バックバッファ用ビットマップ
HDC		g_hMdcBitmap;	// バックバッファ用メモリーDC

// ゲームの進捗
GAME_PHASE Phase;
int level = 1;
int Winner;
std::chrono::system_clock::time_point g_WaitStart;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void Init(HWND hWnd);
void Update();
void Draw(HDC hdc);
bool CheckInput(int num);
void Finalize();

void Title();
void Play();
void PrepareMessage(int winner);
void ResetInput();
void PrepareNextLevel();
void GameEnd();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TYPINGGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

	hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TYPINGGAME));

    MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // メッセージを取得
		{
			if (msg.message == WM_QUIT)
			{
				// 終了メッセージだったらループ終了
				break;
			}
			else
			{
				// メッセージの変換
				TranslateMessage(&msg);
				// コールバック関数の呼び出しリクエストなど
				DispatchMessage(&msg);
			}
		}

		Update();
		// WM_PAINT強制要求
		InvalidateRect(hWnd, nullptr, FALSE);
	}

    return (int) msg.wParam;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TYPINGGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TYPINGGAME);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	std::ostrstream oss;
	char t;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
			case IDM_HOWTOCONTROL:
				oss << "表示された単語を打ち込みエンターキーを押します。\n";
				oss << "打ち込んだ単語が合っていれば、敵にダメージを与えられます。\n";
				oss << "間違って入力した文字は赤色で表示されます。\n";
				oss << std::ends;
				if (MessageBox(hWnd, CString(oss.str()), CString("遊び方"), MB_OK) == IDOK) {
					return 0;
				}
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			// クライアント領域の描画
			Draw(g_hMdcBitmap);
			// バックバッファをクライアント領域に転送
			RECT rc;
			// クライアント領域のサイズを取得
			GetClientRect(hWnd, &rc);
			// メモリーDCをクライアントに転送
			BitBlt(hdc, 0, 0, rc.right, rc.bottom, g_hMdcBitmap, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_KEYDOWN:
		switch (wParam)
		{
			//  エスケープキーの場合
		case VK_ESCAPE:
			//終了メッセージを発生させる
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
			//  スペースキーの場合
		case VK_SPACE:
			break;
		case VK_BACK:
		{
			int i = 0;
			while (input[i] != '\0')  ++i;
			if (i > 0) {
				input[i - 1] = '\0';
			}
			break;
		}
		case VK_RETURN:
			switch (Phase)
			{
			case NONE:
				break;
			case TITLE:
				Phase = GAME_PHASE::LEVELDISP;
				Msg->SetUse(false);
				Sign->Start();
				Sign->SetRect(RECT{ 0, 224, 128, 62 });
				break;
			case LEVELDISP:
				break;
			case PLAY:
				if (CheckInput(probremNum)) {
					g_Bolt->Start();
					// インプット文字列の初期化
					ResetInput();
					if (probremNum == PROBREM_MAX - 1) {
						Phase = GAME_PHASE::RESULT;
						PrepareMessage(1);
					}
					else {
						++probremNum;
						Timer->Start();
					}
				}
				break;
			case RESULT:
				++level;
				if (level > 3) {
					GameEnd();
					Phase = GAME_PHASE::TITLE;
					Msg->SetUse(true);
				}
				else {
					Phase = GAME_PHASE::LEVELDISP;
					Msg->SetUse(false);
				}
				PrepareNextLevel();
				break;
			default:
				break;
			}
			break;
		default:
			switch (Phase)
			{
			case NONE:
				break;
			case TITLE:
				break;
			case PLAY:
				t = wParam;
				if (std::regex_search(std::string{ t }, std::regex("[A-Z]"))) {
					size_t i = 0;
					for (; input[i] != '\0'; i++) {}
					if (i < 10) {
						input[i] = t;
						input[i + 1] = '\0';
					}
				}
				break;
			case RESULT:
				break;
			default:
				break;
			}
			break;
		}
		//再描画メッセージを発生させる
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
    case WM_CREATE:
        Init(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Init(HWND hWnd)
{
	// DC取得
	HDC hdc = GetDC(hWnd);

	// ビットマップ画像の読み込み
	// 背景
	g_hbmpBG = (HBITMAP)LoadImage(nullptr, CString("data\\sample.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	// ビットマップをg_hMdcBGへセット
	g_hMdcBG = CreateCompatibleDC(hdc);
	SelectObject(g_hMdcBG, g_hbmpBG);
	// g_hbmpBGビットマップの情報を取得
	GetObject(g_hbmpBG, sizeof(g_BitmapBG), &g_BitmapBG);

	// 文字列用テクスチャ
	g_hbmpString = (HBITMAP)LoadImage(nullptr, CString("data\\font.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	g_hMdcString = CreateCompatibleDC(hdc);
	SelectObject(g_hMdcString, g_hbmpString);
	GetObject(g_hbmpString, sizeof(g_BitmapString), &g_BitmapString);

	g_hbmpRedString = (HBITMAP)LoadImage(nullptr, CString("data\\font_red.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	g_hMdcRedString = CreateCompatibleDC(hdc);
	SelectObject(g_hMdcRedString, g_hbmpRedString);
	GetObject(g_hbmpRedString, sizeof(g_BitmapRedString), &g_BitmapRedString);

	// 稲妻
	g_hbmpBolt = (HBITMAP)LoadImage(nullptr, CString("data\\bolt.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	g_hMdcBolt = CreateCompatibleDC(hdc);
	SelectObject(g_hMdcBolt, g_hbmpBolt);
	GetObject(g_hbmpBolt, sizeof(g_BitmapBolt), &g_BitmapBolt);

	// 骸骨
	g_hbmpSkull = (HBITMAP)LoadImage(nullptr, CString("data\\bone.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	g_hMdcSkull = CreateCompatibleDC(hdc);
	SelectObject(g_hMdcSkull, g_hbmpSkull);
	GetObject(g_hbmpSkull, sizeof(g_BitmapSkull), &g_BitmapSkull);

	g_hbmpSkull2 = (HBITMAP)LoadImage(nullptr, CString("data\\bone.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	g_hMdcSkull2 = CreateCompatibleDC(hdc);
	SelectObject(g_hMdcSkull2, g_hbmpSkull2);
	GetObject(g_hbmpSkull2, sizeof(g_BitmapSkull2), &g_BitmapSkull2);

	g_hbmpSkull3 = (HBITMAP)LoadImage(nullptr, CString("data\\bone.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	g_hMdcSkull3 = CreateCompatibleDC(hdc);
	SelectObject(g_hMdcSkull3, g_hbmpSkull3);
	GetObject(g_hbmpSkull3, sizeof(g_BitmapSkull3), &g_BitmapSkull3);
	// ゲージ
	g_hbmpGauge = (HBITMAP)LoadImage(nullptr, CString("data\\gauge.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	g_hMdcGauge = CreateCompatibleDC(hdc);
	SelectObject(g_hMdcGauge, g_hbmpGauge);
	GetObject(g_hbmpGauge, sizeof(g_BitmapGauge), &g_BitmapGauge);

	// ダメージエフェクト
	g_hbmpSpike = (HBITMAP)LoadImage(nullptr, CString("data\\spikyEffect.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	g_hMdcSpike = CreateCompatibleDC(hdc);
	SelectObject(g_hMdcSpike, g_hbmpSpike);
	GetObject(g_hbmpSpike, sizeof(g_BitmapSpike), &g_BitmapSpike);

	// メッセージ
	g_hbmpMsg = (HBITMAP)LoadImage(nullptr, CString("data\\message.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	g_hMdcMsg = CreateCompatibleDC(hdc);
	SelectObject(g_hMdcMsg, g_hbmpMsg);
	GetObject(g_hbmpMsg, sizeof(g_BitmapMsg), &g_BitmapMsg);

	g_hbmpSign = (HBITMAP)LoadImage(nullptr, CString("data\\sign.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	g_hMdcSign = CreateCompatibleDC(hdc);
	SelectObject(g_hMdcSign, g_hbmpSign);
	GetObject(g_hbmpSign, sizeof(g_BitmapSign), &g_BitmapSign);

	// ダブルバッファを作成
	RECT rc;
	GetClientRect(hWnd, &rc);
	// 画像の格納領域を作成
	g_hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	// ウィンドウと同じ仕様のメモリーDCを作成
	g_hMdcBitmap = CreateCompatibleDC(hdc);
	// メモリーDCにビットマップを割り付け
	SelectObject(g_hMdcBitmap, g_hBitmap);

	// DC返却
	ReleaseDC(hWnd, hdc);

	// 背景やキャラクタなどの作成
	RECT recBG = { 0, 0, g_BitmapBG.bmWidth, g_BitmapBG.bmHeight };
	RECT recFont = { 0, 0, g_BitmapString.bmWidth, g_BitmapString.bmHeight };
	RECT recBolt = { 0, 0, g_BitmapBolt.bmWidth, g_BitmapBolt.bmHeight };
	RECT recSkull = { 0, 0, g_BitmapSkull.bmWidth, g_BitmapSkull.bmHeight };
	RECT recGauge = { 0, 0, g_BitmapGauge.bmWidth, g_BitmapGauge.bmHeight };
	RECT recSpike = { 0, 0, g_BitmapSpike.bmWidth, g_BitmapSpike.bmHeight };
	RECT recMsg = { 0, 0, g_BitmapMsg.bmWidth, g_BitmapMsg.bmHeight };
	RECT recSign = { 0, 0, g_BitmapSign.bmWidth, g_BitmapSign.bmHeight };

	Bgo = new Sample(g_hMdcBG, FALSE, recBG, 0, 0, 0, 0);

	int skullPosX = WINDOW_WIDTH * 4 / 10;
	int skullPosY = WINDOW_HEIGHT * 2 / 10;
	Skull = new Sample(g_hMdcSkull, TRUE, recSkull,
		skullPosX, skullPosY,
		0, 0, RGB(255, 255, 255)
	);
	Skull->SetSize(256, 256);

	// タイトル画面用の表示
	Msg = new Sample(g_hMdcMsg, TRUE, recMsg,
		(WINDOW_WIDTH * 2 / 10) + 50, WINDOW_HEIGHT * 2 / 10,
		0, 0
	);
	Msg->SetSize(512, 256);
	Msg->SetRect(RECT{ 0, 64, 128, 32 });
	Msg->SetColor(RGB(255, 255, 255));
	Msg->SetUse(true);

	Sign = new Effect(
		g_hMdcSign, TRUE, recSign,
		SIGN_INIT_POS_X, SIGN_INIT_POS_Y,
		0, 0
	);

	Sign->SetSize(256, 128);
	Sign->SetColor(RGB(0, 0, 0));
	ReducMot = new ReductionMotion(POINT{ 512, 256 });
	Sign->SetMotion(ReducMot);


	g_Bolt = new Bolt(
		g_hMdcBolt, TRUE, recBolt,
		float(WINDOW_WIDTH / 2 - 10), float(WINDOW_HEIGHT * 1 / 10 + 40),
		0, 0
	);
	g_Bolt->SetSize(64, 64);
	g_Bolt->SetColor(RGB(255, 255, 255));

	BoltMot = new BoltMotion();
	g_Bolt->SetMotion(BoltMot);

	SpikyEffect = new Impact(g_hMdcSpike, TRUE, recSpike,
		WINDOW_WIDTH / 2, WINDOW_HEIGHT * 2 / 5, 0, 0);
	SpikyEffect->SetSize(256, 256);

	ImpactMot = new ImpactMotion;
	SpikyEffect->SetMotion(ImpactMot);

	{
		int posX = skullPosX + 64;
		int posY = skullPosY + (256 + 50);

		ProbremStr = new Font(g_hMdcString, TRUE, recFont, posX, posY, 0, 0);
		ProbremStr->SetFontBackGround(g_hMdcGauge);

		posY += 74;
		InputStr = new Font(g_hMdcString, TRUE, recFont, posX, posY, 0, 0);
		InputStr->SetFontBackGround(g_hMdcGauge);
		InputStr->SetFontRed(g_hMdcRedString);

		posX = skullPosX;
		posY += 74;
		Hp = new HP(g_hMdcGauge, FALSE, recGauge,
			posX, posY, 0, 0,
			RGB(255, 255, 255)
		);
		Hp->SetSize(300, 32);

		posY += 52;
		Timer = new Time(g_hMdcGauge, FALSE, recGauge,
			posX, posY, 0, 0,
			RGB(255, 255, 255)
		);
		Timer->SetSize(300, 32);
	}

	// 問題文の初期化
	probrems[0] = "KIWI";
	probrems[1] = "GRAPE";
	probrems[2] = "MELON";
	probrems[3] = "LEMON";
	probrems[4] = "APPLE";
	probrems[5] = "BANANA";
	probrems[6] = "CHERRY";
	probrems[7] = "PINEAPPLE";
	probrems[8] = "STRAWBERRY";
	probrems[9] = "GRAPEFRUIT";

	probrems2[0] = "NAPOLEON";
	probrems2[1] = "SOCRATES";
	probrems2[2] = "AUGUSTUS";
	probrems2[3] = "ALEXANDER";
	probrems2[4] = "CLEOPATRA";
	probrems2[5] = "ELIZABETH";
	probrems2[6] = "ARISTOTLE";
	probrems2[7] = "BEETHOVEN";
	probrems2[8] = "EKATERINA";
	probrems2[9] = "DOSTOEVSKY";

	probrems3[0] = "VIRUS";
	probrems3[1] = "ASTHMA";
	probrems3[2] = "VACCINE";
	probrems3[3] = "ALLERGY";
	probrems3[4] = "MEDICINE";
	probrems3[5] = "HEADACHE";
	probrems3[6] = "FRACTURE";
	probrems3[7] = "PNEUMONIA";
	probrems3[8] = "ARTHRITIS";
	probrems3[9] = "ANESTHESIA";

	for (auto i = 0; i < PROBREM_MAX; ++i) {
		currentProbremSet.push_back(probrems[i]);
	}

	Phase = GAME_PHASE::TITLE;
}

/**
*	@param {num} 何番目の問題か
*/
bool CheckInput(int num)
{
	// 入力文字列と問題の文字列を比較する
	for (auto i = 0; currentProbremSet[probremNum][i] != '\0'; ++i) {
		if (input[i] != currentProbremSet[probremNum][i]) return false;
	}
	return true;
}

void Update()
{
	std::chrono::system_clock::time_point nowTime;
	double elapsed;
	switch (Phase)
	{
	case GAME_PHASE::TITLE:
		Sign->SetUse(true);
		break;
	case GAME_PHASE::LEVELDISP:
		Sign->Update();
		if (ReducMot->IsCompleted()) {
			Phase = GAME_PHASE::PLAY;
			Sign->SetUse(false);
			Skull->SetUse(true);
			Timer->Start();
		}
		break;
	case GAME_PHASE::PLAY:
		Play();
		break;
	case GAME_PHASE::RESULT:
		g_Bolt->Update();
		SpikyEffect->Update();
		nowTime = std::chrono::system_clock::now();
		elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - g_WaitStart).count();
		if (elapsed >= 1500) {
			if (Winner == 1) {
				Skull->SetUse(false);
			}
			Msg->SetUse(true);
		}
		break;
	default:
		break;
	}
}

void Draw(HDC hdc)
{
	Bgo->Draw(hdc, false);

	switch (Phase)
	{
	case GAME_PHASE::TITLE:
		Msg->Draw(hdc, TRUE);
		break;
	case GAME_PHASE::LEVELDISP:
		Sign->Draw(hdc);
		break;
	case GAME_PHASE::RESULT:
		Msg->Draw(hdc, TRUE);
	case GAME_PHASE::PLAY:
		Skull->Draw(hdc, TRUE);
		ProbremStr->Draw(hdc, &currentProbremSet[probremNum]);
		InputStr->Draw(hdc, &currentProbremSet[probremNum], input);
		Hp->Draw(hdc);
		Timer->Draw(hdc);
		g_Bolt->Draw(hdc);
		SpikyEffect->Draw(hdc);
		break;
	default:
		break;
	}
}

//===========================================================================
// 資源の解放処理
//===========================================================================
void Finalize()
{
	// ビットマップオブジェクトを解放する
	DeleteObject(g_hbmpBG);
	DeleteObject(g_hbmpString);
	DeleteObject(g_hbmpRedString);
	DeleteObject(g_hbmpBolt);
	DeleteObject(g_hbmpSkull);
	DeleteObject(g_hbmpSkull2);
	DeleteObject(g_hbmpSkull3);
	DeleteObject(g_hbmpGauge);
	DeleteObject(g_hbmpSpike);
	DeleteObject(g_hbmpMsg);
	DeleteObject(g_hbmpSign);
	DeleteObject(g_hBitmap);

	// 作成したメモリーデバイスコンテキストを開放する
	DeleteDC(g_hMdcBG);
	DeleteDC(g_hMdcString);
	DeleteDC(g_hMdcRedString);
	DeleteDC(g_hMdcBolt);
	DeleteDC(g_hMdcSkull);
	DeleteDC(g_hMdcSkull2);
	DeleteDC(g_hMdcSkull3);
	DeleteDC(g_hMdcGauge);
	DeleteDC(g_hMdcSpike);
	DeleteDC(g_hMdcMsg);
	DeleteDC(g_hMdcSign);
	DeleteDC(g_hMdcBitmap);

	SAFE_DELETE(Bgo);
	SAFE_DELETE(Skull);
	SAFE_DELETE(Msg);
	SAFE_DELETE(Sign);
	SAFE_DELETE(Hp);
	SAFE_DELETE(Timer);
	SAFE_DELETE(ImpactMot);
	SAFE_DELETE(BoltMot);
	SAFE_DELETE(ReducMot);
	SAFE_DELETE(SpikyEffect);
	SAFE_DELETE(g_Bolt);
	SAFE_DELETE(ProbremStr);
	SAFE_DELETE(InputStr);
}

void Title()
{
}

void Play()
{
	g_Bolt->Update();
	SpikyEffect->Update();

	// タイマーが0になっていないかチェック
	if (Timer->ResetCheck()) {
		// プレイヤーはダメージを受ける
		Hp->UpdateHP(-1);
		SpikyEffect->Start();
		if (Hp->GetHP() <= HP_MIN) {
			Phase = GAME_PHASE::RESULT;
			PrepareMessage(2);
		}
		else {
			Timer->Start();
		}
	}
	else {
		Timer->Update();
	}
}

void PrepareMessage(int winner)
{
	Msg->SetPosition(WINDOW_WIDTH * 4 / 10, WINDOW_HEIGHT * 1 / 10);
	Msg->SetSize(256, 64);
	if (winner == 1) {
		Msg->SetRect(RECT{ 0, 0, 128, 32 });
	}
	else {
		Msg->SetRect(RECT{ 0, 32, 128, 32 });
	}
	g_WaitStart = std::chrono::system_clock::now();
}

void ResetInput() {
	memset(input, 0, sizeof(input));
}

void PrepareNextLevel()
{
	// 問題集初期化
	probremNum = 0;
	if (currentProbremSet.size() > 0) {
		currentProbremSet.erase(
			currentProbremSet.begin(),
			currentProbremSet.end()
		);
	}
	RECT recSkull;
	Sign->SetPosition(SIGN_INIT_POS_X, SIGN_INIT_POS_Y);
	switch (level)
	{
	case 1:
		// レベル表示の切り替え
		Sign->SetRect(RECT{ 0, 224, 128, 62 });
		Sign->Start();
		for (auto i = 0; i < 10; ++i) {
			currentProbremSet.push_back(probrems[i]);
		}
		// 敵キャラ表示の切り替え
		Skull->SetHMDC(g_hMdcSkull);
		recSkull = { 0, 0, g_BitmapSkull.bmWidth, g_BitmapSkull.bmHeight };
		Skull->SetRect(recSkull);
		break;
	case 2:
		Sign->SetRect(RECT{ 0, 286, 128, 62 });
		Sign->Start();
		for (auto i = 0; i < 10; ++i) {
			currentProbremSet.push_back(probrems2[i]);
		}

		Skull->SetHMDC(g_hMdcSkull2);
		recSkull = { 0, 0, g_BitmapSkull2.bmWidth, g_BitmapSkull2.bmHeight };
		Skull->SetRect(recSkull);
		break;
	case 3:
		Sign->SetRect(RECT{ 0, 348, 128, 62 });
		Sign->Start();
		for (auto i = 0; i < 10; ++i) {
			currentProbremSet.push_back(probrems3[i]);
		}

		Skull->SetHMDC(g_hMdcSkull3);
		recSkull = { 0, 0, g_BitmapSkull3.bmWidth, g_BitmapSkull3.bmHeight };
		Skull->SetRect(recSkull);
		break;
	default:
		break;
	}
}

void GameEnd()
{
	// タイトル画面に戻る準備
	Msg->SetPosition((WINDOW_WIDTH * 2 / 10) + 50, WINDOW_HEIGHT * 2 / 10);
	Msg->SetSize(512, 256);
	Msg->SetRect(RECT{ 0, 64, 128,32 });

	Skull->SetUse(false);
	ProbremStr->SetUse(false);
	InputStr->SetUse(false);
	Sign->SetPosition(WINDOW_WIDTH * 4 / 10, WINDOW_HEIGHT * 3 / 10);
	Hp->ResetHP();

	probremNum = 0;
	ResetInput();

	Phase = GAME_PHASE::TITLE;
	level = 1;
}
