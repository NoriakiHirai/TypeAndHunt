#include "Time.h"
#include <Windows.h>

const int REMAINING_TIME_MAX = 6000;

Time::Time(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy)
    : remainingTime(3000), startTime(0)
{
    hMdc = hdc;
    isTransparent = trs;
    rect = rec;
    positionX = px; positionY = py;
    speedX = sx; speedY = sy;
}

Time::Time(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy, COLORREF col)
    : remainingTime(3000), startTime(0)
{
    hMdc = hdc;
    isTransparent = trs;
    rect = rec;
    positionX = px; positionY = py;
    speedX = sx; speedY = sy;
    color = col;
}

void Time::Start()
{
    remainingTime = REMAINING_TIME_MAX;
    startTime = timeGetTime();
}

void Time::Update()
{
    // DWORDÇÕunsigned longÇ»ÇÃÇ≈ÅAå∏éZÇÃç∑àŸÇ…ÇÕíçà”Ç∑ÇÈÇ±Ç∆
    remainingTime = REMAINING_TIME_MAX - (timeGetTime() - startTime);
}

bool Time::ResetCheck()
{
    if (remainingTime <= 0)
        return true;
    return false;
}

void Time::Draw(HDC hdc)
{
    if (!used) return;

    if (remainingTime < 0) remainingTime = 0;
    // îwåi
    TransparentBlt(
        hdc,
        (int)positionX, (int)positionY, size.x, size.y,
        hMdc,
        0, 96, 128, 32,
        color
    );

    TransparentBlt(
        hdc,
        (int)positionX, (int)positionY, 
        remainingTime * size.x / REMAINING_TIME_MAX, 
        size.y,
        hMdc,
        0, 64, 128, 32,
        color
    );
}
