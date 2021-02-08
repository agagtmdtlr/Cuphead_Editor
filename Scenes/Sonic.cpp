#include "stdafx.h"
#include "Sonic.h"
#include "Viewer/Freedom.h"
#include "Editor/Editor.h"

Sonic::Sonic(SceneValues * values)
	: Scene(values), clickedObject(nullptr)
{
	grid = new Grid();

	wstring shaderFile = Shaders + L"008_Sprite.fx";

	backGround = new Sprite(Textures + L"cuphead/pipe/background/clown_bg_track.png", shaderFile);
	backGround->Position(0, -300);

	((Freedom*)(values->MainCamera))->Position(0, 0);

	//sound = new Sound();
	//sound->Initiialize("D:/dev/dev_project/Direct2D/_Audios/cuphead/clown_bgm/MUS_CarnivalKerfuffle_MixB.wav");
	//sound2 = new Sound();
	//sound2->Initiialize("D:/dev/dev_project/Direct2D/_Audios/cuphead/intro/MUS_Intro_DontDealWithDevil_Vocal.wav");
	
	//soundClass = new SoundClass();
	//soundClass->Initialize(Hwnd);
	//
	editor = new Editor(values);	

	shaderFile = Shaders + L"001_Screen.fx";
	screenfx = new Clip(PlayMode::Loop);
	for (int i = 1; i < 127; i++)
	{
		wstring texturepth = Textures + L"cuphead/common/screen_fx/cuphead_screen_fx_";
		texturepth += (i < 100 ? (i < 10 ? L"000" + to_wstring(i) : L"00" + to_wstring(i)) : L"0" + to_wstring(i));
		texturepth += L".png";
		screenfx->AddFrame(new Sprite(texturepth, shaderFile), 0.07f);
	}
	screenfx->Scale(Width / 1024.0f, Height / 512.0f);

	screenfx->Play();
}

Sonic::~Sonic()
{
	for (Object* object : objects)
		SAFE_DELETE(object);

	SAFE_DELETE(player);
	SAFE_DELETE(backGround);

	sound->Shutdown();
	SAFE_DELETE(sound);

	SAFE_DELETE(soundClass);
}

