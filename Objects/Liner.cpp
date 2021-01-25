#include "stdafx.h"
#include "Liner.h"
#include "Marker.h"

Liner::Liner()
{
	Initialize();
}

Liner::Liner(Marker * firstPoint, Marker * secondPoint)
{
	Initialize(); // 라인 쉐이더 초기화

	firstMarker = firstPoint;
	secondMarker = secondPoint;

	Draw(firstPoint, secondPoint);

	
}


Liner::~Liner()
{
	SAFE_DELETE(shader);
	SAFE_RELEASE(vertexBuffer);
}

void Liner::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (prevFirstPos != firstMarker->position || prevSecondPos != secondMarker->position)
	{
		Draw(firstMarker, secondMarker);
	}

	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);

	D3DXMATRIX S, R, T; // scail, rotation , transform

	D3DXMatrixScaling(&S, scale.x, scale.y, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, 0.0f);

	world = S * R * T;

	shader->AsMatrix("World")->SetMatrix(world);	
}

void Liner::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	shader->Draw(0, bDrawCollision ? 1 : 0, 2);
}

void Liner::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Liner::Position(D3DXVECTOR2 & vec)
{
	position = vec;
}

void Liner::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void Liner::Scale(D3DXVECTOR2 & vec)
{
	scale = vec;
}

void Liner::Rotation(float x, float y, float z)
{
	Rotation(D3DXVECTOR3(x, y, z));
}

void Liner::Rotation(D3DXVECTOR3 & vec)
{
	rotation = vec;
}

void Liner::RotationDegree(float x, float y, float z)
{
	RotationDegree(D3DXVECTOR3(x, y, z));
}

void Liner::RotationDegree(D3DXVECTOR3 & vec)
{
	vec.x = Math::ToRadian(vec.x);
	vec.y = Math::ToRadian(vec.y);
	vec.z = Math::ToRadian(vec.z);

	Rotation(vec);
}

D3DXVECTOR3 Liner::RotationDegree()
{
	D3DXVECTOR3 vec;
	vec.x = Math::ToDegree(rotation.x);
	vec.y = Math::ToDegree(rotation.y);
	vec.z = Math::ToDegree(rotation.z);

	return vec;
}

void Liner::Draw(Marker * firstPoint, Marker * secondPoint)
{
	D3DXVECTOR2 firstPosition = firstPoint->Position();
	D3DXVECTOR2 secondPosition = secondPoint->Position();

	// 모든 선의 출발점은 왼쪽 점을 기준이 되도록 만든다.
	if (firstPosition.x > secondPosition.x)
	{
		swap(firstMarker, secondMarker);
		swap(firstPosition, secondPosition);
	}

	firstPosition = firstMarker->Position();
	secondPosition = secondMarker->Position();

	D3DXVECTOR2 lineVector = secondPosition - firstPosition;



	D3DXVECTOR2 normalVector;
	D3DXVec2Normalize(&normalVector, &lineVector);
	D3DXVECTOR2 xAxis = { 1, 0 };
	// 현재 필요한 각 구하기
	float zRadian = acosf(D3DXVec2Dot(&normalVector, &xAxis));
	zRadian = acosf(normalVector.x * xAxis.x + normalVector.y * xAxis.y);

	float zDegree = Math::ToDegree(zRadian);
	// 외적으로 결정
	float zVal = normalVector.x * xAxis.y - normalVector.y * xAxis.x;
	if (zVal > 0.0f)
	{
		zDegree = 360 - zDegree;
	}

	float d = Math::ToDegree(0.30597901);
	// 선의 길이 구하기
	float xScale = D3DXVec2Length(&lineVector);

	// 선의 기준은 첫번째 점
	Position(firstPosition);
	// z축을 회전시킨다.
	RotationDegree(0, 0, zDegree);
	// 선의 길이
	Scale(xScale, 1);

	prevFirstPos = firstPosition;
	prevSecondPos = secondPosition;
}

void Liner::Initialize()
{
	bDrawCollision = false;
	
	shader = new Shader(Shaders + L"15_Line.fx");

	Position(0, 0);
	Scale(1, 1);
	Rotation(0, 0, 0);
	D3DXMatrixIdentity(&world);

	HRESULT hr;

	Vertex vertices[2];
	vertices[0].Position = D3DXVECTOR3(+0.0f, 0.0f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(+1.0f, 0.0f, 0.0f);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 2;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
}
