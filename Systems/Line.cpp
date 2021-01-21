#include "stdafx.h"
#include "Line.h"

bool Line::LineSegmentIntersection(D3DXVECTOR2 p1, D3DXVECTOR2 p2, D3DXVECTOR2 p3, D3DXVECTOR2 p4)
{
	bool result = false;
	// Line1 : p1, p2
	// Line2 : p3, p4
	//line1ToSegmentLine2
	int l1_l2 = CCW(p1, p2, p3) * CCW(p1, p2, p4);
	//line2ToSegmentLine1
	int l2_l1 = CCW(p3, p4, p1) * CCW(p3, p4, p2);

	// same axis
	if (l1_l2 == 0 && l2_l1 == 0)
	{
		// sort bigger right
		if (p1 > p2)
			swap(p1, p2);
		if (p3 > p4)
			swap(p3, p4);

		if (p2 > p3 && p1 < p4)
			result = true;
	}
	// others
	else
	{
		if (l1_l2 <= 0 && l2_l1 <= 0)
		{
			result = true;
		}
	}
	return result;
}

int Line::CCW(D3DXVECTOR2 p1, D3DXVECTOR2 p2, D3DXVECTOR2 p3)
{
	int cross_product = (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y);

	if (cross_product > 0) {
		return 1;
	}
	else if (cross_product < 0) {
		return -1;
	}
	else {
		return 0;
	}
}
