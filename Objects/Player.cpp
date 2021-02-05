#include "stdafx.h"
#include "Player.h"

Player::Player(Grid* grid_, D3DXVECTOR2 position_, D3DXVECTOR2 scale_, Object_Desc desc, SceneValues * values, RenderType type)
	:Object(grid_,desc, values)
	,moveSpeed(400.0f), focusOffset(-180, -120), bOnGround(false), cosVel(1),
	zDegree(0), yDegree(0), moveDir(1, 0),
	velocity(0), gravity(-1.95f), type(type)
	, startJumpVelocity(0.75)
	, endJumpVelocity(0.5)
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

	animation = new Animation();

	wstring spriteFile = Textures + L"Metalslug.png";
	wstring shaderFile = Shaders + L"008_Sprite.fx";

	//rendertype
	this->type = RenderType::center;

	animation = idleState->animation;

	animation->Rotation(0, 0, 0);
	animation->Position(position_);
	animation->Scale(scale_);
	animation->Play(0);

	animation->DrawBound(true);

	position = position_;
	rotation = { 0,0,0 };
	scale = scale_;
	state = idleState;
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
		}
		else if (object_desc.obj_mode == Object_Mode::Editor)
		{
			state->animation->Update(V, P);
		}		
}

void Player::Render()
{
	/*ImGui::SliderFloat("Move Speed", &moveSpeed, 50, 400);
	ImGui::SliderFloat("degree", &zDegree, 0 ,360);
	ImGui::SliderFloat("ydegree", &yDegree, 0, 180);
	ImGui::LabelText("Position :", "%f %f", animation->Position().x, animation->Position().y);
	ImGui::LabelText("velocity :", "%f", velocity);*/
	
	state->Render();
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
