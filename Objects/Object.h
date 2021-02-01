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

	bool b_render; // ����������
	bool b_line_coll; // ���浹�� ��������
	bool b_bound_coll; // �ڽ��浹�� ��������

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
	// ���浹�� ��ȿ���� �˻��Ѵ�( �ٴڿ� �����, �߷��� �����ϰ� �ִ��� )
	// � ������ �߷°� ������� ��������ġ���� �۵�������, �Ѿ�, Ư�� ������
	// �浹�� �߷¿� ������ �ޱ� bCollideWithLine������ ������ ���ؼ� �浹�� �˻��Ѵ�.
	bool bCollideWithLine = false;
	
	/* ������ save/load�Ҷ� � ��ü�� ���������� ���� read data ����� �޶�����.
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