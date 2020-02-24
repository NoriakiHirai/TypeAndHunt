#include "ReductionMotion.h"
#include "Effect.h"

static const double LIMIT_TIME = 1000;

ReductionMotion::ReductionMotion(POINT orgSize)
    : completed(true)
{
    startTime = std::chrono::system_clock::now();
    originalSize = orgSize;
}

void ReductionMotion::Start()
{
    startTime = std::chrono::system_clock::now();
    completed = false;
}

void ReductionMotion::Update(Effect* effect)
{
    chronoTime nowTime = std::chrono::system_clock::now();
    auto elapsed = std::chrono::
        duration_cast<std::chrono::milliseconds>
        (nowTime - startTime).count();
    if (elapsed < 500) {
        // “®‚©‚³‚È‚¢
    }
    else if (elapsed >= 500 &&
        elapsed < 700) {
        // ‰º‚ÉˆÚ“®
        POINT pos = effect->GetPosition();
        effect->SetPosition(pos.x, pos.y + 1);
    }
    else if (elapsed >= 700 &&
        elapsed < LIMIT_TIME) {
        // ã‚ÉˆÚ“®
        POINT pos = effect->GetPosition();
        effect->SetPosition(pos.x, pos.y - 10);
    }
    else if (elapsed >= LIMIT_TIME) {
        effect->SetUse(false);
        completed = true;
    }
    //else {
    //    effect->SetSize(
    //        originalSize.x * (LIMIT_TIME - elapsed) / LIMIT_TIME,
    //        originalSize.y * (LIMIT_TIME - elapsed) / LIMIT_TIME
    //    );
    //}
}

bool ReductionMotion::IsCompleted()
{
    return completed;
}
