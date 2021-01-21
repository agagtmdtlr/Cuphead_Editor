#pragma once

class Marker : public Object
{
	friend class Grid;
	friend class Liner;
	friend class Sonic;

public:
	Marker(Grid* grid_, wstring shaderFile, D3DXVECTOR2 start);
	~Marker();

	void Move(D3DXVECTOR2 position_);

	virtual D3DXVECTOR2 Position() { return position; }
	virtual void Position(D3DXVECTOR2) override;

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P) override;
	virtual void Render() override;

	Clip* GetClip() { return clip; }

	

private:
	Clip* clip;

	

private:
	Grid* grid;

	Marker* prev;
	Marker* next;
};