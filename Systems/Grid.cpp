#include "stdafx.h"
#include "Grid.h"
#include "Objects/Marker.h"

void Grid::Add(Marker * marker)
{
	int cellX = (int)((marker->position.x + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);
	int cellY = (int)((marker->position.y + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);

	marker->prev = nullptr;
	marker->next = cells[cellX][cellY];

	if (marker->next != nullptr)
	{
		marker->next->prev = marker;
	}

	if (marker->next == nullptr)
	{
		cells[cellX][cellY] = marker;
	}


}

void Grid::Move(Marker * marker, D3DXVECTOR2 position)
{
	int oldCellX = (int)(int)((marker->position.x + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);
	int oldCellY = (int)(int)((marker->position.y + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);

	int cellX = (int)(int)((position.x + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);
	int cellY = (int)(int)((position.y + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);

	marker->position = position;

	if (oldCellX == cellX && oldCellY == cellY)
	{
		return;
	}
	
	// unlinking prev node
	if (marker->prev != nullptr)
	{
		marker->prev->next = marker->next;
	}
	// unlinking next node
	if (marker->next != nullptr)
	{
		marker->next->prev = marker->prev;
	}
	// update cell head
	if (cells[oldCellX][oldCellY] == marker)
	{
		cells[oldCellX][oldCellY] = marker->next;
	}

	Add(marker);
}

Marker * Grid::Pop(D3DXVECTOR2 & clickPosition)
{

	int cellX = (int)((clickPosition.x + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);
	int cellY = (int)((clickPosition.y + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);

	Marker* cmpMarker = cells[cellX][cellY];

	Marker* clickedMarker = NULL;

	bool out = false;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			Marker* cmpMarker = cells[cellX+i][cellY+j];
			if (cmpMarker == nullptr) continue;
			float left = cmpMarker->position.x - 20;
			float right = cmpMarker->position.x + 20;
			float bottom = cmpMarker->position.y - 20;
			float top = cmpMarker->position.y + 20;

			if (
				left <= clickPosition.x
				&& right >= clickPosition.x
				&& bottom <= clickPosition.y
				&& top >= clickPosition.y)
			{
				clickedMarker = cmpMarker;

				// unlinking prev node
				if (cmpMarker->prev != nullptr)
				{
					cmpMarker->prev->next = cmpMarker->next;
				}
				// unlinking next node
				if (cmpMarker->next != nullptr)
				{
					cmpMarker->next->prev = cmpMarker->prev;
				}
				// update cell head
				if (cells[cellX][cellY] == cmpMarker)
				{
					cells[cellX][cellY] = cmpMarker->next;
				}
				out = true;
				break;
			}
			if (out)break;
		}
		if (out)break;
	}

	return clickedMarker;
}
