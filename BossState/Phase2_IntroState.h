#pragma once

class Phase2_IntroState : public BossState
{
public:
	Phase2_IntroState();
	virtual ~Phase2_IntroState();
	virtual void handleInput(class Boss* boss);
	virtual void Enter(class Boss* boss);
	virtual void Update(class Boss* boss, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
private:
	Animation * head_rise_animation;
	Animation * platform;

	float current_time;
	float head_rise_time;

	
};