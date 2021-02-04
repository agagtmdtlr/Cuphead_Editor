#pragma once

class BossState
{
public:
	BossState() {}
	virtual ~BossState();
	virtual void handleInput(class Boss* boss);
	virtual void Enter(class Boss* boss);
	virtual void Update(class Boss* boss, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
protected:
	Animation* animation;	
};

enum class BossAction
{
	Intro,
	Idle,
	Attack,
	Death
};

class BossStateManager
{
public:
	static BossState * bossState;
};