#pragma once
#include "PlayerState.h"

class OnGroundState : public State
{
public:
	OnGroundState();
	virtual ~OnGroundState() {}
	virtual void handleInput(class Player* player) override;
	virtual void Enter(class Player* player);
	virtual void Update(class Player* player, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();



};