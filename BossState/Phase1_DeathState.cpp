#include "stdafx.h"
#include "Phase1_DeathState.h"

Phase1_DeathState::Phase1_DeathState()
{
	animation = new Animation();
	wstring shaderPath = Shaders + L"008_Sprite.fx";
	{
		Clip* clip = new Clip(PlayMode::End);		
		for (int i = 1; i < 15; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Die/clown_bumper_death_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}
		animation->AddClip(clip);
	}
	{
		Clip* clip = new Clip(PlayMode::Loop);
		for (int i = 1; i < 4; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Die/clown_bumper_death_fall_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}
		animation->AddClip(clip);
	}

	animation->Position(0, 0);
	animation->Scale(1, 1);
	animation->Rotation(0, 0, 0);

	animation->Play(0);
}

Phase1_DeathState::~Phase1_DeathState()
{	
}

void Phase1_DeathState::handleInput(Boss * boss)
{
}

void Phase1_DeathState::Enter(Boss * boss)
{
	BossState::Enter(boss);
	animate_time = 0;
	animation_index = 0;
}

void Phase1_DeathState::Update(Boss * boss, D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (animation->GetClip()->CurrentFrame() >= 13 && animation_index == 0)
	{
		animate_time += Timer->Elapsed();
		if (animate_time >= 0.05f)
		{
			animation_index++;
			animation->Play(animation_index);
		}
	}

	if (animation_index == 1) // if fall
	{
		D3DXVECTOR2 position = boss->Position();
		position.y -= 500.0f * Timer->Elapsed();
		boss->Position(position);
		animation->Position(position);
	}
	

	animation->Update(V, P);
}

void Phase1_DeathState::Render()
{
	animation->Render();
}
