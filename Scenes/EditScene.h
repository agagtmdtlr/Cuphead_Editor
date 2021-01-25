#pragma once
#include "Scene.h"

class EditScene : public Scene
{
public:
	EditScene(SceneValues* values);
	~EditScene();

	void Update() override;
	void Render() override;

	class Sound* sound;
	class Sound* sound2;
	class SoundClass* soundClass;


	class Grid* grid;

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

	bool CheckLineColl(class Player* player, class Liner* liner);

	int CCW(D3DXVECTOR2 p1, D3DXVECTOR2 p2, D3DXVECTOR2 p3)
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


private:
	class Object* clickedObject;
	D3DXVECTOR2 clickedStartClickedPosition;
	D3DXVECTOR2 markerStartPosition;

	Sprite* backGround;

	class Player* player;
	vector<class Object*> objects;

	vector<class Marker*> markerToDrawLiner;
	vector<class Liner*> liners;

private:

	function<void(D3DXMATRIX &, D3DXMATRIX &)> updateFunc;

	void MoveObject(D3DXMATRIX & V, D3DXMATRIX & P);
	void CreateObject(D3DXMATRIX & V, D3DXMATRIX & P);
};

