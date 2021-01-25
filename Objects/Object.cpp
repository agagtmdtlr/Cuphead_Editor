#include "stdafx.h"
#include "Object.h"


Object::Object(Grid * grid_)
	: grid(grid_),prev(nullptr),next(nullptr)
{
}

Object::~Object()
{
}

void Object::SetHitBox(RECT hitbox)
{
}
