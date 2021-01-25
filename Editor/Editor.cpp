#include "stdafx.h"
#include "Editor.h"

Editor::Editor(SceneValues * values)
{
	for (layers_n = 0; layers_n < 9; layers_n++)
	{
		layers.push_back(layers_n);
	}
}

Editor::~Editor()
{
}

void Editor::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
}

void Editor::Render()
{
	if (ImGui::TreeNode("In columns"))
	{
		ImGui::Columns(3, NULL, false);
		static bool selected[16] = { 0 };
		for (int i = 0; i < 16; i++)
		{
			char label[32]; sprintf(label, "Item %d", i);
			if (ImGui::Selectable(label, &selected[i])) {}
			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
		ImGui::CheckboxFlags("ImGuiInputTextFlags_ReadOnly", (unsigned int*)&flags, ImGuiInputTextFlags_ReadOnly);
		ImGui::CheckboxFlags("ImGuiInputTextFlags_AllowTabInput", (unsigned int*)&flags, ImGuiInputTextFlags_AllowTabInput);
		ImGui::CheckboxFlags("ImGuiInputTextFlags_CtrlEnterForNewLine", (unsigned int*)&flags, ImGuiInputTextFlags_CtrlEnterForNewLine);
		
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Child windows"))
	{
		static bool disable_mouse_wheel = false;
		static bool disable_menu = false;
		ImGui::Checkbox("Disable Mouse Wheel", &disable_mouse_wheel);
		ImGui::Checkbox("Disable Menu", &disable_menu);

		static int line = 50;
		bool goto_line = ImGui::Button("Goto");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		goto_line |= ImGui::InputInt("##Line", &line, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);
		ImGui::PopItemWidth();
		

		ImGui::SameLine();

		// Child 2: rounded border
		{
			ImGuiWindowFlags window_flags = (disable_mouse_wheel ? ImGuiWindowFlags_NoScrollWithMouse : 0) | (disable_menu ? 0 : ImGuiWindowFlags_MenuBar);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild("Child2", ImVec2(0, 260), true, window_flags);
			
			ImGui::Columns(2);
			for (int i = 0; i < 100; i++)
			{
				char buf[32];
				sprintf(buf, "%03d", i);
				ImGui::Button(buf, ImVec2(-1.0f, 0.0f));
				ImGui::NextColumn();
			}
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}

		
		ImGui::TreePop();
	}

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

	/*enum Mode
	{
		Mode_Copy,
		Mode_Move,
		Mode_Swap
	};
	static int mode = 0;
	if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
	if (ImGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
	if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }*/

	

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
				if(ImGui::Button("Add layer"))
				{ 
					layers.push_back(layers_n++);
				}
			}

			for (int n = 0; n <layers.size(); n++)
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
					else{ ImGui::Text("Please Check Swap Radio and Swap %s", bt_str.c_str()); }
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
