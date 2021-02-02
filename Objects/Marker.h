#pragma once
#include "Object.h"

class Marker : public Object
{
	friend class Grid;
	friend class Liner;
	friend class Sonic;

public:
	Marker(Grid* grid_, wstring shaderFile, D3DXVECTOR2 start, Object_Desc desc, SceneValues * values);
	~Marker();

	void Move(D3DXVECTOR2 position_);

public:
	virtual D3DXVECTOR2 Position() { return position; }
	virtual void Position(D3DXVECTOR2) override;
	virtual D3DXVECTOR2 Scale() override;
	virtual void Scale(D3DXVECTOR2)override;
	virtual D3DXVECTOR3 Rotation() override;
	virtual void Rotation(D3DXVECTOR3)override;

	virtual RECT GetHitBox() override;

public:

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;
	virtual void Render() override;

	Clip* GetClip() { return clip; }	

private:
	Clip* clip;

};

