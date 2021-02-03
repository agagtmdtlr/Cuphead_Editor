#include "stdafx.h"
#include "EditScene.h"
#include "Viewer/Freedom.h"

EditScene::EditScene(SceneValues * values)
	:Scene(values), clickedObject(nullptr)
{
	grid = new Grid();


	wstring shaderFile = Shaders + L"008_Sprite.fx";

	backGround = new Sprite(Textures + L"cuphead/pipe/background/clown_bg_track.png", shaderFile);
	backGround->Position(0, -300);

	{
		Object_Desc desc;
		desc.b_bound_coll = true;
		desc.b_line_coll = true;
		desc.b_render = true;
		desc.label = OBJECT_LABEL::player;
		desc.layer_index = 1; // add a marker layer
		player = new Player(grid, D3DXVECTOR2(0, 500), D3DXVECTOR2(1.0f, 1.0f), desc, values);
	}
	

	((Freedom*)(values->MainCamera))->Position(0, 0);

	Object_Desc desc;
	{
		Marker* marker1 = new Marker(grid, Shaders + L"008_Sprite.fx",
			D3DXVECTOR2(-300, -225), desc, values);
		Marker* marker2 = new Marker(grid, Shaders + L"008_Sprite.fx",
			D3DXVECTOR2(300, -225), desc, values);
		Liner* liner = new Liner(marker1, marker2);

		objects.push_back(marker1);
		objects.push_back(marker2);
		liners.push_back(liner);
	}
}

EditScene::~EditScene()
{
	for (Object* object : objects)
		SAFE_DELETE(object);

	SAFE_DELETE(player);
	SAFE_DELETE(backGround);

	sound->Shutdown();
	SAFE_DELETE(sound);
}

void EditScene::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

}

//int vol = 0;

void EditScene::Render()
{
	
	/*static int mode = 0;
	if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
	if (ImGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
	if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }*/

	ImGui::LabelText("Mouse Position", "%.0f, %.0f", Mouse->Position().x, Mouse->Position().y);

}

bool EditScene::CheckLineColl(Player * player, Liner * liner)
{
	return false;
}

void EditScene::MoveObject(D3DXMATRIX & V, D3DXMATRIX & P)
{
	// 클릭하여 마커 선택하기
	if (Key->Down(VK_LBUTTON))
	{
		D3DXVECTOR2 position = ClickPosition();

		clickedObject = grid->Pop(position);
		if (clickedObject != nullptr)
		{
			clickedStartClickedPosition = position;
			markerStartPosition = clickedObject->Position();
		}
	}

	// 클릭한 마커 드래그 하기
	if (Key->Press(VK_LBUTTON) && clickedObject != nullptr)
	{
		D3DXVECTOR2 movePos = ClickPosition() - clickedStartClickedPosition;
		clickedObject->position = movePos + markerStartPosition;
	}

	// 클릭한 마커 설정 끝
	if (Key->Up(VK_LBUTTON) && clickedObject != nullptr)
	{
		D3DXVECTOR2 movePos = ClickPosition() - clickedStartClickedPosition;
		clickedObject->position = movePos + markerStartPosition;
		grid->Add(clickedObject);
		clickedObject = nullptr;
	}
}

void EditScene::CreateObject(D3DXMATRIX & V, D3DXMATRIX & P)
{
}
