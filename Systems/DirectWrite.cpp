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
	// 개별 directwrite 개체의 후속 생성에 사용되는 팩토리 개체를 만듭니다.
	hr = DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED, // 팩토리 개체를 공유할지 격리할지 지정하는 값
		__uuidof(IDWriteFactory), // iid ( REFID) directWrite 팩토리 인터페이스를 식별하는 GUID값입니다.
		(IUnknown**)&writeFactory // 새로 만든 팩토리 개체에 대한 포인터 주소
	);
	assert(SUCCEEDED(hr));

	D2D1_FACTORY_OPTIONS option;
	option.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
	hr = D2D1CreateFactory
	(
		D2D1_FACTORY_TYPE_MULTI_THREADED, // 팩토리의 스레딩 모델과 생성하는 리소스
		&factory // 새로 만든 팩토리 개체에 대한 포인터 주소
	);
	assert(SUCCEEDED(hr));

	IDXGIDevice* dxgiDevice; // dxgi에 등록된 디바이스
	hr = Device->QueryInterface(&dxgiDevice); //direct3d 에 dxgi device 정보를 부러온다.
	assert(SUCCEEDED(hr));

	// direct2d  리소스 관리 장치를 생성합니다.
	hr = factory->CreateDevice(dxgiDevice, &device); // direct3d dxgi와 direct2d device 호환 시키기
	assert(SUCCEEDED(hr));

	// direct2d 리소스 관리 장치에서 사용할 devicecontext를 생성합니다.
	hr = device->CreateDeviceContext // direct2d device context 만들기
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

