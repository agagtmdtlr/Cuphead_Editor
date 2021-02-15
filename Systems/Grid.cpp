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

	object->prev = 0;
	object->next = 0;

	if (object == cells[cellX][cellY])
	{
		return;
		assert(false);
	}

	if (cells[cellX][cellY] != 0)
	{
		object->next = cells[cellX][cellY];
	}

	if (object->next != nullptr)
	{
		object->next->prev = object;
	}
	cells[cellX][cellY] = object;


	if (object == object->next)
	{
		assert(false);
	}

}

void Grid::Move(Object * object, D3DXVECTOR2 position)
{
	int oldCellX = (int)((object->position.x + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);
	int oldCellY = (int)((object->position.y + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);

	int cellX = (int)((position.x + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);
	int cellY = (int)((position.y + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);

	object->position = position;
	object->gridX = cellX;
	object->gridY = cellY;

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
		if (clickedobject->prev != nullptr)
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

bool Grid::Remove(Object * object)
{
	D3DXVECTOR2 position = object->Position();
	int cellX = (int)((position.x + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);
	int cellY = (int)((position.y + (Grid::NUM_CELLS * Grid::CELL_SIZE * 0.5)) / Grid::CELL_SIZE);


	
	

	// update cell head
	if (cells[cellX][cellY] == object)
	{
		cells[cellX][cellY] = object->next;
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

	if (object == object->prev)
	{
		assert(false);
	}
	if (object == object->next)
	{
		assert(false);
	}
	if (object->prev == object->next && object->prev != 0 && object->next != 0)
	{
		assert(false);
	}
	return true;

}

void Grid::HandleMelee(vector<Liner*> * lines)
{
	for (int x = 0; x < NUM_CELLS; x++)
	{
		for (int y = 0; y < NUM_CELLS; y++)
		{
			HandleCell(x,y,lines);
		}
	}
}

void Grid::HandleCell(int x, int y, vector<Liner*> * lines)
{
	Object * unit = cells[x][y];

	while (unit != nullptr)
	{
		HandleUnit(unit, unit->next);
		if (x > 0) HandleUnit(unit, cells[x - 1][y]); // ��ĭ
		if (y > 0) HandleUnit(unit, cells[x][y - 1]); //  ��ı
		if (x > 0 && y > 0) HandleUnit(unit, cells[x - 1][y - 1]); // ���� �缱
		if (x > 0 && y > NUM_CELLS - 1) HandleUnit(unit, cells[x - 1][y + 1]); // ������ �缱		

		HandleLine(unit, lines);

		unit = unit->next;
	}
}

void Grid::HandleUnit(Object * unit, Object * other)
{
	while (other != nullptr)
	{
		if (distance(unit, other))
		{
			HandleAttack(unit, other);
		}
		other = other->next;
	}
}

void Grid::HandleLine(Object * unit, vector<Liner*>* lines)
{
	if (unit->object_desc.b_line_coll == false)
		return;

	for (Liner* line : *lines)
	{
		if (CheckLineColl(unit, line)) // ���浹�� �Ͼ ���
		{
			D3DXVECTOR2 & p1 = line->firstMarker->position;
			D3DXVECTOR2 & p2 = line->secondMarker->position;

			unit->LineCollision(p1, p2);
		}
	}
}

bool Grid::distance(Object * unit, Object * other)
{
	bool result = false;

	/*if (unit->object_desc.label == OBJECT_LABEL::duck &&
		other->object_desc.label == OBJECT_LABEL::player_bullet)
		assert(false);
	if (other->object_desc.label == OBJECT_LABEL::duck &&
		unit->object_desc.label == OBJECT_LABEL::player_bullet)
		assert(false);*/

	if (unit->object_desc.group == other->object_desc.group)
		return false;
	if (unit->object_desc.label == other->object_desc.label) // ���� ���������� �浹 ó���� ���� �ʴ´�.
		return false;
	if (unit->object_desc.b_bound_coll && other->object_desc.b_bound_coll)
	{
		RECT boxA = unit->GetHitBox();
		RECT boxB = other->GetHitBox();

		if (boxA.left <= boxB.right &&
			boxA.right >= boxB.left &&
			boxA.bottom <= boxB.top &&
			boxA.top >= boxB.bottom)
		{
			result = true;
		}
	}

	return result;
}

bool Grid::HandleAttack(Object * unit, Object * other)
{
	unit->BoundCollision(unit->object_desc);
	other->BoundCollision(unit->object_desc);
	return false;
}

bool Grid::CheckLineColl(Object * object, Liner * liner)
{
	bool result = false;
	Sprite* sprite = object->GetSprite();
	D3DXMATRIX world = sprite->World();

	D3DXVECTOR2 playerPosition = D3DXVECTOR2(world._41, world._42);

	D3DXVECTOR2 xDir = D3DXVECTOR2(world._11, world._12) * 0.5;
	D3DXVECTOR2 yDir = D3DXVECTOR2(world._21, world._22) * 0.5;

	// ���������� �����ϱ� ����
	D3DXVECTOR2 yDirNormal;
	D3DXVec2Normalize(&yDirNormal, &yDir);

	// length is 7 object line;
	// object �ٴ��� �������� ���� 5 , �Ʒ��� 2(�������� ������ ����)
	D3DXVECTOR2 line1_p1 = playerPosition - yDir - yDirNormal * 2;
	D3DXVECTOR2 line1_p2 = playerPosition;

	D3DXVECTOR2 & line2_p1 = liner->firstMarker->position;
	D3DXVECTOR2 & line2_p2 = liner->secondMarker->position;

	result = Line::LineSegmentIntersection(line1_p1, line1_p2, line2_p1, line2_p2);


	return result;
}
