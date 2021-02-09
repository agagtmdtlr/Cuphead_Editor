#pragma once
#include "Objects/Object.h"
#include "Objects/Liner.h"

class Grid
{
public:

	static const int NUM_CELLS = 10; // ������ ����
	static const int CELL_SIZE = 400; // ������ ũ��

	Grid();

	void Reset();
	
	void Add(class Object* marker);
	void Move(Object* marker, D3DXVECTOR2 position);

	Object* Pop(D3DXVECTOR2 & clickPosition);

	bool Remove(Object* object);

	virtual void HandleMelee(vector<Liner*> *lines);
	virtual void HandleCell(int x, int y, vector<Liner*> *lines);
	virtual void HandleUnit(Object * unit, Object * other);
	virtual void HandleLine(Object * unit, vector<Liner*> *lines);

	bool distance(Object * unit, Object * other);
	bool HandleAttack(Object * unit, Object * other);

	
	bool CheckLineColl(class Object* object, class Liner* liner);

private:
	Object* cells[NUM_CELLS][NUM_CELLS];
};