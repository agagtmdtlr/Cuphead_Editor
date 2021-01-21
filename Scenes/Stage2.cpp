#include "stdafx.h"
#include "Stage2.h"
#include "Objects/Player.h"
#include "Viewer/Following.h"


Stage2::Stage2(SceneValues* values)
	:Scene(values)
{
	wstring shaderFile = Shaders + L"008_Sprite.fx";

	backGrounds = new Sprite(Textures + L"Stage.png", shaderFile);
	backGrounds->Position(0, 300);
	backGrounds->Scale(2.5f, 2.5f);

	player = new Player(D3DXVECTOR2(130, 140), D3DXVECTOR2(2.5f, 2.5f));

	SAFE_DELETE(values->MainCamera);
	values->MainCamera = new Following(player);

}

Stage2::~Stage2()
{
	SAFE_DELETE(player);

	SAFE_DELETE(backGrounds);
}

void Stage2::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	backGrounds->Update(V, P);

	player->Update(V, P);

}

void Stage2::Render()
{
	backGrounds->Render();


	player->Render();

}


