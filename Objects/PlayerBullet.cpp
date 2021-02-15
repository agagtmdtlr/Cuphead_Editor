#include "stdafx.h"
#include "PlayerBullet.h"


int PlayerBullet::bulletNumber = 0;

PlayerBullet::PlayerBullet(Grid * grid, Object_Desc desc, SceneValues * values)
	:Object(grid, desc, values)
{
	for (int i = 0; i < 3; i++)
		animation[i] = new Animation();
	wstring shaderPath = Shaders + L"008_Sprite.fx";

	// peashot
	{
		wstring texturePath = Textures + L"cuphead/player/bullet/peashot/";
		
		// peashot idle
		{
			Clip * clip = new Clip(PlayMode::Loop);

			for (int i = 1; i < 7; i++)
			{
				Sprite* sprite = new Sprite(
					texturePath + L"weapon_peashot_main_000" + to_wstring(i) + L".png", shaderPath
				);
				sprite->Scale(0.1f, 0.1f);
				sprite->BoundTextureSize(20.0f, 20.0f);
				clip->AddFrame(sprite, 0.05f);
			}
			animation[0]->AddClip(clip);
		}
		// peashot death
		{
			Clip * clip = new Clip(PlayMode::End);

			for (int i = 1; i < 7; i++)
				clip->AddFrame(new Sprite(
					texturePath + L"weapon_peashot_death_000" + to_wstring(i) + L".png", shaderPath
				), 0.05f);
			animation[0]->AddClip(clip);
		}
	}
	animation[0]->Scale(0.5f, 0.5f);

	// homing
	{
		wstring texturePath = Textures + L"cuphead/player/bullet/homing/";
		
		// homing idle
		{
			Clip * clip = new Clip(PlayMode::Loop);

			for (int i = 1; i < 7; i++)
			{
				Sprite * sprite = new Sprite(
					texturePath + L"weapon_homing_000" + to_wstring(i) + L".png", shaderPath
				);
				sprite->Scale(0.5f, 0.5f);
				sprite->BoundTextureSize(20.0f, 20.0f);
				clip->AddFrame(sprite, 0.05f);
			}
			animation[1]->AddClip(clip);
		}
		// homing death
		{
			Clip * clip = new Clip(PlayMode::End);

			for (int i = 1; i < 5; i++)
				clip->AddFrame(new Sprite(
					texturePath + L"weapon_homing_death_000" + to_wstring(i) + L".png", shaderPath
				), 0.05f);
			animation[1]->AddClip(clip);
		}
	}
	animation[1]->Scale(0.5f, 0.5f);

	// spread
	{
		wstring texturePath = Textures + L"cuphead/player/bullet/spread/";

		// homing idle
		{
			Clip * clip = new Clip(PlayMode::Loop);

			for (int i = 1; i < 5; i++)
			{
				Sprite * sprite = new Sprite(
					texturePath + L"spread_large_idle_000" + to_wstring(i) + L".png", shaderPath
				);
				sprite->Scale(0.5f, 0.5f);
				sprite->BoundTextureSize(20.0f, 20.0f);
				clip->AddFrame(sprite, 0.05f);
			}
			animation[2]->AddClip(clip);
		}
		// homing death
		{
			Clip * clip = new Clip(PlayMode::End);

			for (int i = 1; i < 7; i++)
				clip->AddFrame(new Sprite(
					texturePath + L"spread_large_hit_000" + to_wstring(i) + L".png", shaderPath
				), 0.05f);
			animation[2]->AddClip(clip);
		}
	}
	animation[2]->Scale(0.5f, 0.5f);

	bullet_type = PlAYER_BULLET_TYPE::peashot;
	bulletIndex = bulletNumber++;

	animation[0]->DrawBound(true);
}

PlayerBullet::~PlayerBullet()
{
	for (int i = 0; i < 3; i++)
		SAFE_DELETE(animation[i]);
}

void PlayerBullet::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	int current_type = (int)bullet_type;

	animation[current_type]->Play(0);
	switch (bullet_type)
	{
	case PlAYER_BULLET_TYPE::peashot:
		break;
	case PlAYER_BULLET_TYPE::homingshot:
		break;
	case PlAYER_BULLET_TYPE::spreadshot:
		break;
	default:
		break;
	}

	animation[current_type]->Position(position);
	animation[current_type]->Rotation(rotation);
	animation[current_type]->Scale(scale);


	animation[current_type]->Update(V, P);
}

