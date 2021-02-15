#include "stdafx.h"
#include "Sprite.h"

Sprite::Sprite(wstring textureFile, wstring shaderFile, RenderType renderType)
	: renderType(renderType)
{
	Initialize(textureFile, shaderFile, 0, 0, 0, 0);
}

Sprite::Sprite(wstring textureFile, wstring shaderFile, float endX, float endY, RenderType renderType)
	: renderType(renderType)
{
	Initialize(textureFile, shaderFile, 0, 0, endX, endY);
}

Sprite::Sprite(wstring textureFile, wstring shaderFile, float startX, float startY, float endX, float endY, RenderType renderType)
	: renderType(renderType)
{
	Initialize(textureFile, shaderFile, startX, startY, endX, endY);
}

void Sprite::Initialize(wstring spriteFile, wstring shaderFile, float startX, float startY, float endX, float endY)
{
	bDrawBound = false;
	bDrawCollision = false;
	textureFile = spriteFile;

	shader = new Shader(shaderFile);
	boundShader = new Shader(Shaders + L"15_Bounding.fx");


	srv = Sprites::Load(spriteFile);
	shader->AsShaderResource("Map")->SetResource(srv);

	Position(0, 0);
	Scale(1, 1);
	Rotation(0, 0, 0);	
	BoundPosition(0, 0);

	D3DXMatrixIdentity(&world);

	HRESULT hr;
	D3DX11_IMAGE_INFO info;
	hr = D3DX11GetImageInfoFromFile(spriteFile.c_str(), NULL, &info, NULL);
	assert(SUCCEEDED(hr));

	startX = (startX > 0) ? startX / (float)info.Width : 0.0f;
	startY = (startY > 0) ? startY / (float)info.Height : 0.0f;

	endX = (endX > 0) ? endX / (float)info.Width : 1.0f;
	endY = (endY > 0) ? endY / (float)info.Height : 1.0f;

	Vertex vertices[6];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);

	vertices[0].UV = D3DXVECTOR2(startX, endY);
	vertices[1].UV = D3DXVECTOR2(startX, startY);
	vertices[2].UV = D3DXVECTOR2(endX, endY);
	vertices[3].UV = D3DXVECTOR2(endX, endY);
	vertices[4].UV = D3DXVECTOR2(startX, startY);
	vertices[5].UV = D3DXVECTOR2(endX, startY);

	float sizeX = (endX > 0) ? endX * (float)info.Width : (float)info.Width;
	sizeX -= startX * (float)info.Width;


	float sizeY = (endY > 0) ? endY * (float)info.Height : (float)info.Height;
	sizeY -= startY * (float)info.Height;

	textureSize = D3DXVECTOR2(sizeX, sizeY);
	bound_textureSize = D3DXVECTOR2(sizeX, sizeY);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	CreateBound();

}

Sprite::~Sprite()
{
	SAFE_DELETE(shader);
	SAFE_RELEASE(vertexBuffer);

	Sprites::Remove(textureFile);
}


void Sprite::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);

	boundShader->AsMatrix("View")->SetMatrix(V);
	boundShader->AsMatrix("Projection")->SetMatrix(P);


	D3DXMATRIX S, R, T;
	D3DXMATRIX bound_S,bound_R, bound_T;
	// scaling and rotation for object
	D3DXMatrixScaling(&S, scale.x * textureSize.x, scale.y * textureSize.y, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);

	world = S * R;
	// scailing for bound boux
	D3DXMatrixScaling(&bound_S, scale.x * bound_textureSize.x, scale.y * bound_textureSize.y, 1.0f);
	D3DXMatrixRotationYawPitchRoll(&bound_R, 0, rotation.x, 0);

	bound_world = bound_S * bound_R;

	D3DXVECTOR2 xDir = D3DXVECTOR2(world._11, world._12) * 0.5;
	D3DXVECTOR2 yDir = D3DXVECTOR2(world._21, world._22) * 0.5;
	
	// transportation matrix each for render type
	switch (renderType)
	{
	case RenderType::center:
		D3DXMatrixTranslation(&T, position.x, position.y, 0.0f);
		D3DXMatrixTranslation(&bound_T, bound_position.x + position.x,
			bound_position.y + position.y, 0.0f);
		break;
	case RenderType::left_bottom:
		D3DXMatrixTranslation(&T, position.x + xDir.x + yDir.x, position.y + xDir.y + yDir.y, 0.0f);
		D3DXMatrixTranslation(&bound_T, bound_position.x + position.x + xDir.x + yDir.x, 
			bound_position.y + position.y + xDir.y + yDir.y, 0.0f);
		break;
	case RenderType::center_bottom:
		D3DXMatrixTranslation(&T, position.x + yDir.x, position.y + yDir.y, 0.0f);
		D3DXMatrixTranslation(&bound_T, bound_position.x + position.x + yDir.x, 
			bound_position.y + position.y + yDir.y, 0.0f);
		break;
	default:
		break;
	}
	world *= T;
	bound_world *= bound_T;

	shader->AsMatrix("World")->SetMatrix(world);
	boundShader->AsMatrix("World")->SetMatrix(bound_world);

	UpdateBoundBox();
}

