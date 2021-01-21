#include "stdafx.h"
#include "Math.h"

const float Math::PI = 3.14159265f;

float Math::Modulo(float val1, float val2) // 나머지 계산
{
	while (val1 - val2 >= 0)
		val1 -= val2;

	return val1;
}

float Math::ToRadian(float degree) // 각도를 라디안으로
{
	return degree * PI / 180.0f;
}

float Math::ToDegree(float radian) // 라디안을 각도로
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

float Math::Random(float r1, float r2) // 범위내 무작위 실수
{
	float random = ((float)rand()) / (float)RAND_MAX; // 정규화 0 ~ 1
	float diff = r2 - r1; // 범위
	float val = random * diff; //  ( 0 ~ 1 )  * 범위
	
	return r1 + val; // 최소 값 + 위치 = 범위 내의 무작위 수
}

float Math::Clamp(float value, float min, float max) // 범위 표준화
{
	value = value > max ? max : value;
	value = value < min ? min : value;

	return value;
}

int Math::Random(int r1, int r2) // 범위내 무작위 정수
{
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}