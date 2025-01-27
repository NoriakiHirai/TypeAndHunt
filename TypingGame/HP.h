#pragma once
#include "render.h"

extern const int HP_MAX;

class HP :
    public Object
{
private:
    int hp;

public:
	HP(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy);
	HP(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy, COLORREF col);
	~HP() {}

	void Update() override {}
	void UpdateHP(int);
	void Draw(HDC hdc);
	void ResetHP();

	const int GetHP() const { return hp; }
};

