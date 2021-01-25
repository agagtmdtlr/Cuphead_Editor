#pragma once
#include "Objects/Object.h"

class Grid
{
public:

	static const int NUM_CELLS = 200; // 공간의 개수
	static const int CELL_SIZE = 100; // 공간의 크기

	Grid();

	void Reset();
	
	void Add(class Object* marker);
	void Move(Object* marker, D3DXVECTOR2 position);

	Object* Pop(D3DXVECTOR2 & clickPosition);



private:
	Object* cells[NUM_CELLS][NUM_CELLS];
};