#include "stdafx.h"
#include "BinaryFile.h"

//////////////////////////////////////////////////////////////////////////

BinaryWriter::BinaryWriter()
	: fileHandle(NULL), size(0)
{

}

BinaryWriter::~BinaryWriter()
{

}

void BinaryWriter::Open(wstring filePath, UINT openOption)
{
	assert(filePath.length() > 0); // 파일 경로가 입력된게 없다면 오류
	// 파일을 쓰기 권한으로 연다.( 이미 존재할경우 덮어 씌운다 )
	fileHandle = CreateFile
		// 파일 혹은 오브젝트를 생성하거나 열수 있는 함수 입니다.
		// 핸들을 받아와 다른 함수들을 이용해서 열기나 쓰기 등을 할수 있다.
		// ReadFile, WriteFile
	
	(
		filePath.c_str() // LPCTSTR lpFilename 파일을 생성하거나 열 경로
		, GENERIC_WRITE 
		// DWORD dwDesiredAccess :파일ㅇ르 열거나 쓰기등을 할때 엑세스이
		// 권한을 지정합니다. 어떠한 목적으로 사용하냐에 따라 권한을 지정해야 합니다.
		// 플래그를 중복해서 지정할 수 있다.
		/*
		GENERIC_ALL : 모든 액세스 권한
		GENERIC_EXECUTE : 실행 권한
		GENERIC_READ : 읽기 권한
		GENERIC_WRITE : 쓰기 권한
		DELETE : 삭제 권한
		READ_CONTROL : 사용자,그룹,임의 엑세스 제어 목록(DACL)의 읽기 권한
		WRITE_DAC : 임의  엑세스 제어 모록(DACL)이 쓰기 권한
		WRITE_OWNER : 사용자가 쓰기 권한
		SYNCHRONIZE : 동기화 권한
		*/
		, 0 
		// 파일의 공유 모드 : 공유 모드 지정할 경우 다른 프로세스에서 현재 파일을 엑세스 핤수 있다.
		// 하나의 플래그만 지정할수 있다. ( 0 : 모든 프로세스의 접근을 차단함) WRITE 중이기 때문에
		, NULL 
		// lpSecurityAttributes : security_attributes 구조체 포인터 사용하지 않을경우 null
		, openOption
		/* dwCreationDisposition
		파일 존재하는지에 따른 행동 하나만 지정 가능
		CREATE_NEW 파일이 존재하지 않을 경우 새로운 파일 생성 : 존재할겨우 ERROR_FILE_EXISTS (80) 오류 발생
		CREATE_ALWAYS 항상 새로운 파일 (덮어씌우기)
		OPEN_EXISTING 파일이 존재할 경우에만 파일을 엽니다. 존재하지 않을경우 ERROR_FILE_NOT_FOUND (2) 오류 발생
		OPEN_ALWAYS 무조건 파일을 엽니다. 파일이 존재하지 않을 경우 생성후 엽니다.
		TRUNCATE_EXISTING 파일이 존재할 경우 파일을 연후 0으로 만듭니다.
		파일 존재하지 않을 경우 ERROR_FILE_NOT_FOUND
		불러오는 프로세스는 dwDesiredAccess 인수 GENERIC_WRITE 플래그가 있어야 한다.
		*/
		, FILE_ATTRIBUTE_NORMAL
		/* dwFlagsAndAttributes 생성된 파일의 속성을 지정합니다.
		플래그 중복해서 지정 가능
		FILE_ATTRIBUTE_READONLY 파일을 읽기 전용으로 지정합니다.
		FILE_ATTRIBUTE_HIDDEN 파일을 숨김으로 지정합니다.
		FILE_ATTRIBUTE_SYSTEM 파일을 운영체제 전용으로 지정합니다.
		FILE_ATTRIBUTE_ARCHIVE 파일을 보관 가능으로 지정합니다.(이 파일이 백업될 필요성이 있다는것을 알립니다.)
		FILE_ATTRIBUTE_NORMAL 모든 속성을 지정하지 않습니다. 다른 플래그와 같이 사용시 무시된다.
		FILE_ATTRIBUTE_TEMPORARY 임시 파일로 지정합니다.
		FILE_ATTRIBUTE_OFFLINE
		FILE_ATTRIBUTE_ENCRYPTED 파일이나 폴더를 암호활로 지정ㅎ바니다.
		*/
		, NULL
		// GENERIC_READ 액세스 권한을 가진 템플릿 파일의 유효한 핸들 생성된 파일에 대한 속성을 제공하는 템플릿이다.
	);

	// 유효한 핸들값이지 검사한다( 제대로 파일이 생성 열기가 되었는지 확인한다)
	bool isChecked = fileHandle != INVALID_HANDLE_VALUE;
	assert(isChecked);
}

