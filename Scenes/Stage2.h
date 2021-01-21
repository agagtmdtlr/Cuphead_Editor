#pragma once
#include "Scene.h"

class Stage2 : public Scene
{
public:
	Stage2(SceneValues * values);
	~Stage2();

	void Update() override;
	void Render() override;

private:
	Sprite* backGrounds;
	
	// class ��� �߰� ����
	class Player* player;
	
};