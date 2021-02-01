#include "stdafx.h"
#include "Grid.h"

Grid::Grid()
{
	Reset();
}

void Grid::Reset()
{
	// ���ڸ� �� �����.
	for (int x = 0; x < NUM_CELLS; x++)
	{
		for (int y = 0; y < NUM_CELLS; y++)
		{
			cells[x][y] = nullptr;		
		}
	}

}

void Grid::Add(Object * object)
{
	int cellX = (int)((object->position.x + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);
	int cellY = (int)((object->position.y + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);

	object->prev = nullptr;
	object->next = this->cells[cellX][cellY];

	if (object->next != nullptr)
	{
		object->next->prev = object;
	}

	if (object->next == nullptr)
	{
		cells[cellX][cellY] = object;
	}

	cells[cellX][cellY] = object;
}

void Grid::Move(Object * object, D3DXVECTOR2 position)
{
	int oldCellX = (int)(int)((object->position.x + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);
	int oldCellY = (int)(int)((object->position.y + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);

	int cellX = (int)(int)((position.x + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);
	int cellY = (int)(int)((position.y + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);

	object->position = position;

	if (oldCellX == cellX && oldCellY == cellY)
	{
		return;
	}
	
	// unlinking prev node
	if (object->prev != nullptr)
	{
		object->prev->next = object->next;
	}
	// unlinking next node
	if (object->next != nullptr)
	{
		object->next->prev = object->prev;
	}
	// update cell head
	if (cells[oldCellX][oldCellY] == object)
	{
		cells[oldCellX][oldCellY] = object->next;
	}

	Add(object);
}

Object * Grid::Pop(D3DXVECTOR2 & clickPosition)
{

	int cellX = (int)((clickPosition.x + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);
	int cellY = (int)((clickPosition.y + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);

	Object* cmpobject = cells[cellX][cellY];

	Object* clickedobject = NULL;

	bool out = false;
	// ���� ������
	// ������ ������ ������� �ش� ���콺 ��ǥ�� �����ϴ� �̹�����
	// ������� ������ �����ִ� ��� ���� ������ �ʴ� �̹����� �巡���ϰ� �ȴ�
	// ������Ʈ�� z���� �ο��Ͽ� �������� �ؾ� �ڴ�.
	// ���࿡ ���콺 �����ȿ� �����ϰ� ���� �̹������� z���� ũ�ٸ� �����´�.
	int pop_x, pop_y;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			Object* cmpobject = cells[cellX+i][cellY+j];

			while (cmpobject != nullptr)
			{
				RECT box = cmpobject->GetHitBox();
				float left = (float)box.left;
				float right = (float)box.right;
				float bottom = (float)box.bottom;
				float top = (float)box.top;

				if (
					left <= clickPosition.x
					&& right >= clickPosition.x
					&& bottom <= clickPosition.y
					&& top >= clickPosition.y)
				{
					if (clickedobject == nullptr)
					{
						clickedobject = cmpobject;
						pop_x = cellX + i;
						pop_y = cellY + j;
					}
					else if (clickedobject != nullptr &&
						clickedobject->object_desc.depth < cmpobject->object_desc.depth)
					{
						clickedobject = cmpobject;
						pop_x = cellX + i;
						pop_y = cellY + j;
					}
				}
				cmpobject = cmpobject->next;
			}			
		}
	}

	if (clickedobject != nullptr)
	{
		// update cell head
		if (cells[pop_x][pop_y] == clickedobject)
		{
			cells[pop_x][pop_y] = clickedobject->next;
		}
		// unlinking prev node
		else if (clickedobject->prev != nullptr)
		{
			clickedobject->prev->next = clickedobject->next;
		}
		// unlinking next node
		if (clickedobject->next != nullptr)
		{
			clickedobject->next->prev = clickedobject->prev;
		}
	}

	return clickedobject;
}
