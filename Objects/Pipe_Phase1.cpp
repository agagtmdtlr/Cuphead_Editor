#include "stdafx.h"
#include "Pipe_Phase1.h"

Pipe_Phase1::Pipe_Phase1(Grid * grid_, Object_Desc desc, SceneValues * values)
	:Boss(grid_,desc, values)
{
	state[0] = new Phase1_IntroState();
	state[1] = new Phase1_IdleState();
	state[2] = new Phase1_AttackState();
	state[3] = new Phase1_DeathState();
}

Pipe_Phase1::~Pipe_Phase1()
{
}

void Pipe_Phase1::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
}

void Pipe_Phase1::Render()
{
}
