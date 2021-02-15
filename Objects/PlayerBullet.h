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
	friend class PlayerBulletPool;
public:
	PlayerBullet(Grid* grid,Object_Desc desc, SceneValues *values);
	~PlayerBullet();
	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P) override;
	bool bUpdate(D3DXMATRIX & V, D3DXMATRIX & P);

	virtual void Render() override;

	virtual RECT GetHitBox() override;
	virtual class Sprite* GetSprite() override;


	virtual void BoundCollision(Object_Desc & desc);
	virtual void LineCollision(D3DXVECTOR2 & p1, D3DXVECTOR2 & p2);


	bool InUse() { return inUse; }
	
	static int bulletNumber;

public:
	int bulletIndex;
private:

	bool inUse = false;
	Animation * animation[3];
	PlAYER_BULLET_TYPE bullet_type;
	int damage;

	int currentTime;
	int deathTime;

	D3DXVECTOR2 direction;
	D3DXVECTOR2 homingVelocity;
};

class PlayerBulletPool : public Object
{
	friend class Player;
public:
	PlayerBulletPool(Grid* grid, D3DXVECTOR2 position_, D3DXVECTOR2 scale_, Object_Desc desc, SceneValues *values);
	~PlayerBulletPool();
	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P) override;
	virtual void Render() override;

	void Create(D3DXMATRIX & V, D3DXMATRIX & P);

private:
	vector<PlayerBullet *> bulletLive;
	vector<PlayerBullet *> bulletDead;

	PlAYER_BULLET_TYPE bullet_type;

	D3DXVECTOR2 direction;

	float createTime[3];
	float waitTime;

	PlayerBullet * before = 0;

	UINT updateCount = 0;
	UINT createCount = 0;
};