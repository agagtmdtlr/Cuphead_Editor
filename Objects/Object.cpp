#include "stdafx.h"
#include "Object.h"


Object::Object(Grid * grid_, Object_Desc desc)
	: grid(grid_),prev(nullptr),next(nullptr),object_desc(desc)
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

