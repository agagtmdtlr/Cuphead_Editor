#include "stdafx.h"
#include "Marker.h"


Marker::Marker(Grid* grid_, wstring shaderFile, D3DXVECTOR2 start, Object_Desc desc, SceneValues * values)
	:Object(grid_, desc, values)
{
	clip = new Clip(PlayMode::Loop);
	for (UINT i = 0; i < 7; i++)
	{
		float x = (float)i * 15 + 173;
		clip->AddFrame(new Sprite(Textures + L"Bullets.png", shaderFile, x, 155, x + 11, 167), 0.3f);
	}

	position = start;
	clip->Position(position);
	clip->Play();

	if (grid != nullptr)
	{
		grid->Add((Object*)this);
	}
	
}

Marker::~Marker()
{
	SAFE_DELETE(clip);
}

void Marker::Move(D3DXVECTOR2 position_)
{
	grid->Move(this, position_);
}

void Marker::Position(D3DXVECTOR2 pos)
{
	position = pos;
	clip->Position(position);
}

D3DXVECTOR2 Marker::Scale()
{
	return scale;
}

void Marker::Scale(D3DXVECTOR2 scale_)
{
	scale = scale_;
	clip->Scale(scale_);
}

D3DXVECTOR3 Marker::Rotation()
{
	return rotation;
}

void Marker::Rotation(D3DXVECTOR3 rotation_)
{
	rotation = rotation_;
	clip->Rotation(rotation_);
}

RECT Marker::GetHitBox()
{
	return clip->GetSprite()->BoundBox();
}

void Marker::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (object_desc.obj_mode == Object_Mode::Play)
	{
		
	}
	else if (object_desc.obj_mode == Object_Mode::Editor)
	{
		clip->Update(V, P);
	}
}

void Marker::Render()
{
	if(objectMode == Object_Mode::Editor)
		clip->Render();
	//ImGui::LabelText("mark pos", "%f %f", position.x, position.y);
}