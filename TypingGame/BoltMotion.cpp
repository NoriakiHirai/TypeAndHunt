#include "BoltMotion.h"
#include "Effect.h"
#include "Audio.h"

const float OFFSET_Y_MAX = 20.f;

void BoltMotion::Update(Effect* effect)
{
    if (movementY >= 20.f) {
        speedY = -0.5f;
    }
    else if (movementY <= 0.f) {
        speedY = 0.5f;
    }
    // 稲妻のモーションは上下に動かす
    effect->Move(0.f, speedY);
    movementY += speedY;
}
