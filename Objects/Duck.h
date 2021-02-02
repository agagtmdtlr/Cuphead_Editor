#pragma once
#include "Object.h"
#include "Enemy.h"

class Duck : public EnemyBullet
{
	friend class DuckPool;
public:
	Duck(Grid * grid_, Object_Desc desc);
	~Duck();
	void Update(D3DXMATRIX & V, D3DXMATRIX & P) override;
	void Render() override;

	bool InUse();

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
	Duck * ducks;
};