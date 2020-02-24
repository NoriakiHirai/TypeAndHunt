#pragma once
#include "render.h"

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

	const int GetHP() const { return hp; }
};

