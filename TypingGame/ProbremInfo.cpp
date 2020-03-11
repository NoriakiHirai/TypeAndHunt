#include "ProbremInfo.h"

ProbremInfo::ProbremInfo(HDC hdc, BOOL trs, RECT rec, float px, float py)
{
	hMdc = hdc;
	isTransparent = trs;
	rect = rec;
	positionX = px; positionY = py;
}

void ProbremInfo::Draw(HDC hdc, int probremNum)
{
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
