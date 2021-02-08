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
	
public:

	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P) override;
	virtual void Render() override;

	virtual void Parry(EnemyParry val) { parry = val; }
	virtual EnemyParry Parry() { return parry; }
protected:
	// distinguish this enemy bullet is can parry or not
	EnemyParry parry = EnemyParry::normal;
};