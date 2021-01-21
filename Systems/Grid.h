#pragma once


class Grid
{
public:

	static const int NUM_CELLS = 200; // ������ ����
	static const int CELL_SIZE = 100; // ������ ũ��

	Grid()
	{
		// ���ڸ� �� �����.
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