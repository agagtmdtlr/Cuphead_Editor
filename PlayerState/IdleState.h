#pragma once
#include "OnGroundState.h"

class IdleState : public OnGroundState
{
	friend class OnGroundState;
public:
	IdleState();
	virtual ~IdleState() {}
	virtual void handleInput(class Player* player) override;
	virtual void Enter(class Player* player);
	virtual void Update(class Player* player, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();

};