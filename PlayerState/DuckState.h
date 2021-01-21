#pragma once
#include "OnGroundState.h"
class DuckState :public OnGroundState
{
	friend class OnGroundState;
public:
	DuckState();
	virtual ~DuckState() {}
	virtual void handleInput(class Player* player) override;
	virtual void Enter(class Player* player);
	virtual void Update(class Player* player, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
private:
	float duckTime;
	float duckCurrentTime;
};