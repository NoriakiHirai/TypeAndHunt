#include "BoltMotion.h"
#include "Effect.h"

const float OFFSET_Y_MAX = 20.f;

void BoltMotion::Update(Effect* effect)
{
    if (movementY >= 20.f) {
        speedY = -0.5f;
    }
    else if (movementY <= 0.f) {
        speedY = 0.5f;
    }
    // ��Ȃ̃��[�V�����͏㉺�ɓ�����
    effect->Move(0.f, speedY);
    movementY += speedY;
}
