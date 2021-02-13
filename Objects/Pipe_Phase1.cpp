#include "stdafx.h"
#include "Pipe_Phase1.h"

Pipe_Phase1::Pipe_Phase1(Grid * grid_, Object_Desc desc, SceneValues * values)
	:Boss(grid_,desc, values)
{
	state[0] = new Phase1_IntroState();
	state[1] = new Phase1_IdleState();
	state[2] = new Phase1_AttackState();
	state[3] = new Phase1_DeathState();

	object_desc.obj_mode = Object_Mode::Play;
	object_desc.b_line_coll = false;

	position = { 429, -310 };
	scale = { 1,1 };
	rotation = { 0,0,0 };

	Hp = 10;

	currentState = 0; // intro state;
	state[currentState]->Enter(this);


	direction = { -1,  0 }; // 왼쪽으로 이동
}

Pipe_Phase1::~Pipe_Phase1()
{
	for (int i = 0; i < 4; i++)
	{
		SAFE_DELETE(state[i]);
	}

}

void Pipe_Phase1::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	state[currentState]->handleInput(this);
	state[currentState]->Update(this,V, P);
	if(object_desc.obj_mode == Object_Mode::Play)
		grid->Move(this, position);
}

void Pipe_Phase1::Render()
{
	state[currentState]->Render();
}

RECT Pipe_Phase1::GetHitBox()
{
	return state[currentState]->GetSprite()->BoundBox();
}
