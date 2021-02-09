#include "stdafx.h"
#include "ParryState.h"

ParryState::ParryState()
	:parryTime(0.8f)
{
	animation = new Animation();
	wstring shaderFile = Shaders + L"008_Sprite.fx";
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 9; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Parry/Hand/cuphead_parry_00" + (i < 10 ? L"0" : L"") + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.1f);
		}
		animation->AddClip(clip);
	}
}

void ParryState::handleInput(Player * player)
{
	State * newState = NULL;

	
	if (player->isDamaged == true)
	{
		newState = (State*)player->damageState;
	}
	else if (player->bOnGround == true) // landed on ground
	{
		newState = (State*)player->idleState;
	}
	else if (player->bOnGround == false && (parryCurrentTime > parryTime)) // Jump
	{
		newState = (State*)player->jumpState;
	}
	else
	{
		newState = (State*)player->parryState;
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

void ParryState::Enter(Player * player)
{
	OnAirState::Enter(player);
	parryCurrentTime = 0;
	player->isCanParry = false;
}

void ParryState::Update(Player * player, D3DXMATRIX & V, D3DXMATRIX & P)
{	
	D3DXVECTOR2 position = player->animation->Position();
	D3DXVECTOR3 rotation = player->animation->RotationDegree();

	bool bMove = false;
	bool bShoot = false;
	bool bDash = false;
	D3DXVECTOR2 movement;
	movement = player->moveDir * player->moveSpeed * Timer->Elapsed() * player->cosVel;
	
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
	parryCurrentTime += Timer->Elapsed();

	player->animation->RotationDegree(rotation);
	player->animation->Position(position);
	player->animation->Play(0);
	player->animation->Update(V, P);
}

void ParryState::Render()
{
	animation->Render();
}
