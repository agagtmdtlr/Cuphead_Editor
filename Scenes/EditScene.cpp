#include "stdafx.h"
#include "EditScene.h"
#include "Objects/Player.h"
#include "Objects/Marker.h"
#include "Objects/Liner.h"
#include "Viewer/Freedom.h"

EditScene::EditScene(SceneValues * values)
	:Scene(values), clickedMarker(nullptr)
{
	wstring shaderFile = Shaders + L"008_Sprite.fx";

	backGround = new Sprite(Textures + L"cuphead/pipe/background/clown_bg_track.png", shaderFile);
	backGround->Position(0, -300);

	player = new Player(D3DXVECTOR2(0, 500), D3DXVECTOR2(1.0f, 1.0f));

	((Freedom*)(values->MainCamera))->Position(0, 0);

	grid = new Grid();
	{

		Marker* marker1 = new Marker(grid, Shaders + L"008_Sprite.fx",
			D3DXVECTOR2(-300, -225));
		Marker* marker2 = new Marker(grid, Shaders + L"008_Sprite.fx",
			D3DXVECTOR2(300, -225));
		Liner* liner = new Liner(marker1, marker2);

		markers.push_back(marker1);
		markers.push_back(marker2);
		grid->Add(marker1);
		grid->Add(marker2);
		liners.push_back(liner);
	}
}

EditScene::~EditScene()
{
	for (Marker* marker : markers)
		SAFE_DELETE(marker);

	SAFE_DELETE(player);
	SAFE_DELETE(backGround);

	sound->Shutdown();
	SAFE_DELETE(sound);
}

void EditScene::Update()
{


}

//int vol = 0;

void EditScene::Render()
{
	ImGui::LabelText("Mouse Position", "%.0f, %.0f", Mouse->Position().x, Mouse->Position().y);

}

bool EditScene::CheckLineColl(Player * player, Liner * liner)
{
	return false;
}
