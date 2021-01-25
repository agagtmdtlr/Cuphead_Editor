#include "stdafx.h"
#include "Grid.h"

Grid::Grid()
{
	Reset();
}

void Grid::Reset()
{
	// 격자를 싹 지운다.
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

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			Object* cmpobject = cells[cellX+i][cellY+j];
			if (cmpobject == nullptr) continue;
			RECT box = cmpobject->GetHitBox();
			float left = box.left;
			float right = box.right;
			float bottom = box.bottom;
			float top = box.top;

			if (
				left <= clickPosition.x
				&& right >= clickPosition.x
				&& bottom <= clickPosition.y
				&& top >= clickPosition.y)
			{
				clickedobject = cmpobject;

				// unlinking prev node
				if (cmpobject->prev != nullptr)
				{
					cmpobject->prev->next = cmpobject->next;
				}
				// unlinking next node
				if (cmpobject->next != nullptr)
				{
					cmpobject->next->prev = cmpobject->prev;
				}
				// update cell head
				if (cells[cellX][cellY] == cmpobject)
				{
					cells[cellX][cellY] = cmpobject->next;
				}
				out = true;
				break;
			}
			if (out)break;
		}
		if (out)break;
	}

	return clickedobject;
}
