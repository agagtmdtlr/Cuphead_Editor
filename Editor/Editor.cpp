#include "stdafx.h"
#include "Editor.h"

Editor::Editor(SceneValues * values)
{
}

Editor::~Editor()
{
}

void Editor::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
}

void Editor::Render()
{
	

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
	
	ImGui::LabelText("imPos", "%.0f, %.0f", imPos.x, imPos.y);
	ImGui::LabelText("sizes", "%.0f, %.0f", sizes.x, sizes.y);

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

	if (ImGui::TreeNode("Drag and Drop"))
	{
		{
			// ColorEdit widgets automatically act as drag source and drag target.
			// They are using standardized payload strings IMGUI_PAYLOAD_TYPE_COLOR_3F and IMGUI_PAYLOAD_TYPE_COLOR_4F to allow your own widgets
			// to use colors in their drag and drop interaction. Also see the demo in Color Picker -> Palette demo.
			ImGui::BulletText("Drag and drop in standard widgets");
			ImGui::Indent();
			static float col1[3] = { 1.0f,0.0f,0.2f };
			static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
			ImGui::ColorEdit3("color 1", col1);
			ImGui::ColorEdit4("color 2", col2);
			ImGui::Unindent();
		}

		{
			ImGui::BulletText("Drag and drop to copy/swap items");
			ImGui::Indent();
			enum Mode
			{
				Mode_Copy,
				Mode_Move,
				Mode_Swap
			};
			static int mode = 0;
			if (ImGui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ImGui::SameLine();
			if (ImGui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ImGui::SameLine();
			if (ImGui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }
			static const char* names[9] = { "Bobby", "Beatrice", "Betty", "Brianna", "Barry", "Bernard", "Bibi", "Blaine", "Bryn" };
			for (int n = 0; n < IM_ARRAYSIZE(names); n++)
			{
				ImGui::PushID(n);
				if ((n % 3) != 0)
					ImGui::SameLine();
				ImGui::Button(names[n], ImVec2(60, 60));

				// Our buttons are both drag sources and drag targets here!
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("DND_DEMO_CE", &n, sizeof(int));        // Set payload to carry the index of our item (could be anything)
					if (mode == Mode_Copy) { ImGui::Text("Copy %s", names[n]); }        // Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
					if (mode == Mode_Move) { ImGui::Text("Move %s", names[n]); }
					if (mode == Mode_Swap) { ImGui::Text("Swap %s", names[n]); }
					ImGui::EndDragDropSource();
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CE"))
					{
						IM_ASSERT(payload->DataSize == sizeof(int));
						int payload_n = *(const int*)payload->Data;
						if (mode == Mode_Copy)
						{
							names[n] = names[payload_n];
						}
						if (mode == Mode_Move)
						{
							names[n] = names[payload_n];
							names[payload_n] = "";
						}
						if (mode == Mode_Swap)
						{
							const char* tmp = names[n];
							names[n] = names[payload_n];
							names[payload_n] = tmp;
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
