#pragma once
#include "OnGroundState.h"
class AimState : public OnGroundState
{
	friend class OnGroundState;
public:
	AimState();
	virtual ~AimState() {}
	virtual void handleInput(class Player* player) override;
	virtual void Enter(class Player* player);
	virtual void Update(class Player* player, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
};