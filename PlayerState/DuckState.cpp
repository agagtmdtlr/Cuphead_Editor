#include "stdafx.h"
#include "DuckState.h"

DuckState::DuckState()
	:duckTime(0.7f)
{
	animation = new Animation();
	wstring shaderFile = Shaders + L"008_Sprite.fx";
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 8; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Duck/Idle/cuphead_duck_00" + (i < 10 ? L"0" : L"") + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.1f);
		}
		animation->AddClip(clip);
	}
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 6; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Duck/Idle/cuphead_duck_idle_00" + (i < 10 ? L"0" : L"") + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.1f);
		}
		animation->AddClip(clip);
	}
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 4; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Duck/Shoot/cuphead_duck_shoot_00" + (i < 10 ? L"0" : L"") + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.05f);
		}
		animation->AddClip(clip);
	}
}

void DuckState::handleInput(Player * player)
{
	State * newState = NULL;

	// OnAirState
	if (player->isDamaged == true)
	{
		newState = (State*)player->damageState;
	}
	else if (Key->Press('Z') || player->bOnGround == false) // Jump
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

void DuckState::Enter(Player * player)
{
	OnGroundState::Enter(player);
	duckCurrentTime = 0;
}

void DuckState::Update(Player * player, D3DXMATRIX & V, D3DXMATRIX & P)
{
	D3DXVECTOR2 position = player->animation->Position();
	D3DXVECTOR3 rotation = player->animation->RotationDegree();
	//rotation.z = zDegree;

	if (!player->bOnGround)
	{
		position.y += 2;
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
	
	if (Key->Down('Z'))
	{
		player->StartJump();
		position.y += 3;

	}

	if (Key->Press(VK_LEFT))
	{
		bMove = true;
		player->yDegree = 180;
		rotation.z = 360.0f - player->zDegree;
	}
	else if (Key->Press(VK_RIGHT))
	{
		bMove = true;
		player->yDegree = 0;
		rotation.z = player->zDegree;
	}

	if (Key->Press('X'))
	{
		bShoot = true;
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

	bool bDucking = false;

	if (duckCurrentTime <= duckTime)
	{
		bDucking = true;
		duckCurrentTime += Timer->Elapsed();
	}

	player->animation->Position(position);
	player->animation->RotationDegree(rotation);

	player->animation->Play(bDucking ? 0 : (bShoot ? 2 : 1 ));
	player->animation->Update(V, P);
}

void DuckState::Render()
{
	animation->Render();
}
