#include "stdafx.h"
#include "IdleState.h"

IdleState::IdleState()
{
	animation = new Animation();
	wstring shaderFile = Shaders + L"008_Sprite.fx";
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 6; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Idle/cuphead_idle_000" + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.1f);
		}
		animation->AddClip(clip);
	}
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 4; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Shoot/Straight/cuphead_shoot_straight_00" + (i < 10 ? L"0" : L"") + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.1f);
		}
		animation->AddClip(clip);
	}
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 4; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Shoot/Up/cuphead_shoot_up_00" + (i < 10 ? L"0" : L"") + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.1f);
		}
		animation->AddClip(clip);
	}
	
}

void IdleState::handleInput(Player * player)
{
	OnGroundState::handleInput(player);
}

void IdleState::Enter(Player * player)
{
	__super::Enter(player);
}

void IdleState::Update(Player * player, D3DXMATRIX & V, D3DXMATRIX & P)
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
	bool bUp = false;
	bool bShoot = false;
	bool bDash = false;
	D3DXVECTOR2 movement;
	movement = player->moveDir * player->moveSpeed * Timer->Elapsed() * player->cosVel;

	if (Key->Down('Z'))
	{
		player->StartJump();
		position.y += 3;// 착지 판정 범위에 대한 보정

	}

	if (Key->Press(VK_UP))
	{
		player->shootDir = { 0, 1 };
	}	

	if (Key->Press('X'))
	{
		if (Key->Press(VK_UP))
		{
			bUp = true;
		}
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

	player->animation->Position(position);
	player->animation->RotationDegree(rotation);

	player->animation->Play(bShoot ? (bUp ? 2 : 1) : 0);
	player->animation->Update(V, P);
}

void IdleState::Render()
{
	animation->Render();
}
