#pragma once

class BinaryWriter
{
public:
	BinaryWriter();
	~BinaryWriter();

	void Open(wstring filePath, UINT openOption = CREATE_ALWAYS);
	void Close();

	void Bool(bool data);
	void Word(WORD data);
	void Int(int data);
	void UInt(UINT data);
	void Float(float data);
	void Double(double data);

	void Vector2(const D3DXVECTOR2& data);
	void Vector3(const D3DXVECTOR3& data);
	void Vector4(const D3DXVECTOR4& data);
	void Color3f(const D3DXCOLOR& data);
	void Color4f(const D3DXCOLOR& data);
	void Matrix(const D3DXMATRIX& data);

	void String(const string& data);
	void Byte(void* data, UINT dataSize);

protected:
	// 핸들이란 : 운영체제 내부에 잇는 어떤 리소스의 주소를 정수로 치환한 값
	// ㅇㅇ요프로그램은 운영체제의 리소스에 접근해 여리 장치와 데이터를 사용할수\
	// 있는데 하지만 리소스 관리차원에서 메모리 주소로 직접 접근이 불가능하고
	// 핸들을 사용합니다.
	// 사용자는 핸들테이블<리소스 주소,핸들값>에서 핸들값을 이용해 운영체제에 작업을 요청한다.
	HANDLE fileHandle;
	DWORD size;
};

//////////////////////////////////////////////////////////////////////////

class BinaryReader
{
public:
	BinaryReader();
	~BinaryReader();

	void Open(wstring filePath);
	void Close();

	bool Bool();
	WORD Word();
	int Int();
	UINT UInt();
	float Float();
	double Double();

	D3DXVECTOR2 Vector2();
	D3DXVECTOR3 Vector3();
	D3DXVECTOR4 Vector4();
	D3DXCOLOR Color3f();
	D3DXCOLOR Color4f();
	D3DXMATRIX Matrix();

	string String();
	void Byte(void** data, UINT dataSize);

protected:
	HANDLE fileHandle;
	DWORD size;
};