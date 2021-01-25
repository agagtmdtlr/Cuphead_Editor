#pragma once
#include "Systems/Grid.h"

class Object
{
	friend class Grid;
	friend class Sonic;
	friend class EditScene;
public:

	Object(Grid * grid_);
	virtual ~Object();
	virtual D3DXVECTOR2 Position() = 0;
	virtual void Position(D3DXVECTOR2) = 0;
	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P) = 0;
	virtual void Render() = 0;
	virtual void SetHitBox(RECT hitbox);

protected:
	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR3 rotation;

protected:
	Grid* grid;
	Object * prev;
	Object * next;
};