bool PlayerBullet::bUpdate(D3DXMATRIX & V, D3DXMATRIX & P)
{
	int current_type = (int)bullet_type;

	if (!InScreen())
	{
		inUse = false;
		return false;
	}

	switch (bullet_type)
	{
	case PlAYER_BULLET_TYPE::peashot:
		if (inUse)
		{
			D3DXVECTOR2 newPos;
			newPos = position + direction * 800.0f * Timer->Elapsed();
			grid->Move(this, newPos);
		}
		else
		{
			if (animation[current_type]->GetClip()->CurrentFrame() >= 5)
			{
				animation[current_type]->Stop();
				return false;
			}
		}
		break;
	case PlAYER_BULLET_TYPE::homingshot:
		break;
	case PlAYER_BULLET_TYPE::spreadshot:
		break;
	default:
		break;
	}		

	animation[current_type]->Position(position);
	animation[current_type]->Rotation(rotation);
	animation[current_type]->Scale(scale);

	animation[current_type]->Update(V, P);


	return true;
}

void PlayerBullet::Render()
{
	animation[(int)bullet_type]->Render();
	ImGui::Text("%d %d", gridX, gridY);

}

RECT PlayerBullet::GetHitBox()
{
	return animation[(int)bullet_type]->GetSprite()->BoundBox();
}

Sprite * PlayerBullet::GetSprite()
{
	return animation[(int)bullet_type]->GetSprite();
}

void PlayerBullet::BoundCollision(Object_Desc & desc)
{
	animation[(int)bullet_type]->Play(1);
	inUse = false;
	object_desc.b_bound_coll = false;
	object_desc.b_line_coll = false;

}

void PlayerBullet::LineCollision(D3DXVECTOR2 & p1, D3DXVECTOR2 & p2)
{
	animation[(int)bullet_type]->Play(1);
	inUse = false;
	object_desc.b_bound_coll = false;
	object_desc.b_line_coll = false;
}

PlayerBulletPool::PlayerBulletPool(Grid * grid, D3DXVECTOR2 position_, D3DXVECTOR2 scale_, Object_Desc desc, SceneValues * values)
	:Object(grid, desc, values),waitTime(0.1f),bullet_type(PlAYER_BULLET_TYPE::peashot)
{
	position = position_;
	scale = scale_;
	rotation = { 0,0,0 };

	createTime[0] = 0.2f;
	createTime[1] = 0.5f;
	createTime[2] = 0.2f;

	Object_Desc bullet_desc;
	bullet_desc.b_bound_coll = true;
	bullet_desc.b_line_coll = true;
	bullet_desc.b_render = true;
	bullet_desc.group = OBJECT_GROUP::player;
	bullet_desc.label = OBJECT_LABEL::player_bullet;
	bullet_desc.obj_mode = Object_Mode::Play;
	for (int i = 0; i < 10; i++)
	{
		bulletDead.push_back(new PlayerBullet(grid, bullet_desc, values));
	}

	direction = { 1,0 };
}

PlayerBulletPool::~PlayerBulletPool()
{
	for (auto b : bulletLive)
	{
		SAFE_DELETE(b);
	}
	for (auto b : bulletDead)
	{
		SAFE_DELETE(b);
	}
}

void PlayerBulletPool::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	
	if (Key->Press('X'))
	{
		float times = Timer->Elapsed();
		waitTime += times;
	}

	for (auto bullet = bulletLive.begin(); bullet != bulletLive.end();)
	{
		if (!(*bullet)->bUpdate(V, P))
		{
			bulletDead.push_back(*bullet);
			grid->Remove((Object*)*bullet);
			bullet = bulletLive.erase(bullet);
		}
		else
		{
			bullet++;
		}
	}

	if (waitTime >= createTime[(int)bullet_type])
	{
		waitTime = 0.0f;
		Create(V, P);
	}
	updateCount++;
}

void PlayerBulletPool::Render()
{
	for (auto bullet : bulletLive)
	{
		if (next)
		{
			auto b = dynamic_cast<PlayerBullet*>(next);
			if (b)
				ImGui::Text("%d", b->bulletIndex);
		}
		//ImGui::Text("%f %f", bullet->position.x, bullet->position.y);

		bullet->Render();
	}
}

void PlayerBulletPool::Create(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (bulletDead.empty())
		return;

	PlayerBullet * & bullet = bulletDead.back();
	
	bullet->inUse = true;
	bullet->position = position;
	bullet->rotation = rotation;
	bullet->scale = { 0.5f,0.5f };
	bullet->bullet_type = bullet_type; // ¹«½¼ ÃÑ¾ËÀÎÁö
	bullet->direction = direction; // ¹ß»ç ¹æÇâ

	bullet->prev = 0;
	bullet->next = 0;

	bullet->object_desc.b_bound_coll = true;
	bullet->object_desc.b_line_coll = true;

	switch (bullet_type)
	{
	case PlAYER_BULLET_TYPE::peashot:
		break;
	case PlAYER_BULLET_TYPE::homingshot:
		break;
	case PlAYER_BULLET_TYPE::spreadshot:
		break;
	default:
		break;
	}

	bullet->object_desc.b_bound_coll = true;

	grid->Add(bullet);
	bullet->Update(V, P);

	bulletLive.push_back(bullet);
	bulletDead.pop_back();

	before = bullet;

	createCount++;
}
