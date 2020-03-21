//#pragma comment(lib, "winmm.lib")
//#include <mmsystem.h>
#include <atlstr.h>
#include "Bolt.h"
#include "EffectMotion.h"
#include "Audio.h"

Bolt::Bolt(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy)
{
	hMdc = hdc;
	isTransparent = trs;
	rect = rec;
	positionX = px; positionY = py;
	speedX = sx; speedY = sy;

	SetUse(false);
	SetRenderingFlag(false);
}

Bolt::~Bolt()
{
}

void Bolt::Update()
{
	if (!used) return;
	if (timeGetTime() - startTime <= 1500) {
		motion->Update(this);
	}
	else {
		SetUse(false);
		SetRenderingFlag(false);
		Audio::RePlayBGM();
	}
}

void Bolt::Draw(HDC hdc)
{
	if (!onRendering) return;
	int offset = 25;
	TransparentBlt(
		hdc,
		(int)positionX - offset, (int)positionY, size.x, size.y,
		hMdc,
		rect.left, rect.top, rect.right, rect.bottom,
		color
	);

	TransparentBlt(
		hdc,
		(int)positionX, (int)positionY, size.x, size.y,
		hMdc,
		rect.left, rect.top, rect.right, rect.bottom,
		color
	);

	TransparentBlt(
		hdc,
		(int)positionX + offset, (int)positionY, size.x, size.y,
		hMdc,
		rect.left, rect.top, rect.right, rect.bottom,
		color
	);
}

void Bolt::Start()
{
	SetUse(true);
	SetRenderingFlag(true);
	startTime = timeGetTime();
	Audio::PlayAsync("audio/data/wav/bolt.wav", false);
}
