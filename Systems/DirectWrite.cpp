#include "stdafx.h"
#include "DirectWrite.h"

DirectWrite* DirectWrite::instance = NULL;

IDWriteFactory* DirectWrite::writeFactory = NULL;
ID2D1DeviceContext* DirectWrite::deviceContext = NULL;
ID2D1Bitmap1* DirectWrite::bitmap = NULL;
IDXGISurface* DirectWrite::surface = NULL;

ID2D1SolidColorBrush* DirectWrite::brush = NULL;
IDWriteTextFormat* DirectWrite::format = NULL;


void DirectWrite::Create()
{
	assert(instance == NULL);

	instance = new DirectWrite();
}

void DirectWrite::Delete()
{
	SAFE_DELETE(instance);
}

DirectWrite * DirectWrite::Get()
{
	return instance;
}

DirectWrite::DirectWrite()
{
	HRESULT hr;
	// ���� directwrite ��ü�� �ļ� ������ ���Ǵ� ���丮 ��ü�� ����ϴ�.
	hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED, // ���丮 ��ü�� �������� �ݸ����� �����ϴ� ��
		__uuidof(IDWriteFactory), // iid ( REFID) directWrite ���丮 �������̽��� �ĺ��ϴ� GUID���Դϴ�.
		(IUnknown**)&writeFactory // ���� ���� ���丮 ��ü�� ���� ������ �ּ�
	);
	assert(SUCCEEDED(hr));

	D2D1_FACTORY_OPTIONS option;
	option.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
	hr = D2D1CreateFactory
	(
		D2D1_FACTORY_TYPE_MULTI_THREADED, // ���丮�� ������ �𵨰� �����ϴ� ���ҽ�
		&factory // ���� ���� ���丮 ��ü�� ���� ������ �ּ�
	);
	assert(SUCCEEDED(hr));

	IDXGIDevice* dxgiDevice; // dxgi�� ��ϵ� ����̽�
	hr = Device->QueryInterface(&dxgiDevice); //direct3d �� dxgi device ������ �η��´�.
	assert(SUCCEEDED(hr));

	// direct2d  ���ҽ� ���� ��ġ�� �����մϴ�.
	hr = factory->CreateDevice(dxgiDevice, &device); // direct3d dxgi�� direct2d device ȣȯ ��Ű��
	assert(SUCCEEDED(hr));

	// direct2d ���ҽ� ���� ��ġ���� ����� devicecontext�� �����մϴ�.
	hr = device->CreateDeviceContext // direct2d device context �����
	(
		D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &deviceContext
	);
	assert(SUCCEEDED(hr));

	CreateBackBuffer();
}

DirectWrite::~DirectWrite()
{
	DeleteBackBuffer();
}

