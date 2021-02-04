#include "stdafx.h"
#include "Phase1_IntroState.h"

Phase1_IntroState::Phase1_IntroState()
{
	animation = new Animation();
	wstring shaderPath = Shaders + L"008_Sprite.fx";
	{
		Clip* clip = new Clip(PlayMode::Loop);
		/*for (int i = 1; i < 32; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Intro/clown_intro_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center), 0.05f);
		}*/
		for (int i = 1; i < 27; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Idle/clown_bumper_move_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center), 0.05f);
		}
		for (int i = 1; i < 36; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Attack/clown_bumper_dash_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center), 0.05f);
		}
		for (int i = 1; i < 15; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Die/clown_bumper_death_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center), 0.05f);
		}
		animation->AddClip(clip);		
	}
	
	animation->Position(0, 0);
	animation->Scale(1, 1);
	animation->Rotation(0, 0, 0);

	animation->Play(0);

}

Phase1_IntroState::~Phase1_IntroState()
{
	SAFE_DELETE(animation);
}

void Phase1_IntroState::handleInput(Boss * boss)
{
	if (animate_time >= intro_time);
}

void Phase1_IntroState::Enter(Boss * boss)
{
}

void Phase1_IntroState::Update(Boss * boss, D3DXMATRIX & V, D3DXMATRIX & P)
{
	animation->Update(V, P);
}

void Phase1_IntroState::Render()
{
	animation->Render();
}
