#include "stdafx.h"	
#include "Camera.h"	

Camera::Camera()
	:position(0,0)
{
	D3DXMatrixIdentity(&view); // ������� �ʱ�ȭ
}

Camera::~Camera()
{
}

void Camera::Position(D3DXVECTOR2)
{
}

void Camera::Position(D3DXVECTOR2 &)
{
}

void Camera::Update()
{
	D3DXMatrixTranslation(&view, -position.x, -position.y, 0.0f); // ī�޶� �̵�
}
