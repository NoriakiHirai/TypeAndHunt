#include "Impact.h"
#include "EffectMotion.h"

Impact::Impact(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy)
{
	hMdc = hdc;
	isTransparent = trs;
	rect = rec;
	positionX = px; positionY = py;
	speedX = sx; speedY = sy;

	SetUse(false);
	SetRenderingFlag(false);
}

Impact::~Impact()
{
}

void Impact::Update()
{
	if (!used) return;
	if (timeGetTime() - startTime <= 1500) {
		motion->Update(this);
	}
	else {
		SetUse(false);
		SetRenderingFlag(false);
	}
}

void Impact::Draw(HDC hdc)
{
	if (!onRendering) return;
	int offsetX = 300;
	int offsetY = 100;
	TransparentBlt(
		hdc,
		(int)positionX - offsetX, (int)positionY + offsetY, size.x, size.y,
		hMdc,
		rect.left, rect.top, rect.right, rect.bottom,
		color
	);

	TransparentBlt(
		hdc,
		(int)positionX + offsetX - 15, (int)positionY - offsetY, size.x, size.y,
		hMdc,
		rect.left, rect.top, rect.right, rect.bottom,
		color
	);
}

void Impact::Start()
{
	SetUse(true);
	SetRenderingFlag(true);
	startTime = timeGetTime();
}
