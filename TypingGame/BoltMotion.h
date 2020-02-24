#pragma once
#include "EffectMotion.h"

class Effect;

class BoltMotion :
    public EffectMotion
{
private:
    float movementY;
    float speedY;

public:
    BoltMotion() : movementY(0.f), speedY(0.5f) {}
    ~BoltMotion() {}

    void Update(Effect*) override;
};
