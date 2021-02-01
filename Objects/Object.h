#pragma once
#include "Systems/Grid.h"

enum class Object_Mode
{
	Editor,
	Play
};

enum class OBJECT_LABEL
{
	static_object,
	player,
	pipe_phase1,
	pipe_phase2,
	pipe_phase3,
	pipe_phase4,
	marker
};


struct Object_Desc
{
	OBJECT_LABEL label; //reference call class constructor 
	Object_Mode obj_mode = Object_Mode::Editor; //
	int layer_index; // insert layer

	bool b_render; // 렌더링할지
	bool b_line_coll; // 선충돌을 적용할지
	bool b_bound_coll; // 박스충돌을 적용할지

};


class Object
{
	friend class Grid;
	friend class Sonic;
	friend class Editor;
	friend class EditScene;
public:

	Object(Grid * grid_ , Object_Desc desc );
	virtual ~Object();

public:

	virtual D3DXVECTOR2 Position() = 0;
	virtual void Position(D3DXVECTOR2) = 0;
	virtual D3DXVECTOR2 Scale() = 0;
	virtual void Scale(D3DXVECTOR2) = 0;
	virtual D3DXVECTOR3 Rotation() = 0;
	virtual void Rotation(D3DXVECTOR3) = 0;

public:

	virtual void Update(D3DXMATRIX & V, D3DXMATRIX & P) = 0;
	virtual void Render() = 0;

	virtual RECT GetHitBox();
	virtual void SetHitBox(RECT hitbox);

	void Set_Object_Mode(Object_Mode obj_mode) { object_desc.obj_mode = obj_mode; }

protected:
	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR3 rotation;

	wstring texturePath = L""; // if label is static_object load image this path and  others label's variable is ""


protected:

	Object_Desc object_desc;


	Grid* grid;
	Object * prev;
	Object * next;

	Object_Mode objectMode = Object_Mode::Editor;
	// 선충돌이 유효한지 검사한다( 바닥에 닿는지, 중력을 보유하고 있는지 )
	// 어떤 보스는 중력과 상관없이 고정된위치에서 작동하지만, 총알, 특정 적들을
	// 충돌과 중력에 영향을 받기 bCollideWithLine때문에 판정을 통해서 충돌을 검사한다.
	bool bCollideWithLine = false;
	
	/* 객제를 save/load할때 어떤 객체로 생성할지에 따른 read data 방식이 달라진다.
	 ex) if(label == player )
	 {
		position = readVector2();
		scale = readVector2();
		rotation = readVector3();
		Object * object = new Player(position,scale,rotation);
	 }
	 */
	OBJECT_LABEL label; 

	int layer_index;
};