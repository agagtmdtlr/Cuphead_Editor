#pragma once

class Liner
{
	friend class Sonic;
	friend class Grid;
public:
	Liner();
	Liner(class Marker* firstPoint, class Marker* secondPoint, Object_Mode mode = Object_Mode::Editor);

	virtual ~Liner();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	void DrawBound(bool val) { bDrawBound = val; }
	void DrawCollision(bool val) { bDrawCollision = val; }

public:
	void Position(float x, float y);
	void Position(D3DXVECTOR2& vec);
	D3DXVECTOR2 Position() { return position; }

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

private:
	void Draw(class Marker* firstPoint, class Marker* secondPoint);

private:
	void Initialize();

private:
	class Marker* firstMarker;
	class Marker* secondMarker;

	D3DXVECTOR2 prevFirstPos;
	D3DXVECTOR2 prevSecondPos;


private:
	Object_Mode obj_mode;

	bool bDrawBound;
	bool bDrawCollision;

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

private:
	struct Vertex
	{
		D3DXVECTOR3 Position;
	};

	struct BoundVertex
	{
		D3DXVECTOR3 Position;
	};
};
