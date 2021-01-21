#include "stdafx.h"
#include "Sonic.h"
#include "Objects/Player.h"
#include "Objects/Marker.h"
#include "Objects/Liner.h"
#include "Viewer/Freedom.h"

#include "Editor/Editor.h"


//D3DXVECTOR2 horizontal, vertical;

Editor* editor;

Sonic::Sonic(SceneValues * values)
	: Scene(values),clickedMarker(nullptr)
{
	wstring shaderFile = Shaders + L"008_Sprite.fx";

	backGround = new Sprite(Textures + L"cuphead/pipe/background/clown_bg_track.png", shaderFile);
	backGround->Position(0, -300);

	player = new Player(D3DXVECTOR2(0, 500), D3DXVECTOR2(1.0f, 1.0f));

	((Freedom*)(values->MainCamera))->Position(0, 0);

	//sound = new Sound();
	//sound->Initiialize("D:/dev/dev_project/Direct2D/_Audios/cuphead/clown_bgm/MUS_CarnivalKerfuffle_MixB.wav");
	//sound2 = new Sound();
	//sound2->Initiialize("D:/dev/dev_project/Direct2D/_Audios/cuphead/intro/MUS_Intro_DontDealWithDevil_Vocal.wav");
	
	//soundClass = new SoundClass();
	//soundClass->Initialize(Hwnd);
	//
	editor = new Editor(values);
	
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

Sonic::~Sonic()
{
	for (Marker* marker : markers)
		SAFE_DELETE(marker);

	SAFE_DELETE(player);
	SAFE_DELETE(backGround);

	sound->Shutdown();
	SAFE_DELETE(sound);
}

void Sonic::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;	

	// 클릭하여 마커 선택하기
	if (Key->Down(VK_LBUTTON))
	{
		D3DXVECTOR2 position = ClickPosition();

		clickedMarker = grid->Pop(position);
		if (clickedMarker != nullptr)
		{
			clickedStartClickedPosition = position;
			markerStartPosition = clickedMarker->Position();
		}		
	}

	// 클릭한 마커 드래그 하기
	if (Key->Press(VK_LBUTTON) && clickedMarker != nullptr)
	{
		D3DXVECTOR2 movePos = ClickPosition() - clickedStartClickedPosition;		
		clickedMarker->position = movePos + markerStartPosition;
	}

	// 클릭한 마커 설정 끝
	if (Key->Up(VK_LBUTTON) && clickedMarker != nullptr)
	{
		D3DXVECTOR2 movePos = ClickPosition() - clickedStartClickedPosition;
		clickedMarker->position = movePos + markerStartPosition;
		grid->Add(clickedMarker);
		clickedMarker = nullptr;
	}


	if (Key->Down(VK_SPACE) || Key->Down(VK_RBUTTON))
	{
		
		// 해당 마우스의 월드 좌표에 마커 넣기
		D3DXVECTOR2 position = ClickPosition();

		if (Key->Press(VK_LSHIFT) && imsiMarkers.size() > 0)
		{
			D3DXVECTOR2 backPos = imsiMarkers.back()->Position();
			float xDest = abs(position.x - backPos.x);
			float yDest = abs(position.y - backPos.y);

			if (yDest < xDest)
			{
				position.y = backPos.y;
			}
			else if (yDest > xDest)
			{
				position.x = backPos.x;
			}

		}


		Marker* marker = new Marker(grid, Shaders + L"008_Sprite.fx", position);
		markers.push_back(marker);
		imsiMarkers.push_back(marker); // 라인에 사용할 마카
	}

	

	if (imsiMarkers.size() >= 2 && !Key->Press(VK_CONTROL))
	{
		auto ends = imsiMarkers.end();
		--ends;
		for (auto begin = imsiMarkers.begin(); begin != ends;)
		{
			//Liner* liner = new Liner(imsiMarkers[0], imsiMarkers[1]);
			Marker* first = *begin;
			begin++;
			Marker* second = *begin;
			Liner* liner = new Liner(first, second);
			liners.push_back(liner);
			
		}
		imsiMarkers.clear();
	}

	player->GetSprite()->DrawCollision(false);

	// 라인들과 플레이어 선 출동 검사하기
	// player 의 선은 플레이어 중심에서 바닥까지의 직선이다
	/*
	-------
	|     |
	|  |  | <- 바운딩 박스 중심에서 바닥까지의 선을 사용한다
	-------
	(180 회전시 플레이서 선의 위치가 0~1... 정도의 오차범위가 발생하므로 보정을 해준다.
	*/
	for (Liner* liner : liners) // 모든 직선들과 검사 ( 전탐색 ) 
	{
		
		if (CheckLineColl(player, liner)) // 플레이어와 출동했다면
		{
			Sprite * sprite = player->GetSprite();
			sprite->DrawCollision(true); // 바운딩 박스 충돌됬음으로 색상 변환
			
			// 선분의 두 점(선의 양 끝)의 위치를 가져온다.
			D3DXVECTOR2 & p2 = liner->secondMarker->position;
			D3DXVECTOR2 & p1 = liner->firstMarker->position;

			if (abs(p2.x - p1.x) <= 0) // 수직으로 놓여있으므로 바닥이 아니라 벽이다.
				break;

			// PLANE 각도에 맞게 기울이기
			// 바닥 벡터를 구한다.
			D3DXVECTOR2 plane = p2 - p1;
			D3DXVECTOR2 planeNormal;
			// 바닥 노멀 벡터
			D3DXVec2Normalize(&planeNormal, &plane);

			// plane 에 수직노말벡터 ( 플레이어가 바닥에서 기울어질 벡터이다)
			// 왜냐하면 플레이어는 바닥에 일반적으로 수직으로 세워져 있으니깐
			// 바닥이 경사져도 그 바닥에 수직이니깐 경사에 맞춰 플레이어가 기울어질 것이다.
			D3DXVECTOR2 normalVec = D3DXVECTOR2(-planeNormal.y, planeNormal.x);
			// 수직 축 ( 기본 캐릭트 렌더링 축 )
			// 플레이어는 기본족으로 y축에 평행하게 세워져 있으므로
			// 기울어질 벡터와의 사잇각을 구하기 위한 기준 벡터가 된다.
			D3DXVECTOR2 axis = { 0 , 1 };
			
			//현재 필요한 각 구하기/////////////
			// 사잇각 구하기
			// 바닥의 수직벡터와 플레이어 기준 벡터의 사잇각을 구한다(플레이어를 얼마나 회전시킬지 구한다)
			float zDegree = Math::VectorDegree(normalVec, axis);
			
			// 이 부분 계산을 오차범위를 벗어났지는 판별하기 위한 부분이다.
			float pZ = player->zDegree;
			float pY = player->yDegree;
			//player->animation->RotationDegree(0, player->yDegree, zDegree);
			if (abs(pZ - zDegree) >= 1)
			{
				player->zDegree = zDegree;
			}			
			
			// 현재 플레이어의 위치와 회전정보를 가지고 있는
			// world matrix를 가지고 온다.			
			D3DXMATRIX world = sprite->World();

			//D3DXVECTOR2 playerPosition = D3DXVECTOR2(world._41, world._42);
			D3DXVECTOR2 playerPosition = player->animation->Position();

			D3DXVECTOR2 xDir = D3DXVECTOR2(world._11, world._12) * 0.5;
			D3DXVECTOR2 yDir = D3DXVECTOR2(world._21, world._22) * 0.5;

			float x = playerPosition.x;	
			
			//center
			// 두점을 이용한 직선의 방정식을 이용해서
			// 플레이어가 바닥에 놓일 y값의 위치를 계산한다.
			// 함수의 대입값으로 player의 x값을 그대로 사용하지 않는것은
			// 실제 바닥과 닿는 위치는 바닥부분이기 때문이다.
			// 현제 렌더링 방식은 사물의 위치좌표가 스프라이트의 정중앙에
			// 놓여 있기 때문이다.
			//float y = (p2.y - p1.y) / (p2.x - p1.x) * ((playerPosition - yDir).x - p1.x) + p1.y;			
			//center_bottom
			float y = (p2.y - p1.y) / (p2.x - p1.x) * (playerPosition.x - p1.x) + p1.y;			

			// 직선의 방정식을 통해 계산한 새로운 위치 좌표를 갱신한다.
			D3DXVECTOR2 newPos = D3DXVECTOR2(x, y);			
			//newPos.y += yDir.y + xDir.y -0.5;
			// center
			// 위의 설명과 같이 플레이어의 실제 위치는 스프라이트의 정중앙을
			// 기준으로 하고 있으므로 바닥에서 중심까지로 이동시켜준다.
			//newPos.y += yDir.y;

			player->animation->Position(newPos);			
			
			player->bOnGround = true;
			player->moveDir = planeNormal;

			break;
		}
		else // 바닥에 닿지 않은 경우
		{
			Sprite * sprite = player->GetSprite();
			sprite->DrawCollision(false);
			D3DXVECTOR2 temp = player->moveDir;
			player->bOnGround = false;
			player->zDegree = 0;
			player->moveDir = { 1, 0 };
		}
	}


	for (Marker* marker : markers)
		marker->Update(V, P);

	for (Liner* liner : liners)
		liner->Update(V, P);

	if (clickedMarker != nullptr)
	{
		clickedMarker->Update(V, P);
	}

	backGround->Update(V, P);
	player->state->handleInput(player);
	player->state->Update(player,V, P);
}
int vol = 0;

