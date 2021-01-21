#pragma once


class Grid
{
public:

	static const int NUM_CELLS = 200; // 공간의 개수
	static const int CELL_SIZE = 100; // 공간의 크기

	Grid()
	{
		// 격자를 싹 지운다.
		for (int x = 0; x < NUM_CELLS; x++)
		{
			for (int y = 0; y < NUM_CELLS; y++)
			{
				cells[x][y] = nullptr;
			}
		}
	};

	void Add(class Marker* marker);
	void Move(Marker* marker, D3DXVECTOR2 position);

	Marker* Pop(D3DXVECTOR2 & clickPosition);



private:
	class Marker* cells[NUM_CELLS][NUM_CELLS];
};