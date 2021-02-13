#pragma once
#include "Object.h"	

enum class PlAYER_BULLET_TYPE
{
	peashot,
	homingshot,
	spreadshot
};

class PlayerBullet :public Object
{
	// Inherited via Object
	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P) override;
	virtual void Render() override;
};