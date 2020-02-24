#pragma once
#include "Effect.h"

class Impact : public Effect
{
private:
    int startTime;

public:
    Impact(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy);
    virtual ~Impact();

    virtual void Update() override;
    virtual void Draw(HDC hdc) override;

    void Start();
    
};

