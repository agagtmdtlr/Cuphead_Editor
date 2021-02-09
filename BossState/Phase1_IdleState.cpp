#include "stdafx.h"
#include "Phase1_IdleState.h"

Phase1_IdleState::Phase1_IdleState()
{
	animation = new Animation();
	wstring shaderPath = Shaders + L"008_Sprite.fx";
	// turn direction 0
	{
		Clip* clip = new Clip(PlayMode::End);
		for (int i = 1; i < 14; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Idle/clown_bumper_move_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.1f);
		}		
		animation->AddClip(clip);
	}
	// turn direction 1
	{
		Clip* clip = new Clip(PlayMode::End);
		for (int i = 1; i < 14; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Idle/clown_bumper_move_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.1f);
		}
		animation->AddClip(clip);
	}
	animation->Position(0, 0);
	animation->Scale(1, 1);
	animation->Rotation(0, 0, 0);

	animation->Play(animation_index);
}

Phase1_IdleState::~Phase1_IdleState()
{
	SAFE_DELETE(animation);

}

void Phase1_IdleState::handleInput(Boss * boss)
{
	if (idleEnd == true)
	{
		boss->currentState = 2; // attack idle;
		boss->state[boss->currentState]->Enter(boss);
	}
}

void Phase1_IdleState::Enter(Boss * boss)
{
	BossState::Enter(boss);

	idleEnd = false;
	idle_time = Math::Random(0, 1) + 2.0f; // 2 ~ 3 초

	turn_speed = 100.0f; // 100px ~ 150px 이동;
	turn_direction = { 1, 0 }; // 현재 보스 방향과 같은 방향으로 이동

	animate_time = 0;
	current_time = 0;
	
	animation_index = 0;

	animation->Play(0);
}

void Phase1_IdleState::Update(Boss * boss, D3DXMATRIX & V, D3DXMATRIX & P)
{
	int currentframe = (int)animation->GetClip()->CurrentFrame();
	assert(!(currentframe > 10000));
	if (currentframe >= 12) // animation 현재프레임이 마지막인가?
	{
		animate_time += Timer->Elapsed();
		if (animate_time >= 0.05f)
		{
			animate_time = 0;
			int dx = Math::Random(0, 1);			
			turn_direction.x *= (dx == 0 ? 1 : -1); // 다음 턴은 반대 방향으로
			animation->Stop();
			animation_index++;
			animation->Play(animation_index % 2);
		}
	}

	if (idle_time <= current_time) // 마지막 프레임 재생시간이 끝났는가.
	{
		idleEnd = true;
	}
	current_time += Timer->Elapsed();
	D3DXVECTOR2 position = boss->Position();
	position.x += turn_speed * Timer->Elapsed() * turn_direction.x * boss->direction.x;
	boss->Position(position);
	animation->Position(position);
	animation->Update(V, P);
}

void Phase1_IdleState::Render()
{
	animation->Render();
}
