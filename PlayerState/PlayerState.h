#pragma once

class State
{
public:
	virtual ~State() {}
	virtual void handleInput(class Player* player);
	virtual void Enter(class Player* player);
	virtual void Update(class Player* player, D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();
public:
	Animation* animation;
};