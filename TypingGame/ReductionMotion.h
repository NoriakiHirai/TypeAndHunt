#pragma once
#include "EffectMotion.h"
#include <chrono>
#include <Windows.h>

using chronoTime = std::chrono::system_clock::time_point;

class ReductionMotion :
    public EffectMotion
{
private:
    chronoTime startTime;
    POINT originalSize;
    bool completed;

public:
    ReductionMotion(POINT orgSize);
    ~ReductionMotion() {}

    void Start() override;
    void Update(Effect*) override;
    bool IsCompleted();
};

