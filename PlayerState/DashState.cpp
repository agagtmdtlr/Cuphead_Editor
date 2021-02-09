#include "stdafx.h"
#include "DashState.h"

DashState::DashState()
	:dashReadyTime(0.4f)
	, dashEndTime(0.5f)
	, dashSpeed(2.0f)
{
	animation = new Animation();
	wstring shaderFile = Shaders + L"008_Sprite.fx";
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 0; i < 2; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Dash/Air/"+ to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.2f);
		}
		animation->AddClip(clip);
	}
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 7; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Dash/Ground/cuphead_dash_00" + (i < 10 ? L"0" : L"") + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.05f);
		}
		animation->AddClip(clip);
	}
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 7; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Dash/Air/cuphead_dash_air_00" + (i < 10 ? L"0" : L"") + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.05f);
		}
		animation->AddClip(clip);
	}
}

void DashState::handleInput(Player * player)
{
	State * newState = NULL;

	// OnAirState
	if (player->isDamaged == true)
	{
		newState = (State*)player->damageState;
	}
	else if (dashCurrentTime < dashEndTime)
	{
		newState = (State*)player->dashState;
	}
	else if(player->bOnGround == true)
	{
		newState = (State*)player->idleState;
	}
	else if (player->bOnGround == false)
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

void DashState::Enter(Player * player)
{
	D3DXVECTOR2 scale = player->animation->Scale();
	D3DXVECTOR3 rotation = player->animation->Rotation();
	D3DXVECTOR2 position = player->animation->Position();

	animation->Scale(scale);
	animation->Rotation(rotation);
	animation->Position(position);

	player->animation = animation;

	dashCurrentTime = 0;

	player->isCanDash = false;
}

void DashState::Update(Player * player, D3DXMATRIX & V, D3DXMATRIX & P)
{
	D3DXVECTOR2 position = player->animation->Position();
	D3DXVECTOR3 rotation = player->animation->RotationDegree();

	bool bMove = false;
	bool bShoot = false;
	bool bDash = false;
	D3DXVECTOR2 movement;
	movement = player->moveDir * player->moveSpeed * Timer->Elapsed();	   
	
	position += movement * (float)(player->yDegree == 0.0f ? +1 : -1) * dashSpeed;

	if ((int)player->yDegree == 180)
	{
		rotation.z = 360.0f - player->zDegree;
	}
	else
	{
		rotation.z = player->zDegree;
	}
	rotation.y = player->yDegree;
	
	bool bReady = false;

	if (dashCurrentTime <= dashReadyTime)
	{
		bReady = true;		
	}
	dashCurrentTime += Timer->Elapsed();

	player->animation->RotationDegree(rotation);
	player->animation->Position(position);
	player->animation->Play(bReady ? 0 : (player->bOnGround ? 1 : 2));
	player->animation->Update(V, P);
}

void DashState::Render()
{
	animation->Render();
}
