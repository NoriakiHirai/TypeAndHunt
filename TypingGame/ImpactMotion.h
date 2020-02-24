#pragma once
#include "EffectMotion.h"

class Effect;

class ImpactMotion :
    public EffectMotion
{
private:
    int count;

public:
    ImpactMotion() : count(0) {}
    ~ImpactMotion() {}

    void Update(Effect*) override;
};

