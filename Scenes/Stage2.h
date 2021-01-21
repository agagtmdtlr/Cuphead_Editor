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
	
	// class 헤더 추가 없이
	class Player* player;
	
};