#include "ImpactMotion.h"
#include "Effect.h"

void ImpactMotion::Update(Effect* effect)
{
    if (count % 10 == 0) {
        effect->SetRenderingFlag(
            !effect->GetRenderingFlag()
        );
    }
    ++count;
}

