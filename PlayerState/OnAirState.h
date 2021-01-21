#pragma once

class OnAirState : public State
{
public:
	OnAirState();
	virtual ~OnAirState() {}
	virtual void handleInput(class Player* player) override;
	virtual void Enter(class Player* player);
	virtual void Update(class Player* player, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
};