void Sprite::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 6);

	if (bDrawBound == true)
	{
		stride = sizeof(BoundVertex);
		offset = 0;

		DeviceContext->IASetVertexBuffers(0, 1, &boundVertexBuffer, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		boundShader->Draw(0, bDrawCollision ? 1 : 0, 5);
	}

	//ImGui::LabelText("world", "%f , %f", world._11 + world._21 + +world._31, world._12 + world._22 + world._32);
	//ImGui::LabelText("direct1", "%f , %f", world._11, world._12);
	//ImGui::LabelText("direct2", "%f , %f", world._21, world._22);
}

RECT Sprite::BoundBox()
{	
	return boundbox;
}

bool Sprite::AABB(D3DXVECTOR2 & position)
{
	return AABB(this, position);
}

bool Sprite::AABB(Sprite * b)
{
	return AABB(this, b);
}

bool Sprite::OBB(Sprite * b)
{
	return OBB(this, b);
}

bool Sprite::AABB(Sprite * a, D3DXVECTOR2 & position)
{
	float xScale = a->scale.y * a->TextureSize().x * 0.5f;
	float yScale = a->scale.y * a->TextureSize().y * 0.5f;

	RECT box = a->BoundBox();

	/*float left = a->position.x - xScale;
	float right = a->position.x + xScale;
	float bottom = a->position.y - yScale;
	float top = a->position.y + yScale;*/
	float left = box.left;
	float right = box.right;
	float bottom = box.bottom;
	float top = box.top;

	bool b = true;
	b &= position.x > left;
	b &= position.x <= right;
	b &= position.y > bottom;
	b &= position.y <= top;

	return b;
}

bool Sprite::AABB(Sprite * a, Sprite * b)
{
	float xScale = a->scale.y * a->TextureSize().x * 0.5f;
	float yScale = a->scale.y * a->TextureSize().y * 0.5f;
	
	RECT boxA = a->BoundBox();

	/*float leftA = a->position.x - xScale;
	float rightA = a->position.x + xScale;
	float bottomA = a->position.y - yScale;
	float topA = a->position.y + yScale;*/
	float leftA = boxA.left;
	float rightA = boxA.right;
	float bottomA = boxA.bottom;
	float topA = boxA.top;


	xScale = b->scale.y * b->TextureSize().x * 0.5f;
	yScale = b->scale.y * b->TextureSize().y * 0.5f;

	RECT boxB = b->BoundBox();

	/*float leftB = b->position.x - xScale;
	float rightB = b->position.x + xScale;
	float bottomB = b->position.y - yScale;
	float topB = b->position.y + yScale;*/
	float leftB = boxB.left;
	float rightB = boxB.right;
	float bottomB = boxB.bottom;
	float topB = boxB.top;

	bool bCheck = true;
	bCheck &= leftA < rightB;
	bCheck &= topA > bottomB;
	bCheck &= rightA > leftB;
	bCheck &= bottomA < topB;

	return bCheck;
}

bool Sprite::OBB(Sprite * a, Sprite * b)
{
	OBBDesc obbA, obbB;
	float xScale, yScale; // ?? 사용을 한 적인 없다.

	D3DXVECTOR2 lengthA = D3DXVECTOR2(a->world._11, a->world._22) * 0.5f; // obb 객체의 각 변의 절반 길이를 구한다.
	CreateOBB(&obbA, a->position, a->world, lengthA);

	D3DXVECTOR2 lengthB = D3DXVECTOR2(b->world._11, b->world._22) * 0.5f;
	CreateOBB(&obbB, b->position, b->world, lengthB);

	return CheckOBB(obbA, obbB); // 각 obb의 2변이 기준 축이 되어 총 4번의 검사를 한다.
}

void Sprite::CreateOBB(OUT OBBDesc * out, D3DXVECTOR2 & position, D3DXMATRIX & world, D3DXVECTOR2 & length)
{
	out->Position = position;

	out->Length[0] = length.x; // obb객체의 x축 radius
	out->Length[1] = length.y; // obb객체의 y축 radius

	out->Direction[0] = D3DXVECTOR2(world._11, world._12); // 회전이 적용된 x축 벡터
	out->Direction[1] = D3DXVECTOR2(world._21, world._22); // 회전이 적용된 y축 벡터

	D3DXVec2Normalize(&out->Direction[0], &out->Direction[0]);
	D3DXVec2Normalize(&out->Direction[1], &out->Direction[1]);

}

