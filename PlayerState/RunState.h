#pragma once
#include "OnGroundState.h"
class RunState : public OnGroundState
{
	friend class OnGroundState;
public:
	RunState();
	virtual ~RunState() {}
	virtual void handleInput(class Player* player) override;
	virtual void Enter(class Player* player);
	virtual void Update(class Player* player, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
};