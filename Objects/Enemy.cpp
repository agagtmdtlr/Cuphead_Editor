#include "stdafx.h"
#include "Enemy.h"



EnemyBullet::EnemyBullet(Grid * grid_, Object_Desc desc, SceneValues * values)
	:Object(grid_,desc, values)
{
}

EnemyBullet::~EnemyBullet()
{
}

D3DXVECTOR2 EnemyBullet::Position()
{
	return D3DXVECTOR2();
}

void EnemyBullet::Position(D3DXVECTOR2)
{
}

D3DXVECTOR2 EnemyBullet::Scale()
{
	return D3DXVECTOR2();
}

void EnemyBullet::Scale(D3DXVECTOR2)
{
}

D3DXVECTOR3 EnemyBullet::Rotation()
{
	return D3DXVECTOR3();
}

void EnemyBullet::Rotation(D3DXVECTOR3)
{
}

void EnemyBullet::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
}

void EnemyBullet::Render()
{
}


