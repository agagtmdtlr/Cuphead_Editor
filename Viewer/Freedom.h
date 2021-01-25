#pragma once
#include "Camera.h"

class Freedom : public Camera
{
public:
	Freedom(float speed = 200.0f);
	~Freedom();

	void Update();
	void Position(float x, float y);
	virtual void Position(D3DXVECTOR2 & vec) override;
	virtual void Position(D3DXVECTOR2  vec) override;

private:
	void Move(D3DXVECTOR2& translation);
private:
	float speed;
};