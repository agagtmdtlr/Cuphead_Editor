#pragma once

class DirectWrite
{
public:
	static void Create();
	static void Delete();

	static DirectWrite* Get();
	static ID2D1DeviceContext* GetDC() { return deviceContext; }
	
	static void CreateBackBuffer();
	static void DeleteBackBuffer();

	static void RenderText(wstring& text, RECT& rect);

private:
	static DirectWrite* instance;

private:
	DirectWrite();
	~DirectWrite();

private:
	/*
		<d2d1_1.h>
		ID2D1Factory1 인터페이스 : direct2d 리소스를 생성합니다.
		methods
			CreateDevice : ID2D1Device 개체를 만듭니다.

		<dwrite.h>
		IDWriteFactory 인터페이스 : 모든 후속 DirectWirte 개체를 만드는데 사용됩니다.
		모든 DirectWrite개체에 대한 루트 팩토리 인터페이스 입니다.
		methods
	*/
	ID2D1Factory1* factory;
	static IDWriteFactory* writeFactory;

	/*
		<d2d1_1.h>
		ID2D1Device 인터페이스 : 개체 밎 장치 컨텍스트를 함께 사용할수 잇는 리소스 도메인을 나타냅니다.
		methods
			CreateDeviceContext : direct2d 장치에서 새 장치 컨텍스트를 만듭니다.

		ID2D1DeviceContext 인터페이스 : 대상에 렌더링하는데 사용되는 상태 및 명령 버퍼 집합을 나타냅니다.
		deviceContext는 대상 비트 맵 또는 명령 목록으로 렌더링 할 수 있습니다.
		methods
			CreateBitmap :
			CreateBitmapFromDxgiSurface : DXGI 표면에서 대상 표면으로 설정하거나 추가 색상ㅇ 컨텍스트
			정보를 지정할 수 있는 비트맵을 만듭니다.

	*/
	ID2D1Device* device; // direct2d 리소스 관리
	static ID2D1DeviceContext* deviceContext; // directc2d 디바이스 렌더링 방법 설정

	static ID2D1Bitmap1* bitmap; // 렌더링할 비트맵을 생성한다.
	static IDXGISurface* surface; // bxgi 표면 (direct3d buffer에서 정보를 가져온다) ( swapchain에서 가져온다)

	static ID2D1SolidColorBrush* brush;
	static IDWriteTextFormat* format;
};