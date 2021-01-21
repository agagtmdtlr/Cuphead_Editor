#include "stdafx.h"
#include "Math.h"

const float Math::PI = 3.14159265f;

float Math::Modulo(float val1, float val2) // ������ ���
{
	while (val1 - val2 >= 0)
		val1 -= val2;

	return val1;
}

float Math::ToRadian(float degree) // ������ ��������
{
	return degree * PI / 180.0f;
}

float Math::ToDegree(float radian) // ������ ������
{
	return radian * 180.0f / PI;
}

float Math::VectorDegree(D3DXVECTOR2 vector1, D3DXVECTOR2 vector2)
{
	D3DXVECTOR2 vector1Normal;
	D3DXVECTOR2 vector2Normal;
	D3DXVec2Normalize(&vector1Normal, &vector1);
	D3DXVec2Normalize(&vector2Normal, &vector2);

	float radian = acosf(D3DXVec2Dot(&vector1Normal, &vector2Normal));
	float degree = ToDegree(radian);

	float cross = vector1Normal.x * vector2Normal.y - vector1Normal.y * vector2Normal.x;
	if (cross > 0.0f)
		degree = 360.0f - degree;
	return degree;
}

float Math::Random(float r1, float r2) // ������ ������ �Ǽ�
{
	float random = ((float)rand()) / (float)RAND_MAX; // ����ȭ 0 ~ 1
	float diff = r2 - r1; // ����
	float val = random * diff; //  ( 0 ~ 1 )  * ����
	
	return r1 + val; // �ּ� �� + ��ġ = ���� ���� ������ ��
}

float Math::Clamp(float value, float min, float max) // ���� ǥ��ȭ
{
	value = value > max ? max : value;
	value = value < min ? min : value;

	return value;
}

int Math::Random(int r1, int r2) // ������ ������ ����
{
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}