// SwapChain buffer에 direct2D 콘텐츠 쓰기
void DirectWrite::CreateBackBuffer()
{
	HRESULT hr;

	// swapchain getbuffer메서드로 DXGI 표면을 얻어오기
	// IDXGISurface
	hr = SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&surface);
	assert(SUCCEEDED(hr));

	// 비트맵 옵션과 사용 가능한 컨텍스트 정보를 담은 구조체
	D2D1_BITMAP_PROPERTIES1 bp;
	// 알파를 포함하여 채널당 8비트를 지원하는 4성분, 32비트 부호없는 정규환 된 정수 sRGB형식
	// direct3d backbuffer 상호호환 상태이브로 format을 맞춰야 한다	
	bp.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM; // 각픽셀에서 채널의 크기와 배열을 지정하는값
	/*
	D2D1_ALPHA_MODE
	알파채널이 미리 곱해진 알파, 스트레이트 알파를 사용하는지 여부, 무시하고 불투명 한것으로 간주해야하는지
	또는 알 수 없는지
	*/
	bp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE; // 알파값은 무시됩니다.
	// dpi : 1인치 당 도트의 갯수 높을수록 선명한 이미지
	bp.dpiX = 96;
	bp.dpiY = 96;
	// D2D1_BITMAP_OPTIONS_TARGET 비트 맵은 device context target으로 사용 할 수 있습니다.
	// D2D1_BITMAP_OPTIONS_CANNOT_DRAW 비트 맵은 입력으로 사용할 수 없습니다.
	bp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	// 선택적으로 지정된 색상 컨텍스트 정보 입니다.
	bp.colorContext = NULL;

	/* ID2D1DeviceContext:: CreateBitmapFromDxgiSurface
	(IDXGISurface,
	const d2d1_bitmap_properties1 &,
	ID2D1Bitmap1
	)
	DXGI 표면에서 대상 표면으로 설정하거나 추가 색상 컨텍스트 정보를 지정할 수 있는 비트 맵을 만듭니다.
	새 비트 맵 개체에 대한 포인터 주소를 저장합니다.
	-- 비트 맵 속정이 지정되지 않은 경우 고려할 점
	1. bitmap dpi == 96
	2. pixelformat == dxgi suface format
	3. 반환된 비트 맵은 dxgi surface's binding flag를 상속합니다. ( d3d10_usage_dynamic 은 공개 direct2d 플래그와 호환 안됨)
	4. color context is unknown,
	5. the alphamode : premultiplied(common case) or straight(A8),
	----- 비트맵 속성 정의는 다음 규칙에 따라 적용된다.
	dpi는 속성에 의해 지정된다.
	dpix && dpiy == 0 -> dpi 96 changes
	pixel format must be compatible(상호호환) shader resource view or render target view(RTV) surface
	DXGI surface binding flag와 호환 되어야 한다. ( 하휘 집합 가능) 비트맵 리소스 보기에 영ㅇ향을 준다.
	*/
	// direct2d의 렌더릴 비트맵 표면을 생성한다. (surface(3d backbuffer),( bitmap)
	hr = deviceContext->CreateBitmapFromDxgiSurface(surface, &bp, &bitmap);
	assert(SUCCEEDED(hr));

	// direct2d 장치 컨텍스트가 렌더링 될 비트 맵 또는 명령 목록입니다.
	// 렌더링 할 대상으로 생성한 비트맵을 설정합니다.
	// SetTarget parameter : ID2D1Image interface ( (CreateEffect and CreateBitmap )
	deviceContext->SetTarget(bitmap); 

	// 지정된 색상과 기본 불투명도가 1.0f인 새 SolidColorBrush를 만듭니다.
	hr = deviceContext->CreateSolidColorBrush(
		D2D1::ColorF(1, 1, 1) // 브러시 색상의 빨강,녹색,파랑 및 알파 값입니다.
		, &brush // 이 메서드가 반환되면 새 브러시에 대한 포인터의 주소를 포함 합니다.
	);
	assert(SUCCEEDED(hr));

	// 텍스트 레이아웃에 사용되는 텍스트 서식 개체를 만듭니다.
	writeFactory->CreateTextFormat
	(
		L"돋움체" // fontFamilyName (const WCHAR*)글꼴 패밀리의 이름을 포함하는 문자 배열
		, NULL // fontCollection (IDWriteFontCollection *) 글꼴 컬렉션 개체에 대한 포인터 (NULL == 시스템 글꼴 ㅗㅁ음)
		, DWRITE_FONT_WEIGHT_SEMI_BOLD // 글꼴 두께를 나타내는 값
		, DWRITE_FONT_STYLE_NORMAL // 텍스트 개체의 글꼴 스타일
		, DWRITE_FONT_STRETCH_NORMAL // 텍스트 개체의 글꼴 늘임을 나타내는 값
		, 15 // 포튼 사이즈 ( dip "장치 독립 픽셀") 글꼴의 논리적 크기 ( dip == 1/96inch)
		, L"ko" // lacaleName 로케일 이름을 포함하는 문자 배열
		, &format // textFormat (IDWriteTextFormat **) 이 메서드가 반환 되면 새로 생성된 텍스트 형식 개체에 대한 포인터 주소 포함
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

// 입력 문자를 렌더링 한다.
void DirectWrite::RenderText(wstring & text, RECT & rect)
{
	D2D1_RECT_F temp; // 텍스트가 그려지는 영역의 크기 와 위치
	temp.left = (float)rect.left;
	temp.top = (float)rect.top;
	temp.right = (float)rect.right;
	temp.bottom = (float)rect.bottom;

	// IDWriteTextFormat 개체에서 제공하는 형식 정보를 사용하여 지정된 텍스트를 그립니다.
	DirectWrite::GetDC()->DrawTextW(
		text.c_str(), // 그릴 유니코드 문자 배열에 대한 포인터
		text.size(), // 문자열의 문자수
		format, //글꼴,글꼴크기 흐름 방향가 같이 텍스트 서식 세부 정보를 절명하는 개체
		&temp,// layoutRect (D2D1_RECT_F) 텍스트가 그려지는 영역의 크기와 위치
		brush // defaultFillBrush (ID2D1Brush *) , 그리는데 사용할 브러쉬 서식 게체
		// options : 텍스트를 픽셀 경계에 맞출지 여부와 텍스트를 레이아웃 사각형에 맞출 지 여부를 나타내는 값
		// (D2D1_DRAW_TEXT_OPTIONS_NONE == default option) -> 텍스트가 픽셀 경계에 스냅되고
		// 레이아웃 사각형에 클리핑 되지 않아야 함을 나타냅니다.
		// measuringMode (DWRITE_MEARSURING_MODE)
		// 서식이 지정된 텍스트를 측정하는 데 글리프 메트릭이 사용되는 방식을 나타내는 값입니다.
		// DWRITE_MEASURING_MODE_NATURAL == default option
		// : 값이 혀재 디스프레이 해상도에 독립적인 글리프 이상ㄴ적인 메트릭을 사용하여 텍스트가 측정되도록 지정합니다.
	);
}

