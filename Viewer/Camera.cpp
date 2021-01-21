#include "stdafx.h"	
#include "Camera.h"	

Camera::Camera()
	:position(0,0)
{
	D3DXMatrixIdentity(&view); // 단위행렬 초기화
}

Camera::~Camera()
{
}

void Camera::Update()
{
	D3DXMatrixTranslation(&view, -position.x, -position.y, 0.0f); // 카메라 이동
}
