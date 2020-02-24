#include "Effect.h"
#include "EffectMotion.h"
#include "utility.h"

Effect::Effect()
    : motion(nullptr), onRendering(false)
{
}

Effect::Effect(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy)
    : motion(nullptr), onRendering(false)
{
    hMdc = hdc;
    isTransparent = trs;
    rect = rec;
    positionX = px; positionY = py;
    speedX = sx; speedY = sy;
    //size.x = 100; size.y = 100;
}

Effect::~Effect()
{
    SAFE_DELETE(motion);
}

void Effect::Update()
{
    if (!used) return;
    motion->Update(this);
}

void Effect::SetMotion(EffectMotion* newMotion)
{
    if (motion) {
        SAFE_DELETE(motion);
    }
    motion = newMotion;
}

void Effect::Draw(HDC hdc)
{
    if (!onRendering) return;
    TransparentBlt(
        hdc,
        (int)positionX, (int)positionY, size.x, size.y,
        hMdc,
        rect.left, rect.top, rect.right, rect.bottom,
        color
    );
}
