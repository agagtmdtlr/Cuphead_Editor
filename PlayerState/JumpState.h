#pragma once
#include "OnAirState.h"
class JumpState : public OnAirState
{
	friend class OnAirState;
public:
	JumpState();
	virtual ~JumpState() {}
	virtual void handleInput(class Player* player) override;
	virtual void Enter(class Player* player);
	virtual void Update(class Player* player, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
};