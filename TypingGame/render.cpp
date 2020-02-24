#include "render.h"
#include <math.h>
#include <Windows.h>

//***************************************************************************
// ���C�u�����̃����N
//***************************************************************************
#pragma comment(lib, "winmm.lib")

void Object::Init() {
    used = FALSE;
    isTransparent = FALSE;

    hMdc = nullptr;

    rect = { 0,0,0,0 };
	//color = RGB(255, 255, 255);
	color = RGB(0, 0, 0);

    positionX = positionY = speedX = speedY = 0.f;
}

void Object::Move(float movementX, float movementY)
{
    positionX += movementX;
    positionY += movementY;
}

void Object::Draw(HDC hdc, BOOL transparent)
{
	if (!used) return;
	// hdc���o�b�N�o�b�t�@�p�̃������[DC�ɓ���ւ��Ă���̂ŁA
	// ���L��TransparentBlt�̓o�b�N�o�b�t�@�ɏ������ނ��ƂƂȂ�B
	if (transparent) {
		// ���ߐF�t���ŕ\��
		TransparentBlt(
			hdc,
			(int)positionX, (int)positionY, rect.right, rect.bottom,
			hMdc,
			rect.left, rect.top, rect.right, rect.bottom,
			color);
	}
	else {
		// �P���\��
		BitBlt(
			hdc,
			(int)positionX, (int)positionY,
			rect.right, rect.bottom,
			hMdc,
			rect.left, rect.top,
			SRCCOPY);
	}
}


/**
*	Sample�N���X��`
*/
Sample::Sample(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy)
{
	hMdc = hdc;
	used = TRUE;
	isTransparent = trs;
	rect = rec;
	positionX = px; positionY = py;
	speedX = sx; speedY = sy;
	startTime = 0;
}

Sample::Sample(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy, COLORREF _color)
{
	hMdc = hdc;
	used = TRUE;
	isTransparent = trs;
	rect = rec;
	positionX = px; positionY = py;
	speedX = sx; speedY = sy;
	color = _color;
	startTime = 0;
}

void Sample::Draw(HDC hdc, BOOL transparent)
{
	if (!used) return;
	// hdc���o�b�N�o�b�t�@�p�̃������[DC�ɓ���ւ��Ă���̂ŁA
	// ���L��TransparentBlt�̓o�b�N�o�b�t�@�ɏ������ނ��ƂƂȂ�B
	if (transparent) {
		// ���ߐF�t���ŕ\��
		TransparentBlt(
			hdc,
			(int)positionX, (int)positionY, size.x, size.y,
			hMdc,
			rect.left, rect.top, rect.right, rect.bottom,
			color);
	}
	else {
		// �P���\��
		BitBlt(
			hdc,
			(int)positionX, (int)positionY,
			rect.right, rect.bottom,
			hMdc,
			rect.left, rect.top,
			SRCCOPY);
	}
}

void Sample::DelayDraw(HDC hdc, BOOL transparent)
{
	if (startTime == 0) {
		startTime = timeGetTime();
		return;
	}

	if (timeGetTime() - startTime > 1500) {
		Draw(hdc, transparent);
	}
}
