#pragma once
#include "Object.h"

class StaticObject : public Object
{
public:
	StaticObject(Grid * grid_, wstring textureFile, Object_Desc desc);
	virtual ~StaticObject();

public:

	virtual D3DXVECTOR2 Position() override;
	virtual void Position(D3DXVECTOR2) override;
	virtual D3DXVECTOR2 Scale() override;
	virtual void Scale(D3DXVECTOR2) override;
	virtual D3DXVECTOR3 Rotation() override;
	virtual void Rotation(D3DXVECTOR3) override;

public:

	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P);
	virtual void Render();

	virtual RECT GetHitBox();
	virtual void SetHitBox(RECT hitbox);

	void Set_Object_Mode(Object_Mode obj_mode) { objectMode = obj_mode; }
	
private:
	Sprite * sprite;
};