void BinaryWriter::Close()
{
	// 오픈/생성한 핸들값이 존재하면 닫는다.
	if (fileHandle != NULL)
	{
		// 핸들값을 통해 접근 닫기한다.
		CloseHandle(fileHandle);
		// 핸들값 초기화
		fileHandle = NULL;
	}
}

void BinaryWriter::Bool(bool data)
{
	WriteFile(
		fileHandle, 
		// hFile 리소스 작업 요청 가능한 핸들값
		&data, 
		// lpBuffer :
		//writefile 에서는 파일에 쓰고 싶은 스트링이나 오브젝트를 입력한다.
		//readfile 에서는 파일에서 읽은 값을 저장하고 싶은 싶은 스트링이나 오브젝트의 주소를 입력한다.
		sizeof(bool),
		// nNumberOfBytesToWrite
		// 얼마나 슬것인가에 해당하는 크기 값을 입력한
		&size, 
		// lpNumberOfBytesWritten 얼마나 읽고 쓰여졌는가 결과값이 저장되는 주소값을 입력할수 있다.
		NULL
		// lpOverlapped
	);
}

void BinaryWriter::Word(WORD data)
{
	WriteFile(fileHandle, &data, sizeof(WORD), &size, NULL);
}

void BinaryWriter::Int(int data)
{
	WriteFile(fileHandle, &data, sizeof(int), &size, NULL);
}

void BinaryWriter::UInt(UINT data)
{
	WriteFile(fileHandle, &data, sizeof(UINT), &size, NULL);
}

void BinaryWriter::Float(float data)
{
	WriteFile(fileHandle, &data, sizeof(float), &size, NULL);
}

void BinaryWriter::Double(double data)
{
	WriteFile(fileHandle, &data, sizeof(double), &size, NULL);
}

void BinaryWriter::Vector2(const D3DXVECTOR2& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR2), &size, NULL);
}

void BinaryWriter::Vector3(const D3DXVECTOR3& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR3), &size, NULL);
}

void BinaryWriter::Vector4(const D3DXVECTOR4& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR4), &size, NULL);
}

void BinaryWriter::Color3f(const D3DXCOLOR& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXCOLOR) - 4, &size, NULL);
}

void BinaryWriter::Color4f(const D3DXCOLOR& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXCOLOR), &size, NULL);
}

void BinaryWriter::Matrix(const D3DXMATRIX& data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXMATRIX), &size, NULL);
}

void BinaryWriter::String(const string & data)
{
	UInt(data.size()); // 문자열의 길이를 먼저 쓴다.

	const char* str = data.c_str(); // string을 char문자열로 변환한다.
	WriteFile(fileHandle, str, data.size(), &size, NULL); // 문자열을 쓴다.
}

void BinaryWriter::Byte(void * data, UINT dataSize)
{
	// 임의의 바이트값을 저장한다.
	WriteFile(fileHandle, data, dataSize, &size, NULL);
}

//////////////////////////////////////////////////////////////////////////

BinaryReader::BinaryReader()
	: fileHandle(NULL), size(0)
{

}

BinaryReader::~BinaryReader()
{

}

