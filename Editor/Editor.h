#pragma once

class Editor
{
public:
	Editor(SceneValues * values);
	~Editor();
	void Update(D3DXMATRIX & V, D3DXMATRIX & P);
	void Render();


	
private:
	SceneValues * values;
};