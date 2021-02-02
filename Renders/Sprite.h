#pragma once

enum class RenderType
{
	center,
	left_bottom,
	center_bottom
};

class Sprite
{
public:
	Sprite(wstring textureFile, wstring shaderFile , RenderType renderType = RenderType::center);
	Sprite(wstring textureFile, wstring shaderFile, float endX, float endY, RenderType renderType = RenderType::center);
	Sprite(wstring testureFile, wstring shaderFile, float startX, float startY, float endX, float endY, RenderType renderType = RenderType::center);

	virtual ~Sprite();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	void DrawBound(bool val) { bDrawBound = val; }
	void DrawCollision(bool val) { bDrawCollision = val; }

	RenderType GetRenderType() { return renderType; }
public:
	RECT BoundBox();

	bool AABB(D3DXVECTOR2& position);
	bool AABB(Sprite* b);
	bool OBB(Sprite* b);

	static bool AABB(Sprite* a, D3DXVECTOR2& position);
	static bool AABB(Sprite* a, Sprite* b);
	static bool OBB(Sprite* a, Sprite* b);

private:
	struct OBBDesc
	{
		D3DXVECTOR2 Position;
		D3DXVECTOR2 Direction[2];
		float Length[2];
	};

	static void CreateOBB(OUT OBBDesc* out, D3DXVECTOR2& position, D3DXMATRIX& world, D3DXVECTOR2& length);
	static float SeperateAxis(D3DXVECTOR2& seperate, D3DXVECTOR2& e1, D3DXVECTOR2& e2);
	static bool CheckOBB(OBBDesc& obbA, OBBDesc& obbB);

private:
	void CreateBound();

	void UpdateBoundBox();

public:
	void Position(float x, float y);
	void Position(D3DXVECTOR2& vec);
	D3DXVECTOR2 Position() { return position; }

	void BoundPosition(float x, float y);
	void BoundPosition(D3DXVECTOR2& vec);
	D3DXVECTOR2 BoundPosition() { return bound_position; }

	void BoundTextureSize(float x, float y);
	void BoundTextureSize(D3DXVECTOR2 & vec);

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2& vec);
	D3DXVECTOR2 Scale() { return scale; }

	void Rotation(float x, float y, float z);
	void Rotation(D3DXVECTOR3& vec);
	D3DXVECTOR3 Rotation() { return rotation; }

	void RotationDegree(float x, float y, float z);
	void RotationDegree(D3DXVECTOR3& vec);
	D3DXVECTOR3 RotationDegree();

	D3DXVECTOR2 TextureSize() { return textureSize; }

	D3DXMATRIX World() { return world; }



private:
	void Initialize(wstring spriteFile, wstring shaderFile, float startX, float startY, float endX, float endY);

private:
	RenderType renderType;

private:


	bool bDrawBound;
	bool bDrawCollision;
	wstring textureFile;

	Shader* shader;
	ID3D11Buffer* vertexBuffer;

	Shader* boundShader;
	ID3D11Buffer* boundVertexBuffer;
	ID3D11ShaderResourceView* srv;

	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR2 textureSize;
	D3DXVECTOR3 rotation;
	
	D3DXMATRIX world;

	D3DXVECTOR2 bound_position;
	D3DXVECTOR2 bound_scale;
	D3DXVECTOR2 bound_textureSize;
	D3DXVECTOR3 bound_rotation;

	D3DXMATRIX bound_world;

	RECT boundbox;


private:
	struct Vertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 UV;
	};

	struct BoundVertex
	{
		D3DXVECTOR3 Position;
	};
};

class Sprites
{
private:
	friend class Sprite;

private:
	static ID3D11ShaderResourceView* Load(wstring file);
	static void Remove(wstring file);

private:
	struct SpriteDesc
	{
		UINT RefCount = 0;
		ID3D11ShaderResourceView* SRV = NULL;
	};

	static map<wstring, SpriteDesc> spriteMap;
};
