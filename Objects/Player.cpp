#include "stdafx.h"
#include "Player.h"

Player::Player(Grid* grid_, D3DXVECTOR2 position_, D3DXVECTOR2 scale_, Object_Desc desc, SceneValues * values, RenderType type)
	:Object(grid_,desc, values)
	,moveSpeed(400.0f), focusOffset(-180, -120), bOnGround(false), cosVel(1),
	zDegree(0), yDegree(0), moveDir(1, 0),
	velocity(0), gravity(-1.95f), type(type)
	, startJumpVelocity(0.75)
	, endJumpVelocity(0.5)
	, hitDelay(2.0f)
	, hitSparkTime(0)
	, hittedTime(0)
{
	object_desc.label = OBJECT_LABEL::player;
	object_desc.group = OBJECT_GROUP::player;
	object_desc.b_bound_coll = true;
	object_desc.b_line_coll = true;
	object_desc.b_render = true;


	{
		Object_Desc bullet_desc;
		bulletPool = new PlayerBulletPool(grid, position_, scale_, bullet_desc, values);
	}



	aimState = new AimState();
	dashState = new DashState();
	duckState = new DuckState();
	idleState = new IdleState();
	jumpState = new JumpState();
	onAirState = new OnAirState();
	onGroundState = new OnGroundState();
	parryState = new ParryState();
	runState = new RunState();
	damageState = new DamageState();

	animation = new Animation();

	wstring spriteFile = Textures + L"Metalslug.png";
	wstring shaderFile = Shaders + L"008_Sprite.fx";

	//rendertype
	this->type = RenderType::center;

	state = idleState;

	animation = idleState->animation;

	animation->Rotation(0, 0, 0);
	animation->Position(position_);
	animation->Scale(scale_);
	animation->Play(0);

	animation->DrawBound(true);

	position = position_;
	rotation = { 0,0,0 };
	scale = scale_;
}

Player::~Player()
{
	SAFE_DELETE(animation);
}

void Player::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
		if (object_desc.obj_mode == Object_Mode::Play)
		{
			
			state->handleInput(this);
			state->Update(this, V, P);

			rotation = state->animation->Rotation();
			grid->Move(this, state->animation->Position());
		}
		else if (object_desc.obj_mode == Object_Mode::Editor)
		{
			state->animation->Update(V, P);
		}	

		if (isHitted == true)
		{
			hittedTime += Timer->Elapsed();
			hitSparkTime += Timer->Elapsed();
			if (hittedTime >= 3.0f)
			{
				isHitted = false;
				object_desc.b_bound_coll = true; // 이제 충돌 가능한 상태이다.
			}
		}

		if (Key->Down('X'))
		{
			switch (bullet_type)
			{
			case PlAYER_BULLET_TYPE::peashot:
				bulletPool->waitTime = bulletPool->createTime[0];
				break;
			case PlAYER_BULLET_TYPE::homingshot:
				bulletPool->waitTime = bulletPool->createTime[1];
				break;
			case PlAYER_BULLET_TYPE::spreadshot:
				bulletPool->waitTime = bulletPool->createTime[2];
				break;
			default:
				break;
			}
		}

		if (Key->Press('X'))
		{
			D3DXMATRIX & world = state->animation->GetSprite()->World();
			D3DXVECTOR2 xDir = D3DXVECTOR2(world._11, world._12) * 0.5;
			D3DXVECTOR2 yDir = D3DXVECTOR2(world._21, world._22) * 0.5;
			D3DXVECTOR2 bulletPos = position + yDir;
			
			D3DXVECTOR3 degree = RotationDegree();

			shootDir = { 0,0 };

			if (Key->Press(VK_UP))
			{
				shootDir.y = 1;
				bulletPos += yDir;
				degree.z = 90;
			}
			else if (Key->Press(VK_DOWN) && Key->Press('C'))
			{
				shootDir.y = -1;
				bulletPos -= yDir;
				degree.z = 270;

			}
			if (shootDir.y == 0)
			{
				if (degree.y == 180)
				{
					shootDir.x = -1;
					bulletPos -= xDir;
				}
				else
				{
					shootDir.x = 1;
					bulletPos += xDir;
				}
			}
			else
			{
				if (Key->Press(VK_LEFT))
				{
					shootDir.x = -1;
					bulletPos -= xDir;
					degree.z = 45;
				}
				else if(Key->Press(VK_RIGHT))
				{
					shootDir.x = 1;
					bulletPos += xDir;
					degree.z = 45;
				}
			}

			bulletPool->rotation = Math::ToRadian(degree);
			bulletPool->direction = shootDir;
			bulletPool->position = bulletPos;
		}
		

		bulletPool->Update(V, P);

}

