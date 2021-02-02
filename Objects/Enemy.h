#pragma once
#include "Object.h"


enum class EnemyParry
{
	normal,
	parry	
};

class EnemyBullet : public Object
{
public:

	EnemyBullet(Grid * grid_, Object_Desc desc, SceneValues * values);
	virtual ~EnemyBullet();

public:

	virtual D3DXVECTOR2 Position() override;
	virtual void Position(D3DXVECTOR2) override;
	virtual D3DXVECTOR2 Scale() override;
	virtual void Scale(D3DXVECTOR2) override;
	virtual D3DXVECTOR3 Rotation() override;
	virtual void Rotation(D3DXVECTOR3) override;

public:

	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P) override;
	virtual void Render() override;

protected:
	// distinguish this enemy bullet is can parry or not
	EnemyParry parry = EnemyParry::normal;
};