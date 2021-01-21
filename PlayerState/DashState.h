#pragma once
#include "PlayerState.h"
class DashState : public State
{
	friend class OnGroundState;
public:
	DashState();
	virtual ~DashState() {}
	virtual void handleInput(class Player* player) override;
	virtual void Enter(class Player* player);
	virtual void Update(class Player* player, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
private:
	float dashSpeed;
	float dashEndTime;
	float dashReadyTime;
	float dashCurrentTime;
};