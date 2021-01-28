#include "stdafx.h"
#include "Editor.h"

Editor::Editor(SceneValues * values)
	:edit_category(Edit_Category::Camera),values(values)
{
	wstring shaderFile = Shaders + L"008_Sprite.fx";
	grid = new Grid();

	{
		Object_Desc desc;
		desc.b_bound_coll = true;
		desc.b_line_coll = true;
		desc.b_render = true;
		desc.label = OBJECT_LABEL::player;
		desc.layer_index = 1; // add a marker layer
		desc.texturePath = L"";
		player = new Player(grid, D3DXVECTOR2(0, 0), D3DXVECTOR2(1.0f, 1.0f), desc);
	}

	for (layers_n = 0; layers_n < 9; layers_n++)
	{
		Objects_Layer* obj_layer = new Objects_Layer;
		obj_layer->layer = new vector<Object*>;
		layers.push_back(make_pair(layers_n, obj_layer));
	}

	grid = new Grid();

	

	backGround = new Sprite(Textures + L"cuphead/pipe/background/clown_bg_track.png", shaderFile);
	backGround->Position(0, -300);

	{
		Object_Desc desc;
		Marker* marker1 = new Marker(grid, Shaders + L"008_Sprite.fx",
			D3DXVECTOR2(-300, -225), desc);
		Marker* marker2 = new Marker(grid, Shaders + L"008_Sprite.fx",
			D3DXVECTOR2(300, -225), desc);
		Liner* liner = new Liner(marker1, marker2);

		objects.push_back(marker1);
		objects.push_back(marker2);
		liners.push_back(liner);
	}
}

Editor::~Editor()
{
	for (auto layer : layers)
	{
		for (auto obj : *layer.second->layer)		
			SAFE_DELETE(obj); // delete dynamic object in layer
		SAFE_DELETE(layer.second->layer); // delete dynamic object vector
		SAFE_DELETE(layer.second); // delete dynamic object_layer struct
	}

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

	if (selected_layer > -1)
	{
		auto  obj_layer = layers[selected_layer].second;		
		auto  layer = obj_layer->layer;
		for (auto obj : *layer)
		{
			const Object_Desc & desc = obj->object_desc;
		}
	}


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

	
	D3DXMATRIX V_m = values->MainCamera->View();
	D3DXMATRIX P_m = values->Projection;

	for (Object* object : objects)
		object->Update(V_m, P_m);
	for (Liner* liner : liners)
		liner->Update(V_m, P_m);
	player->Update(V_m, P_m);

}

