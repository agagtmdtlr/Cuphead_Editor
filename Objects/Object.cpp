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


void Object::Position(D3DXVECTOR2 pos_)
{
	position = pos_;
}



void Object::Scale(D3DXVECTOR2 scale_)
{
	scale = scale_;
}



void Object::Rotation(D3DXVECTOR3 rotation_)
{
	rotation = rotation_;
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
