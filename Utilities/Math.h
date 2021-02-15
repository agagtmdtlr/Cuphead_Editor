#pragma once
class Math
{
public:
	static const float PI;

	static float Modulo(float val1, float val2);

	static float ToRadian(float degree);
	static D3DXVECTOR3 ToRadian(D3DXVECTOR3 degree);

	static float ToDegree(float radian);

	static float VectorDegree(D3DXVECTOR2 vector1, D3DXVECTOR2 vector2);

	static int Random(int r1, int r2);
	static float Random(float r1, float r2);

	static float Clamp(float value, float min, float max);


};