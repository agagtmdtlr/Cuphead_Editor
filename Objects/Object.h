#pragma once
#include "Systems/Grid.h"

enum class Object_Mode
{
	Editor,
	Play
};

class Object
{
	friend class Grid;
	friend class Sonic;
	friend class Editor;
	friend class EditScene;
public:

	Object(Grid * grid_);
	virtual ~Object();

public:

	virtual D3DXVECTOR2 Position() = 0;
	virtual void Position(D3DXVECTOR2) = 0;
	virtual D3DXVECTOR2 Scale() = 0;
	virtual void Scale(D3DXVECTOR2) = 0;
	virtual D3DXVECTOR3 Rotation() = 0;
	virtual void Rotation(D3DXVECTOR3) = 0;

public:

	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P) = 0;
	virtual void Render() = 0;

	virtual RECT GetHitBox();
	virtual void SetHitBox(RECT hitbox);

	void Set_Object_Mode(Object_Mode obj_mode) { objectMode = obj_mode; }

protected:
	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR3 rotation;

	

protected:
	Grid* grid;
	Object * prev;
	Object * next;

	Object_Mode objectMode = Object_Mode::Editor;
};