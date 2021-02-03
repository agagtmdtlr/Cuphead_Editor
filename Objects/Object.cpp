#include "stdafx.h"
#include "Object.h"


Object::Object(Grid * grid_, Object_Desc desc, SceneValues * values)
	: grid(grid_), prev(nullptr), next(nullptr), object_desc(desc)
	,values(values)
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

bool Object::InScreen()
{
	RECT box = GetHitBox();
	D3DXVECTOR2 camera = values->MainCamera->Position();

	return false;
}


//////////////////////////

UINT Object_Depth::depth;
