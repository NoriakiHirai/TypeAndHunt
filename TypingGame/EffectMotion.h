#pragma once

class Effect;

class EffectMotion
{
public:
    virtual void Start() {}
    virtual void Update(Effect*) = 0;
};

