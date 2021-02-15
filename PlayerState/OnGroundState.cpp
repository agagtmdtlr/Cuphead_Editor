#include "stdafx.h"	
#include "OnGroundState.h"
#include "Objects/Player.h"


OnGroundState::OnGroundState()
{
	
}

void OnGroundState::handleInput(Player * player)
{
	State * newState = NULL;

	// OnAirState
	if (player->isDamaged == true)
	{
		newState = (State*)player->damageState;
	}
	else if (player->bOnGround == false) // Jump
	{		
		newState = (State*)player->jumpState;
	}
	// OnGroudState
	else if (Key->Press(VK_DOWN)) // DuckState
	{
		newState = (State*)player->duckState;
	}
	else if (Key->Press(VK_LSHIFT)) // DashState
	{
		newState = (State*)player->dashState;
	}
	else if (Key->Press('C')) // AimState
	{
		newState = (State*)player->aimState;
	}
	else if (Key->Press(VK_LEFT) || Key->Press(VK_RIGHT)) // RunState
	{
		newState = (State*)player->runState;
	}
	else // IdleState
	{
		newState = (State*)player->idleState;
	}

	// state change
	if (newState != player->state)
	{
		player->state = newState;
		player->state->Enter(player);
	}
	else // state not change
	{
		player->state = newState;
	}
	
}

void OnGroundState::Enter(Player * player)
{
	D3DXVECTOR2 scale = player->animation->Scale();
	D3DXVECTOR3 rotation = player->animation->Rotation();
	D3DXVECTOR2 position = player->animation->Position();

	animation->Scale(scale);
	animation->Rotation(rotation);
	animation->Position(position);

	player->animation = animation;
	
}

void OnGroundState::Update(Player * player, D3DXMATRIX & V, D3DXMATRIX & P)
{

	D3DXVECTOR2 position = player->animation->Position();
	D3DXVECTOR3 rotation = player->animation->RotationDegree();
	//rotation.z = zDegree;


	if (!player->bOnGround)
	{
		player->velocity += player->gravity * Timer->Elapsed();
	}
	else
	{
		player->velocity = 0;
	}
	//player->velocity = 0;

	position.y += player->velocity;

	bool bMove = false;
	bool bShoot = false;
	bool bDash = false;
	D3DXVECTOR2 movement;
	movement = player->moveDir * player->moveSpeed * Timer->Elapsed() * player->cosVel;



	if (Key->Press('A'))
	{
		bMove = true;
		player->yDegree = 180;
		rotation.z = 360.0f - player->zDegree;
	}
	else if (Key->Press('D'))
	{
		bMove = true;
		player->yDegree = 0;
		rotation.z = player->zDegree;
	}

	if (Key->Press('X'))
	{
		bShoot = true;
	}	
	if (Key->Press(VK_LSHIFT) && bDash == false) // 일정시간 고정된 거리를 빠른 속도로 이동.
	{
		bDash = true;	
	}

	if (bMove = true)
	{
		position += movement * (float)(player->yDegree == 0.0f ? +1 : -1);
	}

	if ((int)player->yDegree == 180)
	{
		rotation.z = 360.0f - player->zDegree;
	}
	else
	{
		rotation.z = player->zDegree;
	}
	rotation.y = player->yDegree;
	//position.y -= 0.5f;

	player->animation->Position(position);
	player->animation->RotationDegree(rotation);

	player->animation->Play(bMove ? 1 : 0);
	player->animation->Update(V, P);
}

void OnGroundState::Render()
{
	animation->Render();
}
