#pragma once

class Phase1_DeathState : public BossState
{
public:
	Phase1_DeathState();
	virtual ~Phase1_DeathState();
	virtual void handleInput(class Boss* boss);
	virtual void Enter(class Boss* boss);
	virtual void Update(class Boss* boss, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();

private:
	float animate_time;
	int animation_index;
};