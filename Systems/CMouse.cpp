#include "stdafx.h"
#include "CMouse.h"

CMouse::CMouse(HWND handle)
	: handle(handle)
	, position(0, 0)
	, wheelValue(0), wheelPrevValue(0), wheelMoveValue(0)
{
	// 마우스 버튼 관련 상태 초기화 ( 0  == none )
	ZeroMemory(buttonStatus, sizeof(byte) * 8);
	ZeroMemory(buttonPrevStatus, sizeof(byte) * 8);
	ZeroMemory(buttonMap, sizeof(byte) * 8);

	ZeroMemory(buttonCount, sizeof(int) * 8);

	doubleClickTime = GetDoubleClickTime(); // 현재 마우스 더블클릭의 카운트 운영체제 시스템 설정에 따라 지연시간은 다르다
	startDoubleClickTime[0] = GetTickCount(); // 더블클릭 지연시간은 측정을 위한 시작 시간 갱신

	for (int i = 1; i < 8; i++) // 각 마우스 버튼 더블클릭 측정 시작 시간 갱신
		startDoubleClickTime[i] = startDoubleClickTime[0];
}

CMouse::~CMouse()
{
}

void CMouse::WndProc(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	// 마우스 정보 관련 프로시전 호출
	if (iMessage == WM_MOUSEMOVE) // 마우스가 움직일 경우 위치 갱신
	{
		position.x = (float)LOWORD(lParam);
		position.y = (float)HIWORD(lParam);
	}
	//D3DXVECTOR2 pos;
	if (iMessage == WM_LBUTTONDOWN) // 마우스 왼쪽 버튼 누를시 위치
	{		
		position.x = (float)LOWORD(lParam);
		position.y = (float)HIWORD(lParam);
	}

	if (iMessage == WM_MOUSEWHEEL) // 마우스 휠 드래시
	{
		short temp = (short)HIWORD(wParam); // 마우스 휠 움직임 값

		wheelPrevValue = wheelValue; // 이전 휠값 갱신
		wheelValue += (float)temp; // 현재 휠값 갱신
	}
}

void CMouse::Update()
{
	memcpy(buttonPrevStatus, buttonStatus, sizeof(byte) * 8);

	// 측정 결과를 얻기 위해 초기화
	ZeroMemory(buttonStatus, sizeof(byte) * 8);
	ZeroMemory(buttonMap, sizeof(byte) * 8);

	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	// 원클릭 상태 ( 왼쪽 마우스 버튼 , 오른쪽 마우스 버튼, 휠 버튼 )
	for (UINT i = 0; i < 8; i++)
	{
		byte prevStatus = buttonPrevStatus[i];
		byte status = buttonStatus[i];

		if (prevStatus == 0 && status == 1) // 전에 눌린적 없고 지금 눌림 ( 버튼이 눌리는 순간)
			buttonMap[i] = Button::Down;
		else if (prevStatus == 1 && status == 0) // 전엔 눌렸고 지금은 아님 ( 버튼을 떼는 순가 )
			buttonMap[i] = Button::Up;
		else if (prevStatus == 1 && status == 1) // 전에도 지금도 눌림 ( 눌려진 상태 )
			buttonMap[i] = Button::Press; 
		else // 버튼 상태 변화 없음
			buttonMap[i] = Button::None;
	}

	// 컴퓨터 켜진 시간 milisecond unit
	UINT buttonStatus = GetTickCount();
	for (UINT i = 0; i < 8; i++)
	{
		if (buttonMap[i] == Button::Down) // 버튼을 누른경우
		{
			if (buttonCount[i] == 1) // 더블클릭( 1 / 2 ) 진행상황
			{
				if (buttonStatus - startDoubleClickTime[i] >= doubleClickTime) // 지연시간 초과 더블클릭 실패
					buttonCount[i] = 0; //
			}
			buttonCount[i]++; // 클릭 카운트 증가

			if (buttonCount[i] == 1) // 클릭 카운특 ( 1 / 2) 진행상황
				// 다음 더블클릭의 시간을 딜레이로 비교하기 위해 현재를 클릭측정 시작 시간으로 갱신
				startDoubleClickTime[i] = buttonStatus;
		}

		if (buttonMap[i] == Button::Up) // 버튼을 떼는 경우
		{
			if (buttonCount[i] == 1) // 1 /2 진행상황
			{
				if (buttonStatus - startDoubleClickTime[i] >= doubleClickTime)
					// 시간 경과 ( 더블 클릭 무효 ) 초기화
					buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2) // 2 /2 진행상황
			{
				if (buttonStatus - startDoubleClickTime[i] <= doubleClickTime)
					// 더블 클릭 성공 상태 갱신
					buttonMap[i] = Button::DoubleClick;
				// 성공후 초기화
				buttonCount[i] = 0;
			}
		}
	}

	POINT point;
	// winapi 좌표는 좌상단 기준
	GetCursorPos(&point); // 윈도우 기준 마우스 좌표
	ScreenToClient(handle, &point); // 프로그램 기준 마우스 좌표

	// 현재 휠 드래그시 움직임 정도를 저장한다.ㅁㄹ
	wheelMoveValue = wheelValue - wheelPrevValue;
	wheelPrevValue = wheelValue;


}


