#include "stdafx.h"
#include "Object.h"


Object::Object(Grid * grid_)
	: grid(grid_),prev(nullptr),next(nullptr)
{
}

Object::~Object()
{
}

RECT Object::GetHitBox()
{
	return RECT();
}

void Object::SetHitBox(RECT hitbox)
{
}

