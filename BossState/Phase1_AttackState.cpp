#include "stdafx.h"
#include "Phase1_AttackState.h"

Phase1_AttackState::Phase1_AttackState()
{
	animation = new Animation();
	wstring shaderPath = Shaders + L"008_Sprite.fx";
	// ready clip
	{
		Clip* clip = new Clip(PlayMode::End);
		for (int i = 1; i < 14; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Attack/clown_bumper_dash_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}
		animation->AddClip(clip);
	}
	// dash clip
	{
		Clip* clip = new Clip(PlayMode::Loop);
		for (int i = 14; i < 18; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Attack/clown_bumper_dash_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}
		animation->AddClip(clip);
	}
	// collide wall part1 clip : check die and change death state
	{
		Clip* clip = new Clip(PlayMode::End);
		for (int i = 18; i < 24; i++) 
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Attack/clown_bumper_dash_";
			image_path += (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) + L".png";
			clip->AddFrame(new Sprite(image_path, shaderPath, RenderType::center_bottom), 0.05f);
		}
		animation->AddClip(clip);
	}
	// collid wall part2 clip
	{
		Clip* clip = new Clip(PlayMode::Loop);
		for (int i = 24; i < 36; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Attack/clown_bumper_dash_";
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

Phase1_AttackState::~Phase1_AttackState()
{
	SAFE_DELETE(animation);
}

void Phase1_AttackState::handleInput(Boss * boss)
{
	if (animation_index == 3 && boss->Hp <= 0)
	{
		boss->currentState = 3; // deathstate;
		boss->state[boss->currentState]->Enter(boss);
	}
	if (attackEnd == true)
	{
		boss->currentState = 1; // idlestate

		D3DXVECTOR3 rotation_degree = boss->RotationDegree();
		rotation_degree.y = (int)(rotation_degree.y + 180.0f) % 360;
		boss->RotationDegree(rotation_degree); // 물체 회전
		boss->direction.x *= -1; // 방향 회전

		boss->state[boss->currentState]->Enter(boss);

	}
}

void Phase1_AttackState::Enter(Boss * boss)
{
	BossState::Enter(boss);
	animate_time = 0;
	attackEnd = false;
	animation_index = 0;
	animation->Play(0);
}

void Phase1_AttackState::Update(Boss * boss, D3DXMATRIX & V, D3DXMATRIX & P)
{
	D3DXVECTOR2 position = boss->Position();

	switch (animation_index)
	{
		case 0: // ready clip
		{
			if (animation->GetClip()->CurrentFrame() >= 12)
			{ 
				animate_time += Timer->Elapsed();
				if (animate_time >= 0.05f)
				{
					animation->Stop();
					animation->Play(1); // play dash
					animation_index = 1;
				}		   				
			}
			// 준비 상태에서는 잠깐 뒤로 백한다.
			position.x += boss->direction.x * -1.0f * 200.0f * Timer->Elapsed();
		}
			break;
		case 1: // dash clip
		{
			RECT box = animation->GetSprite()->BoundBox();
			if ((box.left <= Wall.x && boss->direction.x == -1)
				||
				(box.right >= Wall.y && boss->direction.x == 1))
			{
				animation->Stop();
				animation->Play(2); // player coll part1
				animation_index = 2;
			}
			position.x += boss->direction.x * 400.0f * Timer->Elapsed();
		}
			break;
		case 2: // collide wall clip part1
		{
			if (animation->GetClip()->CurrentFrame() >= 5)
			{
				animate_time += Timer->Elapsed();
				if (animate_time >= 0.05f)
				{
					animation->Stop();
					animation->Play(3); // play dash
					animation_index = 3;
				}
			}
		}
			
			break;
		case 3: // collide wall clip part2
		{
			if (animation->GetClip()->CurrentFrame() >= 5)
			{
				animate_time += Timer->Elapsed();
				if (animate_time >= 0.05f)
				{
					attackEnd = true;
					animation->Stop();
				}
			}
		}
			break;
		default:
			break;
	}

	
	boss->grid->Move(boss,position);
	animation->Position(position);
	animation->Update(V, P);
}

void Phase1_AttackState::Render()
{
	animation->Render();
}
