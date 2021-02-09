#include "stdafx.h"
#include "OnAirState.h"

OnAirState::OnAirState()
{
}

void OnAirState::handleInput(Player * player)
{
	State * newState = NULL;

	// OnAirState
	if (player->isDamaged == true)
	{
		newState = (State*)player->damageState;
	}
	else if (player->bOnGround == true) // landed on ground
	{
		newState = (State*)player->idleState;
	}	
	else if (Key->Down('Z') && player->isCanParry)
	{
		newState = (State*)player->parryState;
	}
	else if (Key->Down(VK_LSHIFT) && player->isCanDash) // DashState
	{
		newState = (State*)player->dashState;
	}
	else if (player->bOnGround == false) // Jump
	{
		newState = (State*)player->jumpState;
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

void OnAirState::Enter(Player * player)
{
	D3DXVECTOR2 scale = player->animation->Scale();
	D3DXVECTOR3 rotation = player->animation->Rotation();
	D3DXVECTOR2 position = player->animation->Position();

	animation->Scale(scale);
	animation->Rotation(rotation);
	animation->Position(position);

	player->animation = animation;
}

void OnAirState::Update(Player * player, D3DXMATRIX & V, D3DXMATRIX & P)
{
	D3DXVECTOR2 position = player->animation->Position();
	D3DXVECTOR3 rotation = player->animation->RotationDegree();

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

	player->animation->RotationDegree(rotation);
	player->animation->Position(position);
	player->animation->Play(0);
	player->animation->Update(V, P);
}

void OnAirState::Render()
{
	animation->Render();
}