void Player::Render()
{
	/*ImGui::SliderFloat("Move Speed", &moveSpeed, 50, 400);
	ImGui::SliderFloat("degree", &zDegree, 0 ,360);
	ImGui::SliderFloat("ydegree", &yDegree, 0, 180);
	ImGui::LabelText("Position :", "%f %f", animation->Position().x, animation->Position().y);
	ImGui::LabelText("velocity :", "%f", velocity);*/
	bulletPool->Render();

	if (isHitted == true)
	{
		if (hitSparkTime >= 0.1f)
		{
			hitSparkTime = 0;
			isSpark = !isSpark;
		}

		if(isSpark)
		{
			state->Render();
		}
	}
	else
	{
		state->Render();
	}
}

D3DXVECTOR2 Player::Position()
{
	return position;
}

void Player::Position(D3DXVECTOR2 pos)
{
	position = pos;
	state->animation->Position(position);
}

D3DXVECTOR2 Player::Scale()
{
	return scale;
}

void Player::Scale(D3DXVECTOR2 scale_)
{
	scale = scale_;
	state->animation->Scale(scale_);
}

D3DXVECTOR3 Player::Rotation()
{
	return rotation;
}

void Player::Rotation(D3DXVECTOR3 rotation_)
{
	rotation = rotation_;
	state->animation->Rotation(rotation_);
}

RECT Player::GetHitBox()
{
	return state->animation->GetSprite()->BoundBox();
}



void Player::Focus(D3DXVECTOR2 * position, D3DXVECTOR2 * size)
{
	*position = animation->Position() - focusOffset;

	//D3DXVECTOR2 textureSize = animation->TextureSize();
	//D3DXVECTOR2 scale = animation->Scale();
	//
	//(*size).x = textureSize.x * scale.x;
	//(*size).y = textureSize.y * scale.y;

	(*size) = D3DXVECTOR2(1, 1);
}

Sprite * Player::GetSprite()
{
	return animation->GetSprite();
}

void Player::SetGraphics(Graphics graphics)
{
}

void Player::BoundCollision(Object_Desc & desc)
{
	isHitted = true;
	isDamaged = true;
	hittedTime = 0;
}

void Player::LineCollision(D3DXVECTOR2 & p1, D3DXVECTOR2 & p2)
{
	D3DXVECTOR2 line = p2 - p1;
	D3DXVECTOR2 normalVec; // 직선의 수직벡터
	D3DXVECTOR2 axis = { 0,1 };
	D3DXVec2Normalize(&normalVec, &line);

	moveDir = normalVec; // 이동 방향 갱신;
	normalVec = { -normalVec.y, normalVec.x }; 
	
	float zDegree = Math::VectorDegree(normalVec, axis);

	D3DXMATRIX & world = state->animation->GetSprite()->World();

	D3DXVECTOR2 xDir = D3DXVECTOR2(world._11, world._12) * 0.5;
	D3DXVECTOR2 yDir = D3DXVECTOR2(world._21, world._22) * 0.5;

	float & x = position.x;
	float y;

	RenderType obj_renderType = state->animation->GetSprite()->GetRenderType();
	D3DXVECTOR2 newPos;
	switch (obj_renderType)
	{
	case RenderType::center:
		y = (p2.y - p1.y) / (p2.x - p1.x) * ((position - yDir).x - p1.x) + p1.y;
		newPos = D3DXVECTOR2(x, y);
		newPos.y += yDir.y;
		break;
	case RenderType::left_bottom:
		break;
	case RenderType::center_bottom:
		y = (p2.y - p1.y) / (p2.x - p1.x) * (position.x - p1.x) + p1.y;
		newPos = D3DXVECTOR2(x, y);
		break;
	default:
		break;
	}
	
	Position(newPos);
	bOnGround = true;

	isCanDash = true;
	isCanParry = true;
}

void Player::StartJump()
{
	velocity = startJumpVelocity;
	bOnGround = false;
}

void Player::EndJump()
{
	if (velocity > endJumpVelocity)
	{
		velocity = endJumpVelocity;
	}
}