// SwapChain buffer�� direct2D ������ ����
void DirectWrite::CreateBackBuffer()
{
	HRESULT hr;

	// swapchain getbuffer�޼���� DXGI ǥ���� ������
	// IDXGISurface
	hr = SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&surface);
	assert(SUCCEEDED(hr));

	// ��Ʈ�� �ɼǰ� ��� ������ ���ؽ�Ʈ ������ ���� ����ü
	D2D1_BITMAP_PROPERTIES1 bp;
	// ���ĸ� �����Ͽ� ä�δ� 8��Ʈ�� �����ϴ� 4����, 32��Ʈ ��ȣ���� ����ȯ �� ���� sRGB����
	// direct3d backbuffer ��ȣȣȯ �����̺�� format�� ����� �Ѵ�	
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM; // ���ȼ����� ä���� ũ��� �迭�� �����ϴ°�
	/*
	D2D1_ALPHA_MODE
	����ä���� �̸� ������ ����, ��Ʈ����Ʈ ���ĸ� ����ϴ��� ����, �����ϰ� ������ �Ѱ����� �����ؾ��ϴ���
	�Ǵ� �� �� ������
	*/
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE; // ���İ��� ���õ˴ϴ�.
	// dpi : 1��ġ �� ��Ʈ�� ���� �������� ������ �̹���
	bp.dpiX = 96;
	bp.dpiY = 96;
	// D2D1_BITMAP_OPTIONS_TARGET ��Ʈ ���� device context target���� ��� �� �� �ֽ��ϴ�.
	// D2D1_BITMAP_OPTIONS_CANNOT_DRAW ��Ʈ ���� �Է����� ����� �� �����ϴ�.
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	// ���������� ������ ���� ���ؽ�Ʈ ���� �Դϴ�.
	bp.colorContext = NULL;

	/* ID2D1DeviceContext:: CreateBitmapFromDxgiSurface
	(IDXGISurface,
	const d2d1_bitmap_properties1 &,
	ID2D1Bitmap1
	)
	DXGI ǥ�鿡�� ��� ǥ������ �����ϰų� �߰� ���� ���ؽ�Ʈ ������ ������ �� �ִ� ��Ʈ ���� ����ϴ�.
	�� ��Ʈ �� ��ü�� ���� ������ �ּҸ� �����մϴ�.
	-- ��Ʈ �� ������ �������� ���� ��� ����� ��
	1. bitmap dpi == 96
	2. pixelformat == dxgi suface format
	3. ��ȯ�� ��Ʈ ���� dxgi surface's binding flag�� ����մϴ�. ( d3d10_usage_dynamic �� ���� direct2d �÷��׿� ȣȯ �ȵ�)
	4. color context is unknown,
	5. the alphamode : premultiplied(common case) or straight(A8),
	----- ��Ʈ�� �Ӽ� ���Ǵ� ���� ��Ģ�� ���� ����ȴ�.
	dpi�� �Ӽ��� ���� �����ȴ�.
	dpix && dpiy == 0 -> dpi 96 changes
	pixel format must be compatible(��ȣȣȯ) shader resource view or render target view(RTV) surface
	DXGI surface binding flag�� ȣȯ �Ǿ�� �Ѵ�. ( ���� ���� ����) ��Ʈ�� ���ҽ� ���⿡ �������� �ش�.
	*/
	// direct2d�� ������ ��Ʈ�� ǥ���� �����Ѵ�. (surface(3d backbuffer),( bitmap)
	hr = deviceContext->CreateBitmapFromDxgiSurface(surface, &bp, &bitmap);
	assert(SUCCEEDED(hr));

	// direct2d ��ġ ���ؽ�Ʈ�� ������ �� ��Ʈ �� �Ǵ� ��� ����Դϴ�.
	// ������ �� ������� ������ ��Ʈ���� �����մϴ�.
	// SetTarget parameter : ID2D1Image interface ( (CreateEffect and CreateBitmap )
	deviceContext->SetTarget(bitmap); 

	// ������ ����� �⺻ �������� 1.0f�� �� SolidColorBrush�� ����ϴ�.
	hr = deviceContext->CreateSolidColorBrush(
		D2D1::ColorF(1, 1, 1) // �귯�� ������ ����,���,�Ķ� �� ���� ���Դϴ�.
		, &brush // �� �޼��尡 ��ȯ�Ǹ� �� �귯�ÿ� ���� �������� �ּҸ� ���� �մϴ�.
	);
	assert(SUCCEEDED(hr));

	// �ؽ�Ʈ ���̾ƿ��� ���Ǵ� �ؽ�Ʈ ���� ��ü�� ����ϴ�.
	writeFactory->CreateTextFormat
	(
		L"����ü" // fontFamilyName (const WCHAR*)�۲� �йи��� �̸��� �����ϴ� ���� �迭
		, NULL // fontCollection (IDWriteFontCollection *) �۲� �÷��� ��ü�� ���� ������ (NULL == �ý��� �۲� �Ǥ���)
		, DWRITE_FONT_WEIGHT_SEMI_BOLD // �۲� �β��� ��Ÿ���� ��
		, DWRITE_FONT_STYLE_NORMAL // �ؽ�Ʈ ��ü�� �۲� ��Ÿ��
		, DWRITE_FONT_STRETCH_NORMAL // �ؽ�Ʈ ��ü�� �۲� ������ ��Ÿ���� ��
		, 15 // ��ư ������ ( dip "��ġ ���� �ȼ�") �۲��� ���� ũ�� ( dip == 1/96inch)
		, L"ko" // lacaleName ������ �̸��� �����ϴ� ���� �迭
		, &format // textFormat (IDWriteTextFormat **) �� �޼��尡 ��ȯ �Ǹ� ���� ������ �ؽ�Ʈ ���� ��ü�� ���� ������ �ּ� ����
	);
}

void DirectWrite::DeleteBackBuffer()
{
	deviceContext->SetTarget(NULL);

	SAFE_RELEASE(brush);
	SAFE_RELEASE(format);
	SAFE_RELEASE(bitmap);
	SAFE_RELEASE(surface);

}

// �Է� ���ڸ� ������ �Ѵ�.
void DirectWrite::RenderText(wstring & text, RECT & rect)
{
	D2D1_RECT_F temp; // �ؽ�Ʈ�� �׷����� ������ ũ�� �� ��ġ
	temp.left = (float)rect.left;
	temp.top = (float)rect.top;
	temp.right = (float)rect.right;
	temp.bottom = (float)rect.bottom;

	// IDWriteTextFormat ��ü���� �����ϴ� ���� ������ ����Ͽ� ������ �ؽ�Ʈ�� �׸��ϴ�.
	DirectWrite::GetDC()->DrawTextW(
		text.c_str(), // �׸� �����ڵ� ���� �迭�� ���� ������
		text.size(), // ���ڿ��� ���ڼ�
		format, //�۲�,�۲�ũ�� �帧 ���Ⱑ ���� �ؽ�Ʈ ���� ���� ������ �����ϴ� ��ü
		&temp,// layoutRect (D2D1_RECT_F) �ؽ�Ʈ�� �׷����� ������ ũ��� ��ġ
		brush // defaultFillBrush (ID2D1Brush *) , �׸��µ� ����� �귯�� ���� ��ü
		// options : �ؽ�Ʈ�� �ȼ� ��迡 ������ ���ο� �ؽ�Ʈ�� ���̾ƿ� �簢���� ���� �� ���θ� ��Ÿ���� ��
		// (D2D1_DRAW_TEXT_OPTIONS_NONE == default option) -> �ؽ�Ʈ�� �ȼ� ��迡 �����ǰ�
		// ���̾ƿ� �簢���� Ŭ���� ���� �ʾƾ� ���� ��Ÿ���ϴ�.
		// measuringMode (DWRITE_MEARSURING_MODE)
		// ������ ������ �ؽ�Ʈ�� �����ϴ� �� �۸��� ��Ʈ���� ���Ǵ� ����� ��Ÿ���� ���Դϴ�.
		// DWRITE_MEASURING_MODE_NATURAL == default option
		// : ���� ���� �������� �ػ󵵿� �������� �۸��� �̻����� ��Ʈ���� ����Ͽ� �ؽ�Ʈ�� �����ǵ��� �����մϴ�.
	);
}

