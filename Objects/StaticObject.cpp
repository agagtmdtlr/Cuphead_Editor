#include "stdafx.h"
#include "StaticObject.h"

StaticObject::StaticObject(Grid * grid_, wstring textureFile, Object_Desc desc, SceneValues * values)
	:Object(grid_, desc, values)
{
	object_desc.b_bound_coll = false;
	object_desc.b_line_coll = false;
	object_desc.b_render = true;

	texturePath = textureFile;

	wstring shaderFile = Shaders + L"008_Sprite.fx";
	sprite = new Sprite(textureFile, shaderFile);
	Position(D3DXVECTOR2(0, 0));
	Scale(D3DXVECTOR2(1, 1));
	Rotation(D3DXVECTOR3(0, 0, 0));
}

StaticObject::~StaticObject()
{
	SAFE_DELETE(sprite);
}

D3DXVECTOR2 StaticObject::Position()
{
	return position;
}

void StaticObject::Position(D3DXVECTOR2 position_)
{
	position = position_;
	sprite->Position(position_);
}

D3DXVECTOR2 StaticObject::Scale()
{
	return scale;
}

void StaticObject::Scale(D3DXVECTOR2 scale_)
{
	scale = scale_;
	sprite->Scale(scale_);
}

D3DXVECTOR3 StaticObject::Rotation()
{
	return rotation;
}

void StaticObject::Rotation(D3DXVECTOR3 rotation_)
{
	rotation = rotation_;
	sprite->Rotation(rotation_);
}

void StaticObject::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	sprite->Update(V, P);
}

void StaticObject::Render()
{
	if (objectMode == Object_Mode::Editor)
		sprite->Render();
}

RECT StaticObject::GetHitBox()
{
	return sprite->BoundBox();
}

void StaticObject::SetHitBox(RECT hitbox)
{
}
