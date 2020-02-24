#pragma once
#include "render.h"

class Font : public Object
{
private:
	char probremStr[61];
	HDC hMdcBG;
	HDC hMdcRedFont;

public:
	Font(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy);
	virtual ~Font();

	virtual void Update() override {}
	virtual void Draw(HDC hdc, std::string* str);
	virtual void Draw(HDC hdc, const char* str);
	virtual void Draw(HDC hdc, std::string* probremStr, const char* inputStr);
	void SetFontBackGround(HDC hdc);
	void SetFontRed(HDC hdc);
	void SetProbremStr(std::string* str);

private:
	void DrawChar(int x, int y, RECT rec, HDC destHdc, HDC srcHdc);
};