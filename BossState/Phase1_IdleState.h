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

public:
	float turn_time; // 한번 턴하는 동안의 시간
	float turn_speed; // 한번 턴하는 동안의 거리
	D3DXVECTOR2 turn_direction; // 턴하면서 움직이는 방향
	float current_time;

	bool idleEnd = false;
	bool isDie = false;

	float idle_time; // idle 지속지간 : 끝나면 공격 턴으로 넘어갑니다.
	float animate_time;

	int animation_index = 0;
};