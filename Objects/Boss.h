#pragma once

class Boss : public Object
{
	friend class Phase1_IdleState;
	friend class Phase1_IntroState;
	friend class Phase1_AttackState;
	friend class Phase1_DeathState;
	friend class Phase2_IdleState;
	friend class Phase2_IntroState;
	friend class Phase2_AttackState;
	friend class Phase2_DeathState;
	friend class Phase3_IdleState;
	friend class Phase3_IntroState;
	friend class Phase3_AttackState;
	friend class Phase3_DeathState;
	friend class Phase4_IdleState;
	friend class Phase4_IntroState;
	friend class Phase4_AttackState;
	friend class Phase4_DeathState;
public:
	Boss(Grid * grid_, Object_Desc desc, SceneValues * values);
	virtual ~Boss();

	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P) = 0;
	virtual void Render() = 0;

public:
	// intro, idle , attack , death
	class BossState * state[4];
	int currentState; // 현재 상태
	int Hp;

	D3DXVECTOR2 direction;
};