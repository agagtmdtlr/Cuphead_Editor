#pragma once

class Pipe_Phase1 : public Boss
{
	friend class Phase1_IntroState;
	friend class Phase1_IdleState;
	friend class Phase1_AttackState;
	friend class Phase1_DeathState;
public:
	Pipe_Phase1(Grid * grid_, Object_Desc desc, SceneValues * values);
	virtual ~Pipe_Phase1();

	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P) override;
	virtual void Render() override;

	
};