void Editor::Render()
{
	ImGui::Text(to_string(layers.size()).c_str());
	for (auto obj_layer : layers)
	{
		if (obj_layer.second->visualized == true)
		{
			for (auto obj : *obj_layer.second->layer)
			{
				obj->Render();
			}
		}		
	}

	for (Object* object : objects)
		object->Render();

	for (Liner* liner : liners)
		liner->Render();

	ImGui::LabelText("objects size", "%d", objects.size());
	ImGui::LabelText("liners size", "%d", liners .size());

	player->Render();
	D3DXVECTOR2 mouse = Mouse->Position();
	mouse.x = mouse.x - (float)Width * 0.5f;
	mouse.y = (mouse.y - (float)Height * 0.5f) * -1.0f;
	D3DXVECTOR2 position = mouse;
	position.x *= (float)(horizontal.y - horizontal.x) / Width;
	position.y *= (float)(vertical.y - vertical.x) / Height;
	ImGui::LabelText("Mouse Window Position", "%.0f, %.0f", position.x, position.y);
	D3DXVECTOR2 wmp = ClickPosition();
	ImGui::LabelText("Mouse World Position", "%.0f, %.0f", wmp.x, wmp.y);

	
	auto imsi = ClickPosition();
	auto imsi2 = values->MainCamera->Position();
	ImGui::LabelText("Click pos", "%f %f", imsi.x, imsi.y);
	ImGui::LabelText("Cam pos", "%f %f", imsi2.x, imsi2.y);

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
		position = Mouse->Position();
		
		position.x = position.x - (float)Width * 0.5f;
		position.y = (position.y - (float)Height * 0.5f) * -1.0f;
		clickedStartClickedPosition = position;
		StartPosition = values->MainCamera->Position();
	}

	D3DXVECTOR2 moveAfter = Mouse->Position();
	moveAfter.x = moveAfter.x - (float)Width * 0.5f;
	moveAfter.y = (moveAfter.y - (float)Height * 0.5f) * -1.0f;
	movePos = moveAfter - clickedStartClickedPosition;
	
	if (Key->Press(VK_LBUTTON))
	{		
		values->MainCamera->Position(-movePos + StartPosition);
		values->MainCamera->Update();
	}
	else if (Key->Up(VK_LBUTTON))
	{
		values->MainCamera->Position(-movePos + StartPosition);
		values->MainCamera->Update();
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

	// 클릭한 object 드래그 하기
	if (Key->Press(VK_LBUTTON) && clickedObject != nullptr)
	{
		D3DXVECTOR2 movePos = ClickPosition() - clickedStartClickedPosition;		
		clickedObject->position = movePos + StartPosition;
	}

	// 클릭한 object 설정 끝
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

		Object_Desc desc;
		desc.b_bound_coll = false;
		desc.b_line_coll = false;
		desc.b_render = false;
		desc.label = OBJECT_LABEL::marker;
		desc.layer_index = marker_layer; // add a marker layer
		desc.texturePath = L"";

		Marker* marker = new Marker(grid, Shaders + L"008_Sprite.fx", clickPosition, desc);
		objects.push_back(marker);
		layers[marker_layer].second->layer->push_back(marker);
		
		// add a draw vector
		markerToDrawLiner.push_back(marker); // 라인에 사용할 마카
	}

	// CHECK ENOUGH MARKERS NUMBER TO DRAW LINE
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

			// add a marker layer
			layers[marker_layer].second->layer->push_back(first); // 마카는 마카 레이어에 집어 넣는다.
			layers[marker_layer].second->layer->push_back(second); // 마카는 마카 레이어에 집어 넣는다.

			Liner* liner = new Liner(first, second);
			liners.push_back(liner);

		}
		markerToDrawLiner.clear();
	}
}

void Editor::Layer_Edit(D3DXMATRIX & V, D3DXMATRIX & P)
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

		if (mode == Mode_Add) // click button and add a layer vector end line
		{
			if (ImGui::Button("Add layer"))
			{
				Objects_Layer * obj_layer = new Objects_Layer;
				obj_layer->layer = new vector<Object*>;
				layers.push_back(make_pair(layers_n++, obj_layer));
			}
		}
		//////////////////////////////////////////////////
		{
			string bt_str = "marker\nlayer";
			if (ImGui::Button(bt_str.c_str(), ImVec2(60, 60)))
			{
				selected_layer = 0;
			}
			ImGui::SameLine();
			ImGui::Checkbox("", &layers[marker_layer].second->visualized);
			
		}		
		//////////////////////////////////////
		for (int n = 1; n < layers.size(); n++)
		{
			ImGui::PushID(n);
			if ((n % 3) != 0)
				ImGui::SameLine();
			int layer_n = layers[n].first;
			string bt_str = "layer" + to_string(layer_n);
			if (ImGui::Button(bt_str.c_str(), ImVec2(60, 60)))
			{
				if (mode == Mode_Delete)
				{
					vector<Object*>* del_layer = layers[n].second->layer;
					for (Object* obj : *del_layer)
					{
						SAFE_DELETE(obj);
					}
					layers.erase(layers.begin() + n);

					if (n == selected_layer)
					{
						selected_layer = -1;
					}
					
				}
				else
				{
					selected_layer = n;
				}
			}
			ImGui::SameLine();
			if(layers.size() > 1)
				ImGui::Checkbox("", &layers[n].second->visualized);

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
						pair<int,Objects_Layer*> tmp = layers[n];
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

	
}

void Editor::OpenComplete(wstring name)
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
			Marker* marker = new Marker(grid, Shaders + L"008_Sprite.fx", v[i], desc);
			objects.push_back(marker);
			markerToDrawLiner.push_back(marker); // 라인에 사용할 마카

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

void Editor::SaveComplete(wstring name)
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

	wstring temp = name + L"\n저장이 완료되었음";

	MessageBox(Hwnd, temp.c_str(), L"save complte", MB_OK);
}


void Editor::SelectLayer(int layer_index)
{
	grid->Reset();
	for (Object* obj : *layers[layer_index].second->layer)
	{
		grid->Add(obj);
	}
}