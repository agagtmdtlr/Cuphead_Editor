#pragma once
class Line
{
public:
	static const float PI;

	static bool LineSegmentIntersection(D3DXVECTOR2 p1, D3DXVECTOR2 p2, D3DXVECTOR2 p3, D3DXVECTOR2 p4);
	static int CCW(D3DXVECTOR2 p1,D3DXVECTOR2 p2 , D3DXVECTOR2 p3);


};