#include "stdafx.h"
#include "Phase2_IntroState.h"


Phase2_IntroState::Phase2_IntroState()
{
	current_time = 0;
	head_rise_time = 0.1f;
	platform = new Animation();
	animation = new Animation();
	head_rise_animation = new Animation();
	wstring shaderPath = Shaders + L"008_Sprite.fx";
	{
		Clip* clip = new Clip(PlayMode::End);
		for (int i = 1; i < 25; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase2/Intro/clown_helium_intro_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.1f);
		}
		for (int i = 1; i < 2; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase2/Idle/body/clown_helium_idle_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}
		/*for (int i = 1; i < 25; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase2/Pipe/clown_helium_pipes_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}*/


		animation->AddClip(clip);
	}	

	{
		Clip* clip = new Clip(PlayMode::End);
		for (int i = 1; i < 6; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase2/Intro/clown_head_rise_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}
		head_rise_animation->AddClip(clip);
	}
	head_rise_animation->Position(0, -200);
	head_rise_animation->Scale(1, 1);
	head_rise_animation->Rotation(0, 0, 0);

	head_rise_animation->Play(0);

	{
		Clip* clip = new Clip(PlayMode::Loop);
		for (int i = 1; i < 2; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase2/Idle/body/clown_helium_platform_bot_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center), 0.05f);
		}
		platform->AddClip(clip);
	}
	platform->Position(0, -200);
	platform->Scale(1, 1);
	platform->Rotation(0, 0, 0);

	platform->Play(0);


	animation->Position(0, -200);
	animation->Scale(1, 1);
	animation->Rotation(0, 0, 0);

	animation->Play(0);

	
}

Phase2_IntroState::~Phase2_IntroState()
{
	SAFE_DELETE(head_rise_animation);
}

void Phase2_IntroState::handleInput(Boss * boss)
{
}

void Phase2_IntroState::Enter(Boss * boss)
{
}

void Phase2_IntroState::Update(Boss * boss, D3DXMATRIX & V, D3DXMATRIX & P)
{
	platform->Position(0,-200-135);
	platform->Update(V, P);
	animation->Update(V, P);

	if (24 == animation->GetClip()->CurrentFrame())
	{
		current_time += Timer->Elapsed();
		if(head_rise_time <= current_time)
			head_rise_animation->Update(V, P);		
	}
	
}

void Phase2_IntroState::Render()
{
	platform->Render();

	if (24 == animation->GetClip()->CurrentFrame())
	{
		if (head_rise_time <= current_time)
			head_rise_animation->Render();
	}
	animation->Render();

}