float Sprite::SeperateAxis(D3DXVECTOR2 & seperate, D3DXVECTOR2 & e1, D3DXVECTOR2 & e2)
{
	// 투영 축에 투영시킨 각 obb 내적 스칼라의 합을 구한
	
	// 또 다른 obb의 객체의 투영 길이는 x변 y변의 벡터를 투영축에 내적시켜 합한것이 총 길이다
	float r1 = fabsf(D3DXVec2Dot(&seperate, &e1));
	float r2 = fabsf(D3DXVec2Dot(&seperate, &e2));


	return r1 + r2;
}

bool Sprite::CheckOBB(OBBDesc & obbA, OBBDesc & obbB)
{
	// n... : 투영 축 , e.. : 각 obb의 내적할 벡터
	D3DXVECTOR2 nea1 = obbA.Direction[0], ea1 = nea1 * obbA.Length[0];
	D3DXVECTOR2 nea2 = obbA.Direction[1], ea2 = nea2 * obbA.Length[1];
	D3DXVECTOR2 neb1 = obbB.Direction[0], eb1 = neb1 * obbB.Length[0];
	D3DXVECTOR2 neb2 = obbB.Direction[1], eb2 = neb2 * obbB.Length[1];
	D3DXVECTOR2 direction = obbA.Position - obbB.Position; // obb 중심을 연결한 벡터 ( 투영 길이와의 비교 기준 )

	float lengthA = 0.0f, lengthB = 0.0f, length = 0.0f;

	// 총 4개의 투영축에 대해서 검사한다.

	// 각 obb의 1개의 변이 투영축이므로 투영축을 사용한 obb객체는 투영에 사용한 변의 radius만 필요하다
	lengthA = D3DXVec2Length(&ea1);
	// 또다른 obb의 투영 길이
	lengthB = SeperateAxis(nea1, eb1, eb2);
	length = fabsf(D3DXVec2Dot(&direction, &nea1));
	if (length > lengthA + lengthB) // 중심 간의 거리( 기준 축에 투영 길이) 보다 분리된 축의 합의 길이( 기준 축에 투영 길이)가 크면 서로 충돌하지 않았다.
		return false;

	lengthA = D3DXVec2Length(&ea2);
	lengthB = SeperateAxis(nea2, eb1, eb2);
	length = fabsf(D3DXVec2Dot(&direction, &nea2));
	if (length > lengthA + lengthB)
		return false;

	lengthA = SeperateAxis(neb1, ea1, ea2);
	lengthB = D3DXVec2Length(&eb1);
	length = fabsf(D3DXVec2Dot(&direction, &neb1));
	if (length > lengthA + lengthB)
		return false;

	lengthA = SeperateAxis(neb2, ea1, ea2);
	lengthB = D3DXVec2Length(&eb2);
	length = fabsf(D3DXVec2Dot(&direction, &neb2));
	if (length > lengthA + lengthB)
		return false;


	return true;
}

void Sprite::CreateBound()
{
	BoundVertex vertices[6];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(BoundVertex) * 5;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &boundVertexBuffer);
		assert(SUCCEEDED(hr));
	}

	UpdateBoundBox();

	/*switch (renderType)
	{
	case RenderType::center:
		boundbox.left = bound_position.x - scale.x * textureSize.x * 0.5f;
		boundbox.right = bound_position.x + scale.x * bound_textureSize.x * 0.5f;
		boundbox.bottom = bound_position.y - scale.y * bound_textureSize.y * 0.5f;
		boundbox.top = bound_position.y + scale.y * bound_textureSize.y * 0.5f;
		break;
	case RenderType::left_bottom:
		boundbox.left = bound_position.x;
		boundbox.right = bound_position.x + scale.x * bound_textureSize.x;
		boundbox.bottom = bound_position.y;
		boundbox.top = bound_position.y + scale.y * bound_textureSize.y;
		break;
	case RenderType::center_bottom:
		boundbox.left = bound_position.x - scale.x * bound_textureSize.x * 0.5f;
		boundbox.right = bound_position.x + scale.x * bound_textureSize.x * 0.5f;
		boundbox.bottom = bound_position.y;
		boundbox.top = bound_position.y + scale.y * bound_textureSize.y;
		break;
	default:
		break;
	}*/

}

