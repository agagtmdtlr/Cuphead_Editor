#pragma once
#include "Object.h"
#include "Enemy.h"

class Duck : public EnemyBullet
{
	friend class DuckPool;
public:
	Duck(Grid * grid_, Object_Desc desc, SceneValues * values);
	~Duck();
	void Update(D3DXMATRIX & V, D3DXMATRIX & P) override;
	void Render() override;

	bool InUse() { return inUse; }

	

	virtual void SetHitBox(RECT hitbox) override;


private:
	bool inUse;
	Animation * animation;
	int play_number = 0;
	Duck * next;

	float x = 0;
};


class DuckPool : public Object
{
private:
	Duck * ducks[6];
	Duck * nextSpawnDuck;
	float createTime;

public:
	DuckPool(Grid * grid_, Object_Desc desc, SceneValues * values);
	~DuckPool();

	void Update(D3DXMATRIX & V, D3DXMATRIX & P) override;
	void Render() override;
};