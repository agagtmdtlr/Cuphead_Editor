#include "stdafx.h"
#include "Marker.h"


Marker::Marker(Grid* grid_, wstring shaderFile, D3DXVECTOR2 start)
	:Object(grid_)
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

void Marker::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	clip->Position(position);
	clip->Update(V, P);
}

void Marker::Render()
{
	clip->Render();
	//ImGui::LabelText("mark pos", "%f %f", position.x, position.y);
}