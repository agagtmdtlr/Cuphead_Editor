#include "stdafx.h"
#include "BossState/BossState.h"

BossState::~BossState()
{
	SAFE_DELETE(animation);
}

void BossState::handleInput(Boss * boss)
{
}

void BossState::Enter(Boss * boss)
{
}

void BossState::Update(Boss * boss, D3DXMATRIX & V, D3DXMATRIX & P)
{
}

void BossState::Render()
{
}