void Sonic::Render()
{
	editor->Render();

	

	D3DXVECTOR2 mouse = Mouse->Position();
	mouse.x = mouse.x - (float)Width * 0.5f;
	mouse.y = (mouse.y - (float)Height * 0.5f) * -1.0f;
	D3DXVECTOR2 position = mouse;
	position.x *= (float)(horizontal.y - horizontal.x) / Width;
	position.y *= (float)(vertical.y - vertical.x) / Height;
	ImGui::LabelText("Position", "%.0f, %.0f", position.x, position.y);
	
	//ImGui::SliderInt("Volume", &vol, -10000, 0);
	//sound2->SetVolume(vol);
	//ImGui::LabelText("Left Down", "%d", Mouse->Down(0) ? 1 : 0);
	//ImGui::LabelText("Left Up", "%d", Mouse->Up(0) ? 1 : 0);
	//ImGui::LabelText("Left Press", "%d", Mouse->Press(0) ? 1 : 0);
	//ImGui::LabelText("Left Doubleclick", "%d", Mouse->DoubleClick(0) ? 1 : 0);

	//ImGui::LabelText("Wheel", "%f", Mouse->Wheel());
	//ImGui::LabelText("horizontal", "%f %f", horizontal.x , horizontal.y);
	//ImGui::LabelText("vertical", "%f %f", vertical.x, vertical.y);

	backGround->Render();

	for (Marker* marker : markers)
		marker->Render();

	for (Liner* liner : liners)
		liner->Render();

	D3DXVECTOR2 pos = player->state->animation->Position();
	ImGui::LabelText("Position", "%.0f, %.0f", pos.x, pos.y);
	player->state->Render();
}

