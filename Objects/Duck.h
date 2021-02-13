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
	bool bUpdate(D3DXMATRIX & V, D3DXMATRIX & P);
	void Render() override;

	bool InUse() { return inUse; }

	
	virtual RECT GetHitBox();
	virtual void SetHitBox(RECT hitbox) override;

	static int duckNumber;


private:
	bool inUse =false;
	int duckIndex;
	Animation * animation;
	int play_number = 0;

	D3DXVECTOR2 sinPosition;

	float x = 0;

};


class DuckPool : public Object
{
private:
	vector<Duck *> ducksLive;
	vector<Duck *> ducksDead;

	Duck * nextSpawnDuck;
	float createTime;
	float waitTime;

public:
	DuckPool(Grid * grid_,D3DXVECTOR2 position_, D3DXVECTOR2 scale_, Object_Desc desc, SceneValues * values);
	~DuckPool();

	void Update(D3DXMATRIX & V, D3DXMATRIX & P) override;
	void Render() override;

	void Create(D3DXMATRIX & V, D3DXMATRIX & P);
};