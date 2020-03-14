#include "Font.h"
#include "TypingGame.h"

const int FONT_WIDTH = 16;
const int FONT_HEIGHT = 32;

Font::Font(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy)
{
	memset(probremStr, 0, sizeof(probremStr));
	hMdc = hdc;
	isTransparent = trs;
	rect = rec;
	positionX = px; positionY = py;
	speedX = sx; speedY = sy;
	size.x = 100; size.y = 100;
}

Font::~Font()
{
	DeleteDC(hMdcBG);
}

void Font::Draw(HDC hdc, std::string* str)
{
	if (!used) return;

	// フォントのバックグラウンド
	int bgOffsetY = (FONT_HEIGHT / 3);
	TransparentBlt(
		hdc,
		(int)positionX,
		(int)positionY - bgOffsetY,
		size.x, size.y,
		//FONT_FRAME_WIDTH + (FONT_WIDTH * 2),	// 後半の(FONT_WIDTH * 2)は余白
		//FONT_HEIGHT + (bgOffsetY * 2),
		hMdcBG,
		0, 0, 128, 32,
		color
	);

	unsigned int index = 0;
	int left, top, width, height;
	for (auto i = 0; (*str)[i] != '\0'; ++i) {
		index = (*str)[i] - 64;
		left = (8 * index) % 128;
		top = 63 + (16 * ((8 * index) / 128));
		width = 8;
		height = 16;
		// 透過色付きで表示
		TransparentBlt(
			hdc,
			(int)positionX + (FONT_WIDTH * i) + FONT_WIDTH, (int)positionY,
			FONT_WIDTH, FONT_HEIGHT,
			hMdc,
			left, top, width, height,
			color
		);
	}
}

void Font::Draw(HDC hdc, const char* str)
{
	// フォントのバックグラウンド
	int bgOffsetY = (FONT_HEIGHT / 3);
	TransparentBlt(
		hdc,
		(int)positionX - FONT_WIDTH,
		(int)positionY - bgOffsetY,
		size.x, size.y,
		//FONT_FRAME_WIDTH + (FONT_WIDTH * 2),
		//FONT_HEIGHT + (bgOffsetY * 2),
		hMdcBG,
		0, 0, 128, 32,
		color
	);

	unsigned int index = 0;
	int left, top, width, height;
	for (auto i = 0; str[i] != '\0'; ++i) {
		index = str[i] - 64;
		left = (8 * index) % 128;
		top = 63 + (16 * ((8 * index) / 128));
		width = 8;
		height = 16;
		// 透過色付きで表示
		TransparentBlt(
			hdc,
			(int)positionX + (FONT_WIDTH * i), (int)positionY,
			FONT_WIDTH, FONT_HEIGHT,
			hMdc,
			left, top, width, height,
			color
		);
	}
}

void Font::Draw(HDC hdc, std::string* probremStr, const char* inputStr)
{
	if (!used) return;

	// フォントのバックグラウンド
	int bgOffsetY = (FONT_HEIGHT / 3);
	TransparentBlt(
		hdc,
		(int)positionX,
		(int)positionY - bgOffsetY,
		size.x, size.y,
		//FONT_FRAME_WIDTH + (FONT_WIDTH * 2),
		//FONT_HEIGHT + (bgOffsetY * 2),
		hMdcBG,
		0, 0, 128, 32,
		color
	);

	unsigned int index = 0;
	int left, top, width, height;
	int x, y;
	HDC srcDest;
	for (auto i = 0; inputStr[i] != '\0'; ++i) {
		index = inputStr[i] - 48;
		left = (8 * index) % 128;
		top = 47 + (16 * ((8 * index) / 128));
		width = 8;
		height = 16;
		RECT rect{ left, top, width, height };
		x = (int)positionX + (FONT_WIDTH * i) + FONT_WIDTH;
		y = (int)positionY;
		if (probremStr->size() > i) {
			if (inputStr[i] == (*probremStr)[i]) {
				srcDest = hMdc;
			}
			else {
				srcDest = hMdcRedFont;
			}
		}
		else {
			srcDest = hMdcRedFont;
		}
		DrawChar(x, y, rect, hdc, srcDest);
	}
}

void Font::SetFontBackGround(HDC hdc)
{
	hMdcBG = hdc;
}

void Font::SetFontRed(HDC hdc)
{
	hMdcRedFont = hdc;
}

void Font::SetProbremStr(std::string* str)
{
	ZeroMemory(probremStr, 61 * sizeof(char));
	for (size_t i = 0; (*str)[i] != '\0'; ++i) {
		probremStr[i] = (*str)[i];
	}
}

void Font::DrawChar(int x, int y, RECT rec, HDC destHdc, HDC srcHdc)
{
	TransparentBlt(
		destHdc,
		x, y,
		FONT_WIDTH, FONT_HEIGHT,
		srcHdc,
		rec.left, rec.top, rec.right, rec.bottom,
		color
	);
}
