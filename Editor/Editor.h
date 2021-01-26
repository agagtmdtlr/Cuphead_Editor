#pragma once

enum class Edit_Category
{
	Camera,
	Line,
	Layer
};

class Editor
{
public:
	Editor(SceneValues * values);
	~Editor();

	void Update(D3DXMATRIX & V, D3DXMATRIX & P);
	void Render();

public:
	D3DXVECTOR2 ClickPosition()
	{
		D3DXVECTOR2 mouse = Mouse->Position();

		mouse.x = mouse.x - (float)Width * 0.5f;
		mouse.y = (mouse.y - (float)Height * 0.5f) * -1.0f;
		
		D3DXVECTOR2 camera = values->MainCamera->Position();
		D3DXVECTOR2 position = mouse;

		position.x *= (float)(horizontal.y - horizontal.x) / Width;
		position.y *= (float)(vertical.y - vertical.x) / Height;

		position += camera;

		return position;
	}

	bool MouseInImgui()
	{
		ImVec2 imsi = ImGui::GetWindowPos();
		D3DXVECTOR2 imPos = { imsi.x, imsi.y };
		imPos.x = imPos.x - (float)Width * 0.5f;
		imPos.y = (imPos.y - (float)Height * 0.5f) * -1.0f;
		imPos.x *= (float)(horizontal.y - horizontal.x) / Width;
		imPos.y *= (float)(vertical.y - vertical.x) / Height;
		D3DXVECTOR2 sizes;

		sizes.x = ImGui::GetWindowWidth();
		sizes.y = ImGui::GetWindowHeight();
		sizes.x *= (float)(horizontal.y - horizontal.x) / Width;
		sizes.y *= (float)(vertical.y - vertical.x) / Height;

		imPos += values->MainCamera->Position();

		D3DXVECTOR2 mousePos = Mouse->Position();
		mousePos.x = mousePos.x - (float)Width * 0.5f;
		mousePos.y = (mousePos.y - (float)Height * 0.5f) * -1.0f;

		mousePos.x *= (float)(horizontal.y - horizontal.x) / Width;
		mousePos.y *= (float)(vertical.y - vertical.x) / Height;

		mousePos += values->MainCamera->Position();

		bool mouseInImgui = false;
		if (mousePos.x >= imPos.x &&
			mousePos.x <= imPos.x + sizes.x &&
			mousePos.y <= imPos.y &&
			mousePos.y >= imPos.y - sizes.y)
		{
			mouseInImgui = true;
		}

		return mouseInImgui;
	}
	
private:
	SceneValues * values;

	vector<int> layers;
	int layers_n = 0;

	int selected_layer = -1;

	Grid* grid;


private:
	class Object* clickedObject;
	D3DXVECTOR2 clickedStartClickedPosition;
	D3DXVECTOR2 StartPosition;

private:
	Edit_Category edit_category;

	Sprite* backGround;

	class Player* player;
	vector<class Object*> objects;

	// Mode to Draw Liner
	vector<class Marker*> markerToDrawLiner;
	vector<class Liner*> liners;

	vector<Sprite*> static_Object;

	vector<vector<Object*>*> static_layers;


private:
	void Camera_Edit(D3DXMATRIX &V, D3DXMATRIX &P);
	void Line_Edit(D3DXMATRIX &V, D3DXMATRIX &P);
	void Layer_Edit(D3DXMATRIX &V, D3DXMATRIX &P);

};