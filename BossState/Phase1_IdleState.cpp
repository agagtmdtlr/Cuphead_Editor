#include "stdafx.h"
#include "Phase1_IdleState.h"

Phase1_IdleState::Phase1_IdleState()
{
	animation = new Animation();
	wstring shaderPath = Shaders + L"008_Sprite.fx";
	{
		Clip* clip = new Clip(PlayMode::Loop);
		for (int i = 1; i < 27; i++)
		{
			wstring image_path = Textures + L"cuphead/pipe/phase1/Idle/clown_bumper_move_";
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

Phase1_IdleState::~Phase1_IdleState()
{
	SAFE_DELETE(animation);

}

void Phase1_IdleState::handleInput(Boss * boss)
{
}

void Phase1_IdleState::Enter(Boss * boss)
{
}

void Phase1_IdleState::Update(Boss * boss, D3DXMATRIX & V, D3DXMATRIX & P)
{
}

void Phase1_IdleState::Render()
{
}
