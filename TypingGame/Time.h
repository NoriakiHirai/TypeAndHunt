#pragma once
#include "render.h"

class Time :
    public Object
{
public:
	int remainingTime;
	DWORD startTime;

public:
	Time(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy);
	Time(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy, COLORREF col);

	~Time() {}

	void Start();
	void Update() override;
	/**
	*	@fn ResetCheck
	*	@brief �^�C�}�[��0�ȉ����`�F�b�N����
	*	@return 0�ȉ��Ftrue
	*/
	bool ResetCheck();
	void Draw(HDC hdc);
};

