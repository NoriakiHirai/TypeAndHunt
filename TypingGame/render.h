#pragma once
#include <Windows.h>
#include <math.h>
#include <wingdi.h>
#include <string>

#pragma comment (lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

/**
*   �L�����N�^�̃X�[�p�[�N���X
*/
class Object
{
public:
protected:
	BOOL used;					// �g�p���t���O
	BOOL isTransparent;			// ���ߏ����̗L��
	HDC hMdc;					// �摜��\��DC
	RECT rect;					// �摜���̃L�����N�^�̍���ƃT�C�Y
	COLORREF color;				// ���ߐF
	float positionX, positionY;	// �L�����N�^�̕\�����W
	float speedX, speedY;		// �L�����N�^�̑��x
	POINT size;	// �T�C�Y

public:
	Object() { Init(); }
	virtual ~Object() {}

	void Init();

	// �A�N�Z�T
	void SetTransparent(BOOL trs) { isTransparent = trs; }
	void SetUse(BOOL use) { used = use; }
	void SetColor(COLORREF col) { color = col; }
	void SetRect(RECT r) { rect = r; }
	void SetPosition(float x, float y) { positionX = x; positionY = y; }
	void SetSpeed(float x, float y) { speedX = x; speedY = y; }
	void SetSize(int w, int h) {
		size.x = w;
		size.y = h;
	}
	virtual void Move(float movementX, float movementY);

	// �A�b�v�f�[�g
	virtual void Update() = 0;
	virtual void Draw(HDC hdc, BOOL transparent);
};

/**
*	�T���v���L�����N�^�I�u�W�F�N�g
*/
class Sample : public Object
{
private:
	int startTime;

public:
	Sample(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy);
	Sample(HDC hdc, BOOL trs, RECT rec, float px, float py, float sx, float sy, COLORREF _color);

	~Sample() {}

	virtual void Update() override {}
	virtual void Draw(HDC hdc, BOOL transparent) override;
	void DelayDraw(HDC hdc, BOOL transparent);
};
