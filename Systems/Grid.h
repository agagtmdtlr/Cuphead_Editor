#pragma once
#include "Objects/Object.h"

class Grid
{
public:

	static const int NUM_CELLS = 10; // 공간의 개수
	static const int CELL_SIZE = 400; // 공간의 크기

	Grid();

	void Reset();
	
	void Add(class Object* marker);
	void Move(Object* marker, D3DXVECTOR2 position);

	Object* Pop(D3DXVECTOR2 & clickPosition);

	bool Remove(Object* object);

	virtual void HandleMelee();
	virtual void HandleCell(int x, int y);
	virtual void HandleUnit(Object * unit, Object * other);

	bool distance(Object * unit, Object * other);
	bool HandleAttack(Object * unit, Object * other);

private:
	Object* cells[NUM_CELLS][NUM_CELLS];
};