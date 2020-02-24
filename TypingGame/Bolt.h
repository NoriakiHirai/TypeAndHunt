#pragma once
#include "Effect.h"
class Bolt :
    public Effect
{
private:
    int startTime;

public:
    Bolt(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy);
    virtual ~Bolt();

    virtual void Update() override;
    virtual void Draw(HDC hdc) override;

    void Start();
};

