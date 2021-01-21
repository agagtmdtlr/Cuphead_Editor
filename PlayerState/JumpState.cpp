#include "stdafx.h"
#include "JumpState.h"

JumpState::JumpState()
{
	animation = new Animation();
	wstring shaderFile = Shaders + L"008_Sprite.fx";
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 9; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Jump/Cuphead/cuphead_jump_000" + to_wstring(i) + L".png", shaderFile, RenderType::center_bottom), 0.1f);
		}
		animation->AddClip(clip);
	}
}

void JumpState::handleInput(Player * player)
{
	OnAirState::handleInput(player);
}

void JumpState::Enter(Player * player)
{
	__super::Enter(player);
}

void JumpState::Update(Player * player, D3DXMATRIX & V, D3DXMATRIX & P)
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

	if (Key->Up('Z'))
	{
		player->EndJump();
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
	if (bMove == true)
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

void JumpState::Render()
{
	animation->Render();
}
