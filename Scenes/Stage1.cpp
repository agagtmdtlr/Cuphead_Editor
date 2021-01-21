#include "stdafx.h"
#include "Stage1.h"
#include "Objects/Player.h"
#include "Viewer/Following.h"
#include "Objects/Bullet.h"

Stage1::Stage1(SceneValues* values)
	:Scene(values)
{
	wstring shaderFile = Shaders + L"008_Sprite.fx";

	backGrounds[0] = new Sprite(Textures + L"Stage2.png", shaderFile, 26, 311, 613, 503);
	backGrounds[0]->Position(400, 350);
	backGrounds[0]->Scale(1.2f, 2.6f);

	backGrounds[1] = new Sprite(Textures + L"Stage2.png", shaderFile, 13, 11, 714, 215);
	backGrounds[1]->Position(400, 350);
	backGrounds[1]->Scale(1.2f, 2.6f);

	backGrounds[2] = new Sprite(Textures + L"Stage2.png", shaderFile, 13, 242, 894, 279);
	backGrounds[2]->Position(400, 40);
	backGrounds[2]->Scale(1.2f, 2.6f);


	player = new Player(D3DXVECTOR2(130, 100), D3DXVECTOR2(2.5f, 2.5f));
	bullet = new Bullet(Shaders + L"008_Sprite.fx", D3DXVECTOR2(300, 300), 0, 0);


}

Stage1::~Stage1()
{
	SAFE_DELETE(fire);
	SAFE_DELETE(player);
	SAFE_DELETE(bullet);

	SAFE_DELETE(backGrounds[0]);
	SAFE_DELETE(backGrounds[1]);
	SAFE_DELETE(backGrounds[2]);
}

void Stage1::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	backGrounds[0]->Update(V, P);
	backGrounds[1]->Update(V, P);
	backGrounds[2]->Update(V, P);

	player->Update(V, P);
	bullet->Update(V, P);
	
}

void Stage1::Render()
{
	RenderBackground();

	static bool check = false;

	Sprite* a = bullet->GetClip()->GetSprite();


	a->DrawCollision(check);
	ImGui::LabelText("OBB", "%d", check ? 1 : 0);

	player->Render();
	bullet->Render();
}

void Stage1::RenderBackground()
{
	backGrounds[0]->Render();
	backGrounds[1]->Render();
	backGrounds[2]->Render();
}
