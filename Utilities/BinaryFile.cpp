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
	assert(filePath.length() > 0); // ���� ��ΰ� �ԷµȰ� ���ٸ� ����
	// ������ ���� �������� ����.( �̹� �����Ұ�� ���� ����� )
	fileHandle = CreateFile
		// ���� Ȥ�� ������Ʈ�� �����ϰų� ���� �ִ� �Լ� �Դϴ�.
		// �ڵ��� �޾ƿ� �ٸ� �Լ����� �̿��ؼ� ���⳪ ���� ���� �Ҽ� �ִ�.
		// ReadFile, WriteFile
	
	(
		filePath.c_str() // LPCTSTR lpFilename ������ �����ϰų� �� ���
		, GENERIC_WRITE 
		// DWORD dwDesiredAccess :���Ϥ��� ���ų� ������� �Ҷ� ��������
		// ������ �����մϴ�. ��� �������� ����ϳĿ� ���� ������ �����ؾ� �մϴ�.
		// �÷��׸� �ߺ��ؼ� ������ �� �ִ�.
		/*
		GENERIC_ALL : ��� �׼��� ����
		GENERIC_EXECUTE : ���� ����
		GENERIC_READ : �б� ����
		GENERIC_WRITE : ���� ����
		DELETE : ���� ����
		READ_CONTROL : �����,�׷�,���� ������ ���� ���(DACL)�� �б� ����
		WRITE_DAC : ����  ������ ���� ���(DACL)�� ���� ����
		WRITE_OWNER : ����ڰ� ���� ����
		SYNCHRONIZE : ����ȭ ����
		*/
		, 0 
		// ������ ���� ��� : ���� ��� ������ ��� �ٸ� ���μ������� ���� ������ ������ �A�� �ִ�.
		// �ϳ��� �÷��׸� �����Ҽ� �ִ�. ( 0 : ��� ���μ����� ������ ������) WRITE ���̱� ������
		, NULL 
		// lpSecurityAttributes : security_attributes ����ü ������ ������� ������� null
		, openOption
		/* dwCreationDisposition
		���� �����ϴ����� ���� �ൿ �ϳ��� ���� ����
		CREATE_NEW ������ �������� ���� ��� ���ο� ���� ���� : �����Ұܿ� ERROR_FILE_EXISTS (80) ���� �߻�
		CREATE_ALWAYS �׻� ���ο� ���� (������)
		OPEN_EXISTING ������ ������ ��쿡�� ������ ���ϴ�. �������� ������� ERROR_FILE_NOT_FOUND (2) ���� �߻�
		OPEN_ALWAYS ������ ������ ���ϴ�. ������ �������� ���� ��� ������ ���ϴ�.
		TRUNCATE_EXISTING ������ ������ ��� ������ ���� 0���� ����ϴ�.
		���� �������� ���� ��� ERROR_FILE_NOT_FOUND
		�ҷ����� ���μ����� dwDesiredAccess �μ� GENERIC_WRITE �÷��װ� �־�� �Ѵ�.
		*/
		, FILE_ATTRIBUTE_NORMAL
		/* dwFlagsAndAttributes ������ ������ �Ӽ��� �����մϴ�.
		�÷��� �ߺ��ؼ� ���� ����
		FILE_ATTRIBUTE_READONLY ������ �б� �������� �����մϴ�.
		FILE_ATTRIBUTE_HIDDEN ������ �������� �����մϴ�.
		FILE_ATTRIBUTE_SYSTEM ������ �ü�� �������� �����մϴ�.
		FILE_ATTRIBUTE_ARCHIVE ������ ���� �������� �����մϴ�.(�� ������ ����� �ʿ伺�� �ִٴ°��� �˸��ϴ�.)
		FILE_ATTRIBUTE_NORMAL ��� �Ӽ��� �������� �ʽ��ϴ�. �ٸ� �÷��׿� ���� ���� ���õȴ�.
		FILE_ATTRIBUTE_TEMPORARY �ӽ� ���Ϸ� �����մϴ�.
		FILE_ATTRIBUTE_OFFLINE
		FILE_ATTRIBUTE_ENCRYPTED �����̳� ������ ��ȣȰ�� �������ٴϴ�.
		*/
		, NULL
		// GENERIC_READ �׼��� ������ ���� ���ø� ������ ��ȿ�� �ڵ� ������ ���Ͽ� ���� �Ӽ��� �����ϴ� ���ø��̴�.
	);

	// ��ȿ�� �ڵ鰪���� �˻��Ѵ�( ����� ������ ���� ���Ⱑ �Ǿ����� Ȯ���Ѵ�)
	bool isChecked = fileHandle != INVALID_HANDLE_VALUE;
	assert(isChecked);
}

