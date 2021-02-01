#include "stdafx.h"
#include "Object.h"


Object::Object(Grid * grid_, Object_Desc desc)
	: grid(grid_),prev(nullptr),next(nullptr),object_desc(desc)
{
	object_desc.depth = Object_Depth::depth++;
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


//////////////////////////

UINT Object_Depth::depth;
