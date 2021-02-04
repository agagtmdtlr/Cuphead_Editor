#pragma once

class Phase1_IntroState : public BossState
{
public:
	Phase1_IntroState();
	virtual ~Phase1_IntroState();
	virtual void handleInput(class Boss* boss);
	virtual void Enter(class Boss* boss);
	virtual void Update(class Boss* boss, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();

private:
	float intro_time;
	float animate_time;
};