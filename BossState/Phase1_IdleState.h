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
	float turn_time; // �ѹ� ���ϴ� ������ �ð�
	float turn_speed; // �ѹ� ���ϴ� ������ �Ÿ�
	D3DXVECTOR2 turn_direction; // ���ϸ鼭 �����̴� ����
	float current_time;

	bool idleEnd = false;
	bool isDie = false;

	float idle_time; // idle �������� : ������ ���� ������ �Ѿ�ϴ�.
	float animate_time;

	int animation_index = 0;
};