#include "stdafx.h"
#include "Object.h"


Object::Object(Grid * grid_, Object_Desc desc, SceneValues * values)
	: grid(grid_), prev(0), next(0), object_desc(desc)
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

void Object::RotationDegree(float x, float y, float z)
{
	RotationDegree(D3DXVECTOR3(x, y, z));

}

void Object::RotationDegree(D3DXVECTOR3 & vec)
{
	vec.x = Math::ToRadian(vec.x);
	vec.y = Math::ToRadian(vec.y);
	vec.z = Math::ToRadian(vec.z);

	Rotation(vec);
}

D3DXVECTOR3 Object::RotationDegree()
{
	D3DXVECTOR3 vec;
	vec.x = Math::ToDegree(rotation.x);
	vec.y = Math::ToDegree(rotation.y);
	vec.z = Math::ToDegree(rotation.z);

	return vec;
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

	
	if (horizontal.x + camera.x > box.right) return false;
	if (horizontal.y + camera.x < box.left) return false;
	if (vertical.x + +camera.y > box.top) return false;
	if (vertical.y + +camera.y < box.bottom) return false;

	return true;
}

void Object::BoundCollision(Object_Desc & desc)
{
}

void Object::LineCollision(D3DXVECTOR2 & p1, D3DXVECTOR2 & p2)
{
}

Sprite * Object::GetSprite()
{
	return nullptr;
}




//////////////////////////

UINT Object_Depth::depth;
