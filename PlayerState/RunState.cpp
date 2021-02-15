#include "stdafx.h"
#include "RunState.h"

RunState::RunState()
{
	animation = new Animation();
	wstring shaderFile = Shaders + L"008_Sprite.fx";
	{

		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 17; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Run/Normal/cuphead_run_00" + (i<10?L"0":L"") + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.05f);
		}		
		animation->AddClip(clip);
	}
	{

		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 17; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Run/Shooting/Straight/cuphead_run_shoot_00" + (i < 10 ? L"0" : L"") + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.05f);
		}
		animation->AddClip(clip);
	}
	{

		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 17; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Run/Shooting/Diagonal_Up/cuphead_run_shoot_diagonal_up_00" + (i < 10 ? L"0" : L"") + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.05f);
		}
		animation->AddClip(clip);
	}
}

void RunState::handleInput(Player * player)
{
	OnGroundState::handleInput(player);
}

void RunState::Enter(Player * player)
{
	OnGroundState::Enter(player);
}

void RunState::Update(Player * player, D3DXMATRIX & V, D3DXMATRIX & P)
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
	bool bUp = false;
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
		if (Key->Press(VK_UP))
		{
			bUp = true;
		}
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
	player->animation->Play(bShoot ? (bUp ? 2 : 1 ) : 0);
	player->animation->Update(V, P);
}

void RunState::Render()
{
	animation->Render();
}
