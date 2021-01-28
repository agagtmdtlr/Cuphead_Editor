#include "stdafx.h"
#include "CMouse.h"

CMouse::CMouse(HWND handle)
	: handle(handle)
	, position(0, 0)
	, wheelValue(0), wheelPrevValue(0), wheelMoveValue(0)
{
	// ���콺 ��ư ���� ���� �ʱ�ȭ ( 0  == none )
	ZeroMemory(buttonStatus, sizeof(byte) * 8);
	ZeroMemory(buttonPrevStatus, sizeof(byte) * 8);
	ZeroMemory(buttonMap, sizeof(byte) * 8);

	ZeroMemory(buttonCount, sizeof(int) * 8);

	doubleClickTime = GetDoubleClickTime(); // ���� ���콺 ����Ŭ���� ī��Ʈ �ü�� �ý��� ������ ���� �����ð��� �ٸ���
	startDoubleClickTime[0] = GetTickCount(); // ����Ŭ�� �����ð��� ������ ���� ���� �ð� ����

	for (int i = 1; i < 8; i++) // �� ���콺 ��ư ����Ŭ�� ���� ���� �ð� ����
		startDoubleClickTime[i] = startDoubleClickTime[0];
}

CMouse::~CMouse()
{
}

void CMouse::WndProc(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	// ���콺 ���� ���� ���ν��� ȣ��
	if (iMessage == WM_MOUSEMOVE) // ���콺�� ������ ��� ��ġ ����
	{
		position.x = (float)LOWORD(lParam);
		position.y = (float)HIWORD(lParam);
	}
	//D3DXVECTOR2 pos;
	if (iMessage == WM_LBUTTONDOWN) // ���콺 ���� ��ư ������ ��ġ
	{		
		position.x = (float)LOWORD(lParam);
		position.y = (float)HIWORD(lParam);
	}

	if (iMessage == WM_MOUSEWHEEL) // ���콺 �� �巡��
	{
		short temp = (short)HIWORD(wParam); // ���콺 �� ������ ��

		wheelPrevValue = wheelValue; // ���� �ٰ� ����
		wheelValue += (float)temp; // ���� �ٰ� ����
	}
}

void CMouse::Update()
{
	memcpy(buttonPrevStatus, buttonStatus, sizeof(byte) * 8);

	// ���� ����� ��� ���� �ʱ�ȭ
	ZeroMemory(buttonStatus, sizeof(byte) * 8);
	ZeroMemory(buttonMap, sizeof(byte) * 8);

	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	// ��Ŭ�� ���� ( ���� ���콺 ��ư , ������ ���콺 ��ư, �� ��ư )
	for (UINT i = 0; i < 8; i++)
	{
		byte prevStatus = buttonPrevStatus[i];
		byte status = buttonStatus[i];

		if (prevStatus == 0 && status == 1) // ���� ������ ���� ���� ���� ( ��ư�� ������ ����)
			buttonMap[i] = Button::Down;
		else if (prevStatus == 1 && status == 0) // ���� ���Ȱ� ������ �ƴ� ( ��ư�� ���� ���� )
			buttonMap[i] = Button::Up;
		else if (prevStatus == 1 && status == 1) // ������ ���ݵ� ���� ( ������ ���� )
			buttonMap[i] = Button::Press; 
		else // ��ư ���� ��ȭ ����
			buttonMap[i] = Button::None;
	}

	// ��ǻ�� ���� �ð� milisecond unit
	UINT buttonStatus = GetTickCount();
	for (UINT i = 0; i < 8; i++)
	{
		if (buttonMap[i] == Button::Down) // ��ư�� �������
		{
			if (buttonCount[i] == 1) // ����Ŭ��( 1 / 2 ) �����Ȳ
			{
				if (buttonStatus - startDoubleClickTime[i] >= doubleClickTime) // �����ð� �ʰ� ����Ŭ�� ����
					buttonCount[i] = 0; //
			}
			buttonCount[i]++; // Ŭ�� ī��Ʈ ����

			if (buttonCount[i] == 1) // Ŭ�� ī��Ư ( 1 / 2) �����Ȳ
				// ���� ����Ŭ���� �ð��� �����̷� ���ϱ� ���� ���縦 Ŭ������ ���� �ð����� ����
				startDoubleClickTime[i] = buttonStatus;
		}

		if (buttonMap[i] == Button::Up) // ��ư�� ���� ���
		{
			if (buttonCount[i] == 1) // 1 /2 �����Ȳ
			{
				if (buttonStatus - startDoubleClickTime[i] >= doubleClickTime)
					// �ð� ��� ( ���� Ŭ�� ��ȿ ) �ʱ�ȭ
					buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2) // 2 /2 �����Ȳ
			{
				if (buttonStatus - startDoubleClickTime[i] <= doubleClickTime)
					// ���� Ŭ�� ���� ���� ����
					buttonMap[i] = Button::DoubleClick;
				// ������ �ʱ�ȭ
				buttonCount[i] = 0;
			}
		}
	}

	POINT point;
	// winapi ��ǥ�� �»�� ����
	GetCursorPos(&point); // ������ ���� ���콺 ��ǥ
	ScreenToClient(handle, &point); // ���α׷� ���� ���콺 ��ǥ

	// ���� �� �巡�׽� ������ ������ �����Ѵ�.����
	wheelMoveValue = wheelValue - wheelPrevValue;
	wheelPrevValue = wheelValue;


}


