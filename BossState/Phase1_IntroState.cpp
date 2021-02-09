#include "stdafx.h"
#include "Phase1_IntroState.h"

Phase1_IntroState::Phase1_IntroState()
{
	animation = new Animation();
	wstring shaderPath = Shaders + L"008_Sprite.fx";
	{
		Clip* clip = new Clip(PlayMode::Loop);
		for (int i = 1; i < 32; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Intro/clown_intro_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}
		for (int i = 1; i < 27; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Idle/clown_bumper_move_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}
		for (int i = 1; i < 36; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Attack/clown_bumper_dash_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}
		for (int i = 1; i < 15; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Die/clown_bumper_death_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}
		animation->AddClip(clip);		
	}
	
	animation->Position(0, 0);
	animation->Scale(1, 1);
	animation->Rotation(0, 0, 0);

	animation->Play(0);

	intro_time = 0.05f;
	animate_time = 0;
}

Phase1_IntroState::~Phase1_IntroState()
{
	SAFE_DELETE(animation);
}

void Phase1_IntroState::handleInput(Boss * boss)
{
	if (introEnd == true) // 인트로가 끝났다.
	{
		boss->currentState = 1; // Idle state로 변경한다.
		boss->state[boss->currentState]->Enter(boss);
	}
}

void Phase1_IntroState::Enter(Boss * boss)
{
	BossState::Enter(boss);
	animate_time = 0;
	introEnd = false;
}

void Phase1_IntroState::Update(Boss * boss, D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (boss->object_desc.obj_mode == Object_Mode::Play)
	{
		if (animation->GetClip()->CurrentFrame() >= 31) // animation 현재프레임이 마지막인가?
		{
			if (intro_time <= animate_time) // 마지막 프레임 재생시간이 끝났는가.
			{
				introEnd = true;  // 재생이 끝났음
			}
			animate_time += Timer->Elapsed();
		}
		animation->Update(V, P);

	}
	else
	{
		animation->DrawBound(true);
		animation->Position(boss->Position());
		animation->Update(V, P);
		animation->Stop();
	}
	

}

void Phase1_IntroState::Render()
{
	animation->Render();
}