void BinaryReader::Open(wstring filePath)
{
	assert(filePath.length() > 0);
	fileHandle = CreateFile
	(
		filePath.c_str()
		, GENERIC_READ // 읽기 권한
		, FILE_SHARE_READ // 다른 프로세스의 열기 권한을 허가합니다.
		, NULL
		, OPEN_EXISTING // 존재할경우에만 엽니다.
		, FILE_ATTRIBUTE_NORMAL // 파일 속성 없음
		, NULL
	);

	// 열기 성공했는지 검사한다.
	bool isChecked = fileHandle != INVALID_HANDLE_VALUE;
	assert(isChecked);
}

void BinaryReader::Close()
{
	if (fileHandle != NULL)
	{
		CloseHandle(fileHandle);
		fileHandle = NULL;
	}
}

bool BinaryReader::Bool()
{
	bool temp = false; // 파일에서 읽어올 데이터를 담을 변수
	ReadFile(fileHandle,
		&temp, // 읽은 데이터 저장 주소
		sizeof(bool), // 얼마나 읽어올지
		&size, // 얼마나 읽어왔는지 결과
		NULL);

	return temp; // 읽은 데이터 반환
}

WORD BinaryReader::Word()
{
	WORD temp = 0;
	ReadFile(fileHandle, &temp, sizeof(WORD), &size, NULL);

	return temp;
}

int BinaryReader::Int()
{
	int temp = 0;
	ReadFile(fileHandle, &temp, sizeof(int), &size, NULL);

	return temp;
}

UINT BinaryReader::UInt()
{
	UINT temp = 0;
	ReadFile(fileHandle, &temp, sizeof(UINT), &size, NULL);

	return temp;
}

float BinaryReader::Float()
{
	float temp = 0.0f;
	ReadFile(fileHandle, &temp, sizeof(float), &size, NULL);

	return temp;
}

double BinaryReader::Double()
{
	double temp = 0.0f;
	ReadFile(fileHandle, &temp, sizeof(double), &size, NULL);

	return temp;
}

D3DXVECTOR2 BinaryReader::Vector2()
{
	float x = Float();
	float y = Float();

	return D3DXVECTOR2(x, y);
}

D3DXVECTOR3 BinaryReader::Vector3()
{
	float x = Float();
	float y = Float();
	float z = Float();

	return D3DXVECTOR3(x, y, z);
}

D3DXVECTOR4 BinaryReader::Vector4()
{
	float x = Float();
	float y = Float();
	float z = Float();
	float w = Float();

	return D3DXVECTOR4(x, y, z, w);
}

D3DXCOLOR BinaryReader::Color3f()
{
	float r = Float();
	float g = Float();
	float b = Float();

	return D3DXCOLOR(r, g, b, 1.0f);
}

D3DXCOLOR BinaryReader::Color4f()
{
	float r = Float();
	float g = Float();
	float b = Float();
	float a = Float();

	return D3DXCOLOR(r, g, b, a);
}

D3DXMATRIX BinaryReader::Matrix()
{
	D3DXMATRIX matrix;
	matrix._11 = Float();	matrix._12 = Float();	matrix._13 = Float();	matrix._14 = Float();
	matrix._21 = Float();	matrix._22 = Float();	matrix._23 = Float();	matrix._24 = Float();
	matrix._31 = Float();	matrix._32 = Float();	matrix._33 = Float();	matrix._34 = Float();
	matrix._41 = Float();	matrix._42 = Float();	matrix._43 = Float();	matrix._44 = Float();

	return matrix;
}

string BinaryReader::String()
{
	// 가변길이 문자열에 맞춘 메모리공간을 할당하기 위함
	UINT size = Int(); // 읽어올 문자열의 길이 먼저 받는다.

	char* temp = new char[size + 1]; // \0 포함한 문자열의 크기
	ReadFile(fileHandle,
		temp, 
		sizeof(char) * size, // 읽어올 문자열 길이
		&this->size, NULL); //데이터 읽기
	temp[size] = '\0';

	return temp;
}

void BinaryReader::Byte(void ** data, UINT dataSize)
{
	ReadFile(fileHandle, *data, dataSize, &size, NULL);
}
