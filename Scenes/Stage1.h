#pragma once
#include "Scene.h"

class Stage1 : public Scene
{
public:
	Stage1(SceneValues * values);
	~Stage1();

	void Update() override;
	void Render() override;

private:
	void RenderBackground();

private:
	Sprite* backGrounds[3];
	
	// class 헤더 추가 없이
	class Player* player;
	class Bullet* bullet;
	class Fire* fire;
};