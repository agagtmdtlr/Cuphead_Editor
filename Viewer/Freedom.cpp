#include "stdafx.h"
#include "Freedom.h"

Freedom::Freedom(float speed)
	:speed(speed)
{

}

Freedom::~Freedom()
{
}

void Freedom::Update()
{
	/*if (Key->Press('A'))
		Move(D3DXVECTOR2(-1, 0) * speed);
	else if (Key->Press('D'))
		Move(D3DXVECTOR2(1, 0) * speed);

	if (Key->Press('W'))
		Move(D3DXVECTOR2(0, 1) * speed);
	else if (Key->Press('S'))
		Move(D3DXVECTOR2(0, -1) * speed);*/
	
	__super::Update();
}

void Freedom::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Freedom::Position(D3DXVECTOR2 & vec)
{
	position = vec;
}

void Freedom::Position(D3DXVECTOR2 vec)
{
	position = vec;
}

void Freedom::Move(D3DXVECTOR2& translation)
{
	position += translation * Timer->Elapsed();
}