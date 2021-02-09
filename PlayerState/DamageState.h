#pragma once
#include "PlayerState.h"

class DamageState : public State
{
	friend class OnGroundState;
public:
	DamageState();
	virtual ~DamageState() {}
	virtual void handleInput(class Player* player) override;
	virtual void Enter(class Player* player);
	virtual void Update(class Player* player, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
private:
	float damageCurrentTime;
	bool damageEnd;
};