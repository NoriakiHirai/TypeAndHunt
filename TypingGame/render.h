#pragma once
#include <Windows.h>
#include <math.h>
#include <wingdi.h>
#include <string>

#pragma comment (lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

/**
*   キャラクタのスーパークラス
*/
class Object
{
public:
protected:
	BOOL used;					// 使用中フラグ
	BOOL isTransparent;			// 透過処理の有無
	HDC hMdc;					// 画像を表すDC
	RECT rect;					// 画像内のキャラクタの左上とサイズ
	COLORREF color;				// 透過色
	float positionX, positionY;	// キャラクタの表示座標
	float speedX, speedY;		// キャラクタの速度
	POINT size;	// サイズ

public:
	Object() { Init(); }
	virtual ~Object() {}

	void Init();

	// アクセサ
	void SetTransparent(BOOL trs) { isTransparent = trs; }
	void SetUse(BOOL use) { used = use; }
	void SetColor(COLORREF col) { color = col; }
	void SetRect(RECT r) { rect = r; }
	void SetPosition(float x, float y) { positionX = x; positionY = y; }
	void SetSpeed(float x, float y) { speedX = x; speedY = y; }
	void SetSize(int w, int h) {
		size.x = w;
		size.y = h;
	}
	virtual void Move(float movementX, float movementY);

	// アップデート
	virtual void Update() = 0;
	virtual void Draw(HDC hdc, BOOL transparent);
};

/**
*	サンプルキャラクタオブジェクト
*/
class Sample : public Object
{
private:
	int startTime;

public:
	Sample(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy);
	Sample(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy, COLORREF _color);

	~Sample() {}

	virtual void Update() override {}
	virtual void Draw(HDC hdc, BOOL transparent) override;
	void DelayDraw(HDC hdc, BOOL transparent);
};
