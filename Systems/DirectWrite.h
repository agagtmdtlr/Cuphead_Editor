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
		ID2D1Factory1 �������̽� : direct2d ���ҽ��� �����մϴ�.
		methods
			CreateDevice : ID2D1Device ��ü�� ����ϴ�.

		<dwrite.h>
		IDWriteFactory �������̽� : ��� �ļ� DirectWirte ��ü�� ����µ� ���˴ϴ�.
		��� DirectWrite��ü�� ���� ��Ʈ ���丮 �������̽� �Դϴ�.
		methods
	*/
	ID2D1Factory1* factory;
	static IDWriteFactory* writeFactory;

	/*
		<d2d1_1.h>
		ID2D1Device �������̽� : ��ü �G ��ġ ���ؽ�Ʈ�� �Բ� ����Ҽ� �մ� ���ҽ� �������� ��Ÿ���ϴ�.
		methods
			CreateDeviceContext : direct2d ��ġ���� �� ��ġ ���ؽ�Ʈ�� ����ϴ�.

		ID2D1DeviceContext �������̽� : ��� �������ϴµ� ���Ǵ� ���� �� ��� ���� ������ ��Ÿ���ϴ�.
		deviceContext�� ��� ��Ʈ �� �Ǵ� ��� ������� ������ �� �� �ֽ��ϴ�.
		methods
			CreateBitmap :
			CreateBitmapFromDxgiSurface : DXGI ǥ�鿡�� ��� ǥ������ �����ϰų� �߰� ���� ���ؽ�Ʈ
			������ ������ �� �ִ� ��Ʈ���� ����ϴ�.

	*/
	ID2D1Device* device; // direct2d ���ҽ� ����
	static ID2D1DeviceContext* deviceContext; // directc2d ����̽� ������ ��� ����

	static ID2D1Bitmap1* bitmap; // �������� ��Ʈ���� �����Ѵ�.
	static IDXGISurface* surface; // bxgi ǥ�� (direct3d buffer���� ������ �����´�) ( swapchain���� �����´�)

	static ID2D1SolidColorBrush* brush;
	static IDWriteTextFormat* format;
};