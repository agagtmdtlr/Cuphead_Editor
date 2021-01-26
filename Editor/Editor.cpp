#include "stdafx.h"
#include "Editor.h"

Editor::Editor(SceneValues * values)
	:edit_category(Edit_Category::Camera),values(values)
{
	for (layers_n = 0; layers_n < 9; layers_n++)
	{
		layers.push_back(layers_n);
	}

	grid = new Grid();

	wstring shaderFile = Shaders + L"008_Sprite.fx";


	backGround = new Sprite(Textures + L"cuphead/pipe/background/clown_bg_track.png", shaderFile);
	
	
	backGround->Position(0, -300);
}

Editor::~Editor()
{

	SAFE_DELETE(grid);
}

void Editor::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (ImGui::RadioButton("Camera", edit_category == Edit_Category::Camera)) { edit_category = Edit_Category::Camera; } ImGui::SameLine();
	if (ImGui::RadioButton("Line", edit_category == Edit_Category::Line)) { edit_category = Edit_Category::Line; } ImGui::SameLine();
	if (ImGui::RadioButton("Layer", edit_category == Edit_Category::Layer)) { edit_category = Edit_Category::Layer; }
	
	if (edit_category == Edit_Category::Camera)
		Camera_Edit(V, P);
	if (edit_category == Edit_Category::Line)
		Line_Edit(V, P);
	if (edit_category == Edit_Category::Layer)
		Layer_Edit(V, P);

	{
		ImVec2 imPos = ImGui::GetWindowPos();
		imPos.x = imPos.x - (float)Width * 0.5f;
		imPos.y = (imPos.y - (float)Height * 0.5f) * -1.0f;

		imPos.x *= (float)(horizontal.y - horizontal.x) / Width;
		imPos.y *= (float)(vertical.y - vertical.x) / Height;

		D3DXVECTOR2 sizes;

		sizes.x = ImGui::GetWindowWidth();
		sizes.y = ImGui::GetWindowHeight();

		ImGui::LabelText("imgui Pos", "%.0f, %.0f", imPos.x, imPos.y);
		ImGui::LabelText("imgui sizes", "%.0f, %.0f", sizes.x, sizes.y);
	}

	
}

void Editor::Render()
{
	
	
	
}

void Editor::Camera_Edit(D3DXMATRIX & V, D3DXMATRIX & P)
{	
	{
		ImGui::BulletText("Drag Mouse and Move Camera position");
		ImGui::Indent();			
		ImGui::Unindent();
	}
	
	if (MouseInImgui())
		return;

	D3DXVECTOR2 movePos;
	D3DXVECTOR2 position;
	if (Key->Down(VK_LBUTTON))
	{
		position = ClickPosition();

		clickedStartClickedPosition = position;
		StartPosition = values->MainCamera->Position();
	}

	movePos = ClickPosition() - clickedStartClickedPosition;

	if (Key->Press(VK_LBUTTON))
	{		
		values->MainCamera->Position(-movePos + StartPosition);
	}
	else if (Key->Up(VK_LBUTTON))
	{
		values->MainCamera->Position(-movePos + StartPosition);
	}
}

void Editor::Line_Edit(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (Key->Down(VK_LBUTTON))
	{
		D3DXVECTOR2 position = ClickPosition();

		clickedObject = grid->Pop(position);
		if (clickedObject != nullptr)
		{
			clickedStartClickedPosition = position;
			StartPosition = clickedObject->Position();
		}		
	}

	// 클릭한 마커 드래그 하기
	if (Key->Press(VK_LBUTTON) && clickedObject != nullptr)
	{
		D3DXVECTOR2 movePos = ClickPosition() - clickedStartClickedPosition;		
		clickedObject->position = movePos + StartPosition;
	}

	// 클릭한 마커 설정 끝
	if (Key->Up(VK_LBUTTON) && clickedObject != nullptr)
	{
		D3DXVECTOR2 movePos = ClickPosition() - clickedStartClickedPosition;
		clickedObject->position = movePos + StartPosition;
		grid->Add(clickedObject);
		clickedObject = nullptr;
	}
	// Add Marker
	if (Key->Down(VK_SPACE) || Key->Down(VK_RBUTTON))
	{
		D3DXVECTOR2 clickPosition = ClickPosition();

		// 수평 수직 선 만들기
		if (Key->Press(VK_LSHIFT) && markerToDrawLiner.size() > 0)
		{
			D3DXVECTOR2 backPos = markerToDrawLiner.back()->Position();
			float xDest = abs(clickPosition.x - backPos.x);
			float yDest = abs(clickPosition.y - backPos.y);

			if (yDest < xDest)
			{
				clickPosition.y = backPos.y;
			}
			else if (yDest > xDest)
			{
				clickPosition.x = backPos.x;
			}
		}

		Marker* marker = new Marker(grid, Shaders + L"008_Sprite.fx", clickPosition);
		objects.push_back(marker);
		markerToDrawLiner.push_back(marker); // 라인에 사용할 마카
	}

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

void Editor::Layer_Edit(D3DXMATRIX & V, D3DXMATRIX & P)
{
	if (ImGui::TreeNode("Layer"))
	{

		{
			ImGui::BulletText("Drag and drop to swap layers");
			ImGui::Indent();
			enum Mode
			{
				Mode_Add,
				Mode_Delete,
				Mode_Swap
			};
			static int mode = 0;
			if (ImGui::RadioButton("Add", mode == Mode_Add)) { mode = Mode_Add; } ImGui::SameLine();
			if (ImGui::RadioButton("Delete", mode == Mode_Delete)) { mode = Mode_Delete; } ImGui::SameLine();
			if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }

			if (mode == Mode_Add)
			{
				if (ImGui::Button("Add layer"))
				{
					layers.push_back(layers_n++);
				}
			}

			for (int n = 0; n < layers.size(); n++)
			{
				ImGui::PushID(n);
				if ((n % 3) != 0)
					ImGui::SameLine();
				string bt_str = "layer" + to_string(layers[n]);
				if (ImGui::Button(bt_str.c_str(), ImVec2(60, 60)))
				{
					if (mode == Mode_Delete)
					{
						layers.erase(layers.begin() + n);
					}
					else
					{
						selected_layer = n;
					}
				}

				// Our buttons are both drag sources and drag targets here!
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("DND_DEMO_CE", &n, sizeof(int));        // Set payload to carry the index of our item (could be anything)
					if (mode == Mode_Swap) { ImGui::Text("Swap %s", bt_str.c_str()); }
					else { ImGui::Text("Please Check Swap Radio and Swap %s", bt_str.c_str()); }
					ImGui::EndDragDropSource();
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CE"))
					{
						IM_ASSERT(payload->DataSize == sizeof(int));
						int payload_n = *(const int*)payload->Data;

						if (mode == Mode_Swap)
						{
							int tmp = layers[n];
							layers[n] = layers[payload_n];
							layers[payload_n] = tmp;
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::PopID();
			}
			ImGui::Unindent();
		}

		ImGui::TreePop();
	}
}
