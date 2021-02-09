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
	object_desc.b_bound_coll = true;
	object_desc.b_line_coll = true;
	object_desc.b_render = true;


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
			position = state->animation->Position();
			rotation = state->animation->Rotation();
			grid->Move(this, position);
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
}

void Player::Render()
{
	/*ImGui::SliderFloat("Move Speed", &moveSpeed, 50, 400);
	ImGui::SliderFloat("degree", &zDegree, 0 ,360);
	ImGui::SliderFloat("ydegree", &yDegree, 0, 180);
	ImGui::LabelText("Position :", "%f %f", animation->Position().x, animation->Position().y);
	ImGui::LabelText("velocity :", "%f", velocity);*/
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
	if (desc.label == OBJECT_LABEL::homingshot ||
		desc.label == OBJECT_LABEL::peashot ||
		desc.label == OBJECT_LABEL::spreadshot)
	{

	}
	else
	{
		isHitted = true;
		hittedTime = 0;
	}
}

void Player::LineCollision(D3DXVECTOR2 & vec)
{
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
