#include "ProbremInfo.h"
#include <string.h>
#include <string>
#include "Font.h"

#pragma warning(disable:4996)

ProbremInfo::ProbremInfo(HDC hdc, BOOL trs, RECT rec, float px, float py)
{
	hMdc = hdc;
	isTransparent = trs;
	rect = rec;
	positionX = px; positionY = py;
}

void ProbremInfo::Draw(HDC hdc, int probremNum)
{
	int left, top, width, height;
	int x, y;
	char outputStr[8] = {};
	if (probremNum > 99) {
		strcat(outputStr, "100");
	}
	else if (probremNum <= 99 && probremNum >= 10) {
		outputStr[0] = ' ';
		strcat(outputStr, std::to_string(probremNum).c_str());
	}
	else if (probremNum < 10 && probremNum >0 ) {
		outputStr[0] = outputStr[1] = ' ';
		outputStr[2] = char(probremNum + 48);
	}
	else {
		outputStr[0] = outputStr[1] = ' ';
		outputStr[2] = '0';
	}
	strcat(outputStr, "/100");
	HDC srcDest;
	int index;
	for (auto i = 0; i < 7 ; ++i) {
		index = outputStr[i] - 32;
		if (index < 0) continue;
		left = (8 * index) % 128;
		top = 31 + (16 * ((8 * index) / 128));
		width = 8;
		height = 16;
		RECT rect{ left, top, width, height };
		x = (int)positionX + (FONT_WIDTH * i) + FONT_WIDTH;
		y = (int)positionY;
		TransparentBlt(
			hdc,
			x, y,
			FONT_WIDTH, FONT_HEIGHT,
			hMdc,
			rect.left, rect.top, rect.right, rect.bottom,
			color
		);
	}
}