void Sprite::UpdateBoundBox()
{

	D3DXVECTOR2 xDir = D3DXVECTOR2(world._11, world._12) * 0.5;
	D3DXVECTOR2 yDir = D3DXVECTOR2(world._21, world._22) * 0.5;
	switch (renderType)
	{
	case RenderType::center:
		boundbox.left = position.x - scale.x * textureSize.x * 0.5f + bound_position.x;
		boundbox.right = position.x + scale.x * bound_textureSize.x * 0.5f + bound_position.x;
		boundbox.bottom = position.y - scale.y * bound_textureSize.y * 0.5f + bound_position.y;
		boundbox.top = position.y + scale.y * bound_textureSize.y * 0.5f + bound_position.y;
		break;
	case RenderType::left_bottom:
		boundbox.left = position.x + bound_position.x;
		boundbox.right = position.x + scale.x * bound_textureSize.x + bound_position.x;
		//
		boundbox.bottom = position.y + bound_position.y + xDir.y + yDir.y - scale.y * bound_textureSize.y * 0.5;
		boundbox.top = position.y + bound_position.y + xDir.y + yDir.y + scale.y * bound_textureSize.y * 0.5;
		break;
	case RenderType::center_bottom:
		boundbox.left = position.x - scale.x * bound_textureSize.x * 0.5f + bound_position.x;
		boundbox.right = position.x + scale.x * bound_textureSize.x * 0.5f + bound_position.x;
		boundbox.bottom = position.y + bound_position.y + xDir.y + yDir.y - scale.y * bound_textureSize.y * 0.5;
		boundbox.top = position.y + bound_position.y + xDir.y + yDir.y + scale.y * bound_textureSize.y * 0.5;
		break;
	default:
		break;
	}
}

void Sprite::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Sprite::Position(D3DXVECTOR2 & vec)
{
	/*switch (renderType)
	{
	case RenderType::center:		
		break;
	case RenderType::left_bottom:
		vec.x += scale.x * textureSize.x * 0.5f;
		vec.y += scale.y * textureSize.y * 0.5f;
		break;
	case RenderType::center_bottom:
		vec.y += scale.y * textureSize.y * 0.5f;
		break;
	default:
		break;
	}*/
	position = vec;
}

void Sprite::BoundPosition(float x, float y)
{
	BoundPosition(D3DXVECTOR2(x, y));
}

void Sprite::BoundPosition(D3DXVECTOR2 & vec)
{
	bound_position = vec;
}

void Sprite::BoundTextureSize(float x, float y)
{
	BoundTextureSize(D3DXVECTOR2(x, y));
}

void Sprite::BoundTextureSize(D3DXVECTOR2 & vec)
{
	bound_textureSize = vec;
}

void Sprite::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void Sprite::Scale(D3DXVECTOR2 & vec)
{
	scale = vec;
	bound_scale = vec;
}

void Sprite::Rotation(float x, float y, float z)
{
	Rotation(D3DXVECTOR3(x, y, z));
}

void Sprite::Rotation(D3DXVECTOR3 & vec)
{
	rotation = vec;
	bound_rotation = vec;
}

void Sprite::RotationDegree(float x, float y, float z)
{
	RotationDegree(D3DXVECTOR3(x, y, z));
}

void Sprite::RotationDegree(D3DXVECTOR3 & vec)
{
	vec.x = Math::ToRadian(vec.x);
	vec.y = Math::ToRadian(vec.y);
	vec.z = Math::ToRadian(vec.z);

	Rotation(vec);

}

D3DXVECTOR3 Sprite::RotationDegree()
{
	D3DXVECTOR3 vec;
	vec.x = Math::ToDegree(rotation.x);
	vec.y = Math::ToDegree(rotation.y);
	vec.z = Math::ToDegree(rotation.z);

	return vec;
}



//-----------------------------------------------------------------------------
//Sprites
//-----------------------------------------------------------------------------
map<wstring, Sprites::SpriteDesc> Sprites::spriteMap;


ID3D11ShaderResourceView * Sprites::Load(wstring file)
{
	if (spriteMap.count(file) > 0)
	{
		spriteMap[file].RefCount++;

		return spriteMap[file].SRV;
	}

	HRESULT hr;
	ID3D11ShaderResourceView* srv;
	hr = D3DX11CreateShaderResourceViewFromFile
	(
		Device, file.c_str(), NULL, NULL, &srv, NULL
	);
	assert(SUCCEEDED(hr));

	SpriteDesc desc;
	desc.RefCount++;
	desc.SRV = srv;
	spriteMap[file] = desc;

	return desc.SRV;

}

void Sprites::Remove(wstring file)
{
	UINT count = spriteMap.count(file);
	assert(count > 0);

	spriteMap[file].RefCount--;
	if (spriteMap[file].RefCount < 1)
	{
		SAFE_RELEASE(spriteMap[file].SRV);

		spriteMap.erase(file);
	}

}
