#pragma once
#include "render.h"

class EffectMotion;

class Effect :
    public Object
{
protected:
    EffectMotion* motion;
	bool onRendering;

public:
	Effect();
	Effect(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy);
	virtual ~Effect();

	void SetRenderingFlag(bool flg) { onRendering = flg; }
	bool GetRenderingFlag() const { return onRendering; }

	virtual void Update() override;
	void SetMotion(EffectMotion*);
	virtual void Draw(HDC hdc);
};