void Sonic::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;	

	//// Ŭ���Ͽ� ��Ŀ �����ϱ�
	//if (Key->Down(VK_LBUTTON))
	//{
	//	D3DXVECTOR2 position = ClickPosition();

	//	clickedObject = grid->Pop(position);
	//	if (clickedObject != nullptr)
	//	{
	//		clickedStartClickedPosition = position;
	//		markerStartPosition = clickedObject->Position();
	//	}		
	//}

	//// Ŭ���� ��Ŀ �巡�� �ϱ�
	//if (Key->Press(VK_LBUTTON) && clickedObject != nullptr)
	//{
	//	D3DXVECTOR2 movePos = ClickPosition() - clickedStartClickedPosition;		
	//	clickedObject->position = movePos + markerStartPosition;
	//}

	//// Ŭ���� ��Ŀ ���� ��
	//if (Key->Up(VK_LBUTTON) && clickedObject != nullptr)
	//{
	//	D3DXVECTOR2 movePos = ClickPosition() - clickedStartClickedPosition;
	//	clickedObject->position = movePos + markerStartPosition;
	//	grid->Add(clickedObject);
	//	clickedObject = nullptr;
	//}

	// ���ε�� �÷��̾� �� �⵿ �˻��ϱ�
	// player �� ���� �÷��̾� �߽ɿ��� �ٴڱ����� �����̴�
	/*
	-------
	|     |
	|  |  | <- �ٿ�� �ڽ� �߽ɿ��� �ٴڱ����� ���� ����Ѵ�
	-------
	(180 ȸ���� �÷��̼� ���� ��ġ�� 0~1... ������ ���������� �߻��ϹǷ� ������ ���ش�.
	*/
	{
		for (Liner* liner : liners) // ��� ������� �˻� ( ��Ž�� ) 
		{

			if (CheckLineColl(player, liner)) // �÷��̾�� �⵿�ߴٸ�
			{
				Sprite * sprite = player->GetSprite();
				sprite->DrawCollision(true); // �ٿ�� �ڽ� �浹�������� ���� ��ȯ

				// ������ �� ��(���� �� ��)�� ��ġ�� �����´�.
				D3DXVECTOR2 & p2 = liner->secondMarker->position;
				D3DXVECTOR2 & p1 = liner->firstMarker->position;

				if (abs(p2.x - p1.x) <= 0) // �������� ���������Ƿ� �ٴ��� �ƴ϶� ���̴�.
					break;

				// PLANE ������ �°� ����̱�
				// �ٴ� ���͸� ���Ѵ�.
				D3DXVECTOR2 plane = p2 - p1;
				D3DXVECTOR2 planeNormal;
				// �ٴ� ��� ����
				D3DXVec2Normalize(&planeNormal, &plane);

				// plane �� �����븻���� ( �÷��̾ �ٴڿ��� ������ �����̴�)
				// �ֳ��ϸ� �÷��̾�� �ٴڿ� �Ϲ������� �������� ������ �����ϱ�
				// �ٴ��� ������� �� �ٴڿ� �����̴ϱ� ��翡 ���� �÷��̾ ������ ���̴�.
				D3DXVECTOR2 normalVec = D3DXVECTOR2(-planeNormal.y, planeNormal.x);
				// ���� �� ( �⺻ ĳ��Ʈ ������ �� )
				// �÷��̾�� �⺻������ y�࿡ �����ϰ� ������ �����Ƿ�
				// ������ ���Ϳ��� ���հ��� ���ϱ� ���� ���� ���Ͱ� �ȴ�.
				D3DXVECTOR2 axis = { 0 , 1 };

				//���� �ʿ��� �� ���ϱ�/////////////
				// ���հ� ���ϱ�
				// �ٴ��� �������Ϳ� �÷��̾� ���� ������ ���հ��� ���Ѵ�(�÷��̾ �󸶳� ȸ����ų�� ���Ѵ�)
				float zDegree = Math::VectorDegree(normalVec, axis);

				// �� �κ� ����� ���������� ������� �Ǻ��ϱ� ���� �κ��̴�.
				float pZ = player->zDegree;
				float pY = player->yDegree;
				//player->animation->RotationDegree(0, player->yDegree, zDegree);
				if (abs(pZ - zDegree) >= 1)
				{
					player->zDegree = zDegree;
				}

				// ���� �÷��̾��� ��ġ�� ȸ�������� ������ �ִ�
				// world matrix�� ������ �´�.			
				D3DXMATRIX world = sprite->World();

				//D3DXVECTOR2 playerPosition = D3DXVECTOR2(world._41, world._42);
				D3DXVECTOR2 playerPosition = player->animation->Position();

				D3DXVECTOR2 xDir = D3DXVECTOR2(world._11, world._12) * 0.5;
				D3DXVECTOR2 yDir = D3DXVECTOR2(world._21, world._22) * 0.5;

				float x = playerPosition.x;

				RenderType obj_renderType = sprite->GetRenderType();

				float y;
				D3DXVECTOR2 newPos;
				switch (obj_renderType)
				{
					case RenderType::center:
						y = (p2.y - p1.y) / (p2.x - p1.x) * ((playerPosition - yDir).x - p1.x) + p1.y;
						newPos = D3DXVECTOR2(x, y);
						newPos.y += yDir.y;
						break;
					case RenderType::left_bottom:
						break;
					case RenderType::center_bottom:
						y = (p2.y - p1.y) / (p2.x - p1.x) * (playerPosition.x - p1.x) + p1.y;
						newPos = D3DXVECTOR2(x, y);
						break;
					default:
						break;
				}

				//center
				// ������ �̿��� ������ �������� �̿��ؼ�
				// �÷��̾ �ٴڿ� ���� y���� ��ġ�� ����Ѵ�.
				// �Լ��� ���԰����� player�� x���� �״�� ������� �ʴ°���
				// ���� �ٴڰ� ��� ��ġ�� �ٴںκ��̱� �����̴�.
				// ���� ������ ����� �繰�� ��ġ��ǥ�� ��������Ʈ�� ���߾ӿ�
				// ���� �ֱ� �����̴�.
				//float y = (p2.y - p1.y) / (p2.x - p1.x) * ((playerPosition - yDir).x - p1.x) + p1.y;			
				//center_bottom
				//float y = (p2.y - p1.y) / (p2.x - p1.x) * (playerPosition.x - p1.x) + p1.y;			

				// ������ �������� ���� ����� ���ο� ��ġ ��ǥ�� �����Ѵ�.
				//D3DXVECTOR2 newPos = D3DXVECTOR2(x, y);			
				//newPos.y += yDir.y + xDir.y -0.5;
				// center
				// ���� ����� ���� �÷��̾��� ���� ��ġ�� ��������Ʈ�� ���߾���
				// �������� �ϰ� �����Ƿ� �ٴڿ��� �߽ɱ����� �̵������ش�.
				//newPos.y += yDir.y;

				player->animation->Position(newPos);

				player->bOnGround = true;
				player->moveDir = planeNormal;

				break;
			}
			else // �ٴڿ� ���� ���� ���
			{
				Sprite * sprite = player->GetSprite();
				sprite->DrawCollision(false);
				D3DXVECTOR2 temp = player->moveDir;
				player->bOnGround = false;
				player->zDegree = 0;
				player->moveDir = { 1, 0 };
			}
		}

	}
	

	if (clickedObject != nullptr)
	{
		clickedObject->Update(V, P);
	}

	backGround->Update(V, P);
	editor->Update(V, P);
	screenfx->Update(V, P);

}

