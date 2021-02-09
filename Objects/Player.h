#pragma once
#include "../Viewer/IFollowing.h"
#include "Object.h"

enum class Graphics
{
	Aim_Diagonal_Down,
	Aim_Diagonal_Up,
	Aim_Down,
	Aim_Straight,
	Aim_Up,
	
	Dash_Air,
	Dash_Ground,
	
	Death,
	
	Duck_Idle,
	Duck_Shoot,
	
	Ghost,
	
	Hit_Air,
	Hit_Ground,
	
	Idle,
	Intro,
	Jump,
	Parry,
	
	Run_Normal,
	Run_Shoot_Diagonal_Up,
	Run_Shoot_Straight,
	
	Shoot_Diagonal_Down,
	Shoot_Diagonal_Up,
	Shoot_Down,
	Shoot_Straight,
	Shoot_Up,

	Air_SpecialAttack_Diagonal_Down,
	Air_SpecialAttack_Diagonal_Up,
	Air_SpecialAttack_Down,
	Air_SpecialAttack_Straight,
	Air_SpecialAttack_Up,

	Super_Beam
};

class Player : public IFollowing , public Object
{
	friend class PlayerState;
	friend class AimState;
	friend class DashState;
	friend class DuckState;
	friend class IdleState;
	friend class JumpState;
	friend class OnAirState;
	friend class OnGroundState;
	friend class ParryState;
	friend class RunState;
	friend class DamageState;

	friend class Sonic;
public:
	Player(class Grid* grid,D3DXVECTOR2 position_, D3DXVECTOR2 scale_ , Object_Desc desc, SceneValues * values,RenderType type = RenderType::center_bottom);
	~Player();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;
	virtual void Render() override;

public:
	virtual D3DXVECTOR2 Position() override;
	virtual void Position(D3DXVECTOR2) override;
	virtual D3DXVECTOR2 Scale() override;
	virtual void Scale(D3DXVECTOR2)override;
	virtual D3DXVECTOR3 Rotation() override;
	virtual void Rotation(D3DXVECTOR3)override;

	virtual RECT GetHitBox() override;

public:
	void Focus(D3DXVECTOR2* position, D3DXVECTOR2* size);
	Sprite* GetSprite();

	void SetGraphics(Graphics graphics);

	virtual void BoundCollision(Object_Desc & desc) override;
	virtual void LineCollision(D3DXVECTOR2 & vec) override;

private:
	void StartJump();
	void EndJump();


private:
	RenderType type;
	
	class State* state;

	float moveSpeed;
	Animation* animation;

	bool bOnGround;

	bool isDamaged = false; // 데미지를 입었다.
	bool isHitted = true; // 데미지를 입은 상태 ( 잠시 무적시간이다)
	bool isCanDash = true;
	bool isCanParry = true;
	// use hit render
	float hittedTime;
	float hitDelay;
	float hitSparkTime;

	bool isSpark = true;


	float zDegree;
	float yDegree;

	float velocity;
	float cosVel;

	float gravity;

	float startJumpVelocity;
	float endJumpVelocity;

	

	D3DXVECTOR2 moveDir;
	D3DXVECTOR2 shootDir;
	D3DXVECTOR2 focusOffset;

private:
	AimState* aimState;
	DashState* dashState;
	DuckState* duckState;
	IdleState* idleState;
	JumpState* jumpState;
	OnAirState* onAirState;
	OnGroundState* onGroundState;
	ParryState* parryState;
	RunState* runState;
	DamageState* damageState;
};