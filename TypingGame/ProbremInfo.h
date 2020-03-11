#pragma once
#include "render.h"
#include "Font.h"

class ProbremInfo :
    public Object
{
public:
	ProbremInfo(HDC hdc, BOOL trs, RECT rec, float px, float py);
	~ProbremInfo() {}

	void Update() override {}
	void Draw(HDC hdc, int probremNum);
};

