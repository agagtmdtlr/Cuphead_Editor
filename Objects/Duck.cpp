#include "stdafx.h"
#include "Duck.h"

Duck::Duck(Grid * grid_, Object_Desc desc)
	:EnemyBullet(grid_, desc)
{
	animation = new Animation[2];
	wstring spritePath = Textures + L"cuphead/pipe/phase1/duck/duck_yellow.png";
	{
		Clip * clip = new Clip(PlayMode::Loop);
		for (int i = 0; i < 6; i++)
		{
			Sprite * sprite = new Sprite(spritePath, Shaders + L"008_Sprite.fx",
				i * 150, 0, i * 150 + 150, 400,
				RenderType::left_bottom);
			sprite->BoundTextureSize(150, 100);
			sprite->BoundPosition(0, -150);
			clip->AddFrame(sprite, 0.1f);
		}
		for (int i = 0; i < 10; i++)
		{
			Sprite * sprite = new Sprite(spritePath, Shaders + L"008_Sprite.fx",
				(i % 3) * 300, (i / 3) * 400 + 400, (i % 3) * 300 + 300, (i / 3) * 400 + 800,
				RenderType::left_bottom);
			clip->AddFrame(sprite, 0.05f);
		}

		animation[0].AddClip(clip);		
	}
	{
		Clip * clip = new Clip(PlayMode::Loop);
		for (int i = 0; i < 10; i++)
		{
			clip->AddFrame(new Sprite(spritePath, Shaders + L"008_Sprite.fx",
				(i % 3) * 300, (i / 3) * 400 + 400, (i % 3) * 300 + 300, (i / 3) * 400 + 800,
				RenderType::left_bottom), 0.1f);
		}
		animation[0].AddClip(clip);
	}

	position = { 0,0 };
	scale = { 1,1 };

	animation[0].Position({ 0,0 });
	animation[0].Scale({ 1,1 });
	animation[0].Rotation(D3DXVECTOR3(0,0,0));

	animation[0].Play(0);

	animation[0].DrawBound(true);
}

Duck::~Duck()
{
	SAFE_DELETE_ARRAY(animation);
}

void Duck::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{

	if (inUse)
	{
		x += Timer->Elapsed();
		float y_weight = 50.0f * sinf(x);
		D3DXVECTOR2 sin_pos = position;
		sin_pos.y += y_weight;

		animation[play_number].Position(sin_pos);
		animation[play_number].Update(V, P);
	}
}

void Duck::Render()
{
	if (inUse)
	{
		animation[play_number].Render();
	}
}

bool Duck::InUse()
{
	return false;
}

void Duck::SetHitBox(RECT hitbox)
{
}


