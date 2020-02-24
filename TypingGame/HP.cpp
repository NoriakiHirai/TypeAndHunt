#include "HP.h"

const int HP_MAX = 5;

HP::HP(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy)
{
	hp = 5;
	hMdc = hdc;
	isTransparent = trs;
	rect = rec;
	positionX = px; positionY = py;
	speedX = sx; speedY = sy;
	//size.x = 100; size.y = 100;
}

HP::HP(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy, COLORREF col)
{
	hp = 5;
	hMdc = hdc;
	isTransparent = trs;
	rect = rec;
	positionX = px; positionY = py;
	speedX = sx; speedY = sy;
	color = col;
}

void HP::UpdateHP(int delta)
{
	hp += delta;
	if (hp > HP_MAX) {
		hp = HP_MAX;
	}
	else if (hp < 0) {
		hp = 0;
	}
}

void HP::Draw(HDC hdc)
{
	// ”wŒi
	TransparentBlt(
		hdc,
		(int)positionX, (int)positionY, size.x, size.y,
		hMdc,
		0, 96, 128, 32,
		color
	);
	// HP
	TransparentBlt(
		hdc,
		(int)positionX, (int)positionY, hp * size.x / HP_MAX, size.y,
		hMdc,
		0, 32, 128, 32,
		color
	);
}

void HP::ResetHP()
{
	hp = HP_MAX;
}