void BinaryWriter::Close()
{
	// ����/������ �ڵ鰪�� �����ϸ� �ݴ´�.
	if (fileHandle != NULL)
	{
		// �ڵ鰪�� ���� ���� �ݱ��Ѵ�.
		CloseHandle(fileHandle);
		// �ڵ鰪 �ʱ�ȭ
		fileHandle = NULL;
	}
}

void BinaryWriter::Bool(bool data)
{
	WriteFile(
		fileHandle, 
		// hFile ���ҽ� �۾� ��û ������ �ڵ鰪
		&data, 
		// lpBuffer :
		//writefile ������ ���Ͽ� ���� ���� ��Ʈ���̳� ������Ʈ�� �Է��Ѵ�.
		//readfile ������ ���Ͽ��� ���� ���� �����ϰ� ���� ���� ��Ʈ���̳� ������Ʈ�� �ּҸ� �Է��Ѵ�.
		sizeof(bool),
		// nNumberOfBytesToWrite
		// �󸶳� �����ΰ��� �ش��ϴ� ũ�� ���� �Է���
		&size, 
		// lpNumberOfBytesWritten �󸶳� �а� �������°� ������� ����Ǵ� �ּҰ��� �Է��Ҽ� �ִ�.
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
	UInt(data.size()); // ���ڿ��� ���̸� ���� ����.

	const char* str = data.c_str(); // string�� char���ڿ��� ��ȯ�Ѵ�.
	WriteFile(fileHandle, str, data.size(), &size, NULL); // ���ڿ��� ����.
}

void BinaryWriter::Byte(void * data, UINT dataSize)
{
	// ������ ����Ʈ���� �����Ѵ�.
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
		, GENERIC_READ // �б� ����
		, FILE_SHARE_READ // �ٸ� ���μ����� ���� ������ �㰡�մϴ�.
		, NULL
		, OPEN_EXISTING // �����Ұ�쿡�� ���ϴ�.
		, FILE_ATTRIBUTE_NORMAL // ���� �Ӽ� ����
		, NULL
	);

	// ���� �����ߴ��� �˻��Ѵ�.
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
	bool temp = false; // ���Ͽ��� �о�� �����͸� ���� ����
	ReadFile(fileHandle,
		&temp, // ���� ������ ���� �ּ�
		sizeof(bool), // �󸶳� �о����
		&size, // �󸶳� �о�Դ��� ���
		NULL);

	return temp; // ���� ������ ��ȯ
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
	// �������� ���ڿ��� ���� �޸𸮰����� �Ҵ��ϱ� ����
	UINT size = Int(); // �о�� ���ڿ��� ���� ���� �޴´�.

	char* temp = new char[size + 1]; // \0 ������ ���ڿ��� ũ��
	ReadFile(fileHandle,
		temp, 
		sizeof(char) * size, // �о�� ���ڿ� ����
		&this->size, NULL); //������ �б�
	temp[size] = '\0';

	return temp;
}

void BinaryReader::Byte(void ** data, UINT dataSize)
{
	ReadFile(fileHandle, *data, dataSize, &size, NULL);
}
