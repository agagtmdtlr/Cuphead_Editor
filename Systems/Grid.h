#pragma once
#include "Objects/Object.h"

class Grid
{
public:

	static const int NUM_CELLS = 200; // ������ ����
	static const int CELL_SIZE = 100; // ������ ũ��

	Grid();

	void Reset();
	
	void Add(class Object* marker);
	void Move(Object* marker, D3DXVECTOR2 position);

	Object* Pop(D3DXVECTOR2 & clickPosition);



private:
	Object* cells[NUM_CELLS][NUM_CELLS];
};