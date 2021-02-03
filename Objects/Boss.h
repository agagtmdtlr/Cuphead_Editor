#pragma once

class Boss : public Object
{
public:
	Boss(Grid * grid_, Object_Desc desc, SceneValues * values);
	virtual ~Boss();

	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P) = 0;
	virtual void Render() = 0;

protected:
	// intro, idle , attack , death
	BossState * state[4];

	int Hp;
};