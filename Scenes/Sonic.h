#pragma once
#include "Scene.h"
#include "Systems/Grid.h"
#include "Objects/Object.h"

class Sonic : public Scene
{
public:
	Sonic(SceneValues* values);
	~Sonic();

	void Update() override;
	void Render() override;

	class Sound* sound;
	class Sound* sound2;
	class SoundClass* soundClass;
	
public:
	Grid* grid;

public:

	D3DXVECTOR2 ClickPosition()
	{
		D3DXVECTOR2 mouse = Mouse->Position();
		// ���콺 & ����ī�޶� ������ǥ
		// ����ī�޶� �߽� ��ġ�� 0,0
		// ���콺 ���� �»�� ������ �߽� �������� �����Ͽ� ����ī�޶�� �߽� ��ġ ���Ͻ�Ű��
		mouse.x = mouse.x - (float)Width * 0.5f;
		// �»�� ���ؿ��� �������� �߽���ǥ ���ؿ��� ���� �̹Ƿ� -1.0f�� ��ȣ ����
		mouse.y = (mouse.y - (float)Height * 0.5f) * -1.0f;

		// ���� ī�޶� ��ġ (0,0)���� �󸶳� �������°�
		D3DXVECTOR2 camera = values->MainCamera->Position();
		// ���� ȭ�� ���콺 ��ǥ�� ī�޶� ���忡�� ������ ��ǥ�� ������ ���콺�� ���忡 ����Ű�� ������� ��ǥ�� ��´�.

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
	void OpenComplete(wstring name);
	void SaveComplete(wstring name);

private:
	Object* clickedObject;
	D3DXVECTOR2 clickedStartClickedPosition;
	D3DXVECTOR2 markerStartPosition;

	Sprite* backGround;

	Clip * screenfx;

	class Player* player;
	vector<class Object*> objects;

	vector<class Marker*> markerToDrawLiner;
	vector<class Liner*> liners;

	class Editor* editor;
};