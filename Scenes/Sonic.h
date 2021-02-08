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
		// 마우스 & 메인카메라 동차좌표
		// 메인카메라 중심 위치가 0,0
		// 마우스 기준 좌상단 기준을 중심 기준으로 변경하여 메인카메라와 중심 위치 동일시키지
		mouse.x = mouse.x - (float)Width * 0.5f;
		// 좌상단 기준에선 증가지만 중심좌표 기준에선 감소 이므로 -1.0f로 부호 변경
		mouse.y = (mouse.y - (float)Height * 0.5f) * -1.0f;

		// 현재 카메라 위치 (0,0)에서 얼마나 움직였는가
		D3DXVECTOR2 camera = values->MainCamera->Position();
		// 현재 화면 마우스 좌표와 카메라가 월드에서 움직임 좌표가 더해져 마우스과 월드에 가리키는 상대적인 좌표를 얻는다.

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