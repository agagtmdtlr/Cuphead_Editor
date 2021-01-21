#pragma once

class Camera // 상속 클래스 ?? 누구를???
{
public:
	Camera();
	virtual ~Camera();

	D3DXMATRIX View() { return view; }
	D3DXVECTOR2 Position() { return position; }
	
	virtual void Update();
protected:
	D3DXVECTOR2 position;

private:
	D3DXMATRIX view;

};