#pragma once

class Phase1_IdleState : public BossState
{
public:
	Phase1_IdleState();
	virtual ~Phase1_IdleState();
	virtual void handleInput(class Boss* boss);
	virtual void Enter(class Boss* boss);
	virtual void Update(class Boss* boss, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
};