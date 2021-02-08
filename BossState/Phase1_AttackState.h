#pragma once

class Phase1_AttackState : public BossState
{
public:
	Phase1_AttackState();
	virtual ~Phase1_AttackState();
	virtual void handleInput(class Boss* boss);
	virtual void Enter(class Boss* boss);
	virtual void Update(class Boss* boss, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
	
public:
	float ready_time;
	float attack_speed;
	
	float animate_time;

	float current_time;
	bool attackEnd = false;

	int animation_index = 0;

};