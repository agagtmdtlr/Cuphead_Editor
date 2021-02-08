#pragma once

class Boss : public Object
{
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