bool Sonic::CheckLineColl(Player * player, Liner * liner)
{
	bool result = false;
	Sprite* sprite = player->GetSprite();
	D3DXVECTOR2 scale = sprite->Scale();
	D3DXVECTOR2 textureSize = sprite->TextureSize();
	D3DXMATRIX world = sprite->World();
	
	D3DXVECTOR2 playerPosition = D3DXVECTOR2(world._41, world._42);

	D3DXVECTOR2 xDir = D3DXVECTOR2(world._11, world._12) * 0.5;
	D3DXVECTOR2 yDir = D3DXVECTOR2(world._21, world._22) * 0.5;

	//D3DXVECTOR2 line1_p1 = playerPosition - xDir - yDir;
	//D3DXVECTOR2 line1_p2 = playerPosition + xDir - yDir;

	// 오차범위를 보정하기 위함
	D3DXVECTOR2 yDirNormal;
	D3DXVec2Normalize(&yDirNormal, &yDir);
	
	D3DXVECTOR2 line1_p1 = playerPosition - yDir - yDirNormal * 2;
	D3DXVECTOR2 line1_p2 = playerPosition;
	//D3DXVECTOR2 line1_p1 = playerPosition;
	//D3DXVECTOR2 line1_p2 = playerPosition + yDir;


	D3DXVECTOR2 line2_p1 = liner->firstMarker->position;
	D3DXVECTOR2 line2_p2 = liner->secondMarker->position;

	result = Line::LineSegmentIntersection(line1_p1, line1_p2, line2_p1, line2_p2);
	

	return result;
}
