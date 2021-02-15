#include "stdafx.h"
#include "Duck.h"

int Duck::duckNumber = 0;

Duck::Duck(Grid * grid_, Object_Desc desc, SceneValues * values)
	:EnemyBullet(grid_, desc, values)
{
	next = 0;
	// dynamic array;
	animation = new Animation[2];
	// create yellow duck
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
		

		animation[0].AddClip(clip);		
	}
	{
		Clip * clip = new Clip(PlayMode::Loop);
		for (int i = 0; i < 10; i++)
		{
			Sprite * sprite = new Sprite(spritePath, Shaders + L"008_Sprite.fx",
				(i % 3) * 300, (i / 3) * 400 + 400, (i % 3) * 300 + 300, (i / 3) * 400 + 800,
				RenderType::left_bottom);
			clip->AddFrame(sprite, 0.05f);
		}
		animation[0].AddClip(clip);
	}

	// create pink duck
	spritePath = Textures + L"cuphead/pipe/phase1/duck/duck_pink.png";
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


		animation[0].AddClip(clip);
	}
	{
		Clip * clip = new Clip(PlayMode::Loop);
		for (int i = 0; i < 10; i++)
		{
			Sprite * sprite = new Sprite(spritePath, Shaders + L"008_Sprite.fx",
				(i % 3) * 300, (i / 3) * 400 + 400, (i % 3) * 300 + 300, (i / 3) * 400 + 800,
				RenderType::left_bottom);
			clip->AddFrame(sprite, 0.05f);
		}
		animation[0].AddClip(clip);
	}

	position = { 0,0 };
	scale = { 1,1 };
	rotation = { 0,0,0 };

	animation[0].Position(0,0 );
	animation[0].Scale( 1,1 );
	animation[0].Rotation(D3DXVECTOR3(0,0,0));

	animation[0].Play(0);

	animation[0].DrawBound(true);

	play_number = 0;

	this->object_desc.group = OBJECT_GROUP::enemy;

	duckIndex = duckNumber++;
}

Duck::~Duck()
{
	SAFE_DELETE_ARRAY(animation);
}

void Duck::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (object_desc.obj_mode == Object_Mode::Editor)
	{
		animation[play_number].Position(position);
		animation[play_number].Update(V, P);

	}
	else if ((object_desc.obj_mode == Object_Mode::Play))
	{
		if (inUse)
		{
			//sinPosition.x -= Timer->Elapsed() * 200.0;
			//float y_weight = 50.0f * sinf(sinPosition.x * 0.025);
			position = sinPosition;
			//sition.y += y_weight;

			animation[play_number].Position(position);

			animation[play_number].Update(V, P);
		}

	}
}

bool Duck::bUpdate(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (object_desc.obj_mode == Object_Mode::Editor)
	{
		animation[play_number].Position(position);
		animation[play_number].Update(V, P);

		return false;
	}
	else if ((object_desc.obj_mode == Object_Mode::Play))
	{
		if (inUse)
		{
			if (!InScreen())// 
			{
				inUse = false;
				return false;
			}

			sinPosition.x -= Timer->Elapsed() * 200.0;
			float y_weight = 50.0f * sinf(sinPosition.x * 0.025);
			D3DXVECTOR2 newPos = sinPosition;
			newPos.y += y_weight;

			animation[play_number].Position(newPos);			
			animation[play_number].Update(V, P);
			grid->Move(this, newPos);

			return true;
		}

		return false;
	}

	return false;
}

void Duck::Render()
{
	if (inUse)
	{
		animation[play_number].Render();
		//ImGui::Text("%d %d", gridX, gridY);

	}
}





RECT Duck::GetHitBox()
{
	return animation->GetSprite()->BoundBox();
}

void Duck::SetHitBox(RECT hitbox)
{
}
///////////////////////////////////////////////////////////////
// DuckPool
///////////////////////////////////////////////////////////////
DuckPool::DuckPool(Grid * grid_, D3DXVECTOR2 position_, D3DXVECTOR2 scale_, Object_Desc desc, SceneValues * values)
	:Object(grid_, desc, values), waitTime(0), createTime(3.0f)
{
	Object_Desc duck_desc;
	duck_desc.b_bound_coll = true;
	duck_desc.b_line_coll = false;
	duck_desc.b_render = true;
	duck_desc.label = OBJECT_LABEL::duck;
	duck_desc.obj_mode = Object_Mode::Play;


	for (int i = 0; i < 8; i++)
	{
		ducksDead.push_back(new Duck(grid, duck_desc, values));
	}	

	position = position_;
	scale = scale_;
	rotation = { 0,0,0 };
}

DuckPool::~DuckPool()
{

	for (auto d : ducksDead)
	{
		SAFE_DELETE(d);
	}
	for (auto d : ducksLive)
	{
		SAFE_DELETE(d);
	}
}

void DuckPool::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	float times = Timer->Elapsed();
	waitTime += times;
	

	for (auto duck = ducksLive.begin(); duck != ducksLive.end(); )
	{
		if (!(*duck)->bUpdate(V, P))
		{
			ducksDead.push_back(*duck);
			grid->Remove((Object*)*duck);
			duck = ducksLive.erase(duck);
		}
		else
		{
			duck++;
		}
	}


	if (waitTime >= createTime)
	{
		waitTime = 0.0f;
		Create(V, P);
	}
}

void DuckPool::Render()
{
	for (auto duck : ducksLive)
	{
		if (duck->InUse())
			duck->Render();
	}
}

void DuckPool::Create(D3DXMATRIX & V, D3DXMATRIX & P)
{
	Duck * & duck = ducksDead.back();
	duck->inUse = true;
	duck->sinPosition = position;
	duck->rotation = rotation;
	duck->scale = scale;
	duck->object_desc.b_bound_coll = true;
	duck->Update(V, P);

	ducksLive.push_back(duck);
	ducksDead.pop_back();

	grid->Add(duck);
}