void Sonic::Render()
{

	/*if (ImGui::Button("Save Binary") == true)
	{
		function<void(wstring)> f = bind(&Sonic::SaveComplete, this, placeholders::_1);
		Path::SaveFileDialog(L"", L"Binary\0*.bin", L".", f, Hwnd);
	}

	if (ImGui::Button("Load Binary") == true)
	{
		function<void(wstring)> f = bind(&Sonic::OpenComplete, this, placeholders::_1);
		Path::OpenFileDialog(L"", L"Binary\0*.bin", L".", f, Hwnd);

	}*/

	editor->Render();
	screenfx->Render();
	
	//ImGui::SliderInt("Volume", &vol, -10000, 0);
	//sound2->SetVolume(vol);
	//ImGui::LabelText("Left Down", "%d", Mouse->Down(0) ? 1 : 0);
	//ImGui::LabelText("Left Up", "%d", Mouse->Up(0) ? 1 : 0);
	//ImGui::LabelText("Left Press", "%d", Mouse->Press(0) ? 1 : 0);
	//ImGui::LabelText("Left Doubleclick", "%d", Mouse->DoubleClick(0) ? 1 : 0);

	//ImGui::LabelText("Wheel", "%f", Mouse->Wheel());
	//ImGui::LabelText("horizontal", "%f %f", horizontal.x , horizontal.y);
	//ImGui::LabelText("vertical", "%f %f", vertical.x, vertical.y);


	//D3DXVECTOR2 pos = player->state->animation->Position();
	//ImGui::LabelText("player Position", "%.0f, %.0f", pos.x, pos.y);

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

	// ���������� �����ϱ� ����
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


void Sonic::OpenComplete(wstring name)
{

	if (Path::ExistFile(name) == true)
	{
		for (Object* object : objects)
		{
			SAFE_DELETE(object);
		}

		for (Liner* liner : liners)
		{
			SAFE_DELETE(liner)				
		}

		objects.clear();
		liners.clear();
		grid->Reset();

		BinaryReader* r = new BinaryReader();
		r->Open(name);

		UINT count;
		count = r->UInt();

		vector<D3DXVECTOR2> v;
		v.assign(count, D3DXVECTOR2());

		void* ptr = (void *)&(v[0]);
		r->Byte(&ptr, sizeof(D3DXVECTOR2) * count);

		
		for (UINT i = 0; i < count; i++)
		{
			Object_Desc desc;
			Marker* marker = new Marker(grid, Shaders + L"008_Sprite.fx", v[i], desc, values);
			objects.push_back(marker);
			markerToDrawLiner.push_back(marker); // ���ο� ����� ��ī

			if (markerToDrawLiner.size() >= 2)
			{
				auto ends = markerToDrawLiner.end();
				--ends;
				for (auto begin = markerToDrawLiner.begin(); begin != ends;)
				{
					//Liner* liner = new Liner(imsiMarkers[0], imsiMarkers[1]);
					Marker* first = *begin;
					begin++;
					Marker* second = *begin;
					Liner* liner = new Liner(first, second);
					liners.push_back(liner);

				}
				markerToDrawLiner.clear();
			}
		}

		r->Close();
		SAFE_DELETE(r);
	}

	MessageBox(Hwnd, name.c_str(), L"Open", MB_OK);

}

void Sonic::SaveComplete(wstring name)
{
	BinaryWriter* w = new BinaryWriter();
	w->Open(name);

	vector<D3DXVECTOR2> v;
	for (Object* object : objects)
	{
		v.push_back(object->Position());
	}

	w->UInt(v.size());
	w->Byte(&v[0], sizeof(D3DXVECTOR2) * v.size());

	w->Close();
	SAFE_DELETE(w);

	wstring temp = name + L"\n������ �Ϸ�Ǿ���";

	MessageBox(Hwnd, temp.c_str(), L"save complte", MB_OK);
}
