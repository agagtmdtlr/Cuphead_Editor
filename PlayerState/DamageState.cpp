#include "stdafx.h"
#include "DamageState.h"

DamageState::DamageState()
{
	animation = new Animation();
	wstring shaderFile = Shaders + L"008_Sprite.fx";
	{
		Clip* clip = new Clip(PlayMode::Loop, 1.0f);
		for (int i = 1; i < 7; i++)
		{
			clip->AddFrame(new Sprite(Textures + L"cuphead/player_origin/Hit/Ground/cuphead_hit_000" + to_wstring(i) + L".png",
				shaderFile, RenderType::center_bottom), 0.1f);
		}
		animation->AddClip(clip);
	}
}

void DamageState::handleInput(Player * player)
{
	State * newState = NULL;
	if(damageEnd == false)
	{ 
		newState = (State*)player->damageState;
	}
	else if (player->bOnGround == true)
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

void DamageState::Enter(Player * player)
{
	D3DXVECTOR2 scale = player->animation->Scale();
	D3DXVECTOR3 rotation = player->animation->Rotation();
	D3DXVECTOR2 position = player->animation->Position();

	animation->Scale(scale);
	animation->Rotation(rotation);
	animation->Position(position);

	player->animation = animation;

	damageCurrentTime = 0;
	damageEnd = false;

	player->isDamaged = false;

}

void DamageState::Update(Player * player, D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (animation->GetClip()->CurrentFrame() >= 5)
	{
		damageCurrentTime += Timer->Elapsed();
		if (damageCurrentTime >= 0.2f)
		{
			damageEnd = true;
		}
	}

	animation->Update(V, P);
}

void DamageState::Render()
{

	animation->Render();
}
