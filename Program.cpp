#include "stdafx.h"
#include "./Systems/Device.h"
#include "Scenes/Scene.h"
#include "Viewer/Freedom.h"

#include "Scenes/Stage1.h"
#include "Scenes/Stage2.h"
#include "Scenes/Sonic.h"

SceneValues* values;
vector<Scene*> scenes;

//D3DXVECTOR2 horizontal(0, 800), vertical(0, 600);
D3DXVECTOR2 horizontal, vertical;

void InitScene()
{
	

	values = new SceneValues();
	values->MainCamera = new Freedom();
	D3DXMatrixIdentity(&values->Projection);

	//scenes.push_back(new Stage1(values));
	//scenes.push_back(new Stage2(values));
	scenes.push_back(new Sonic(values));

	horizontal.x = (float)Width * -0.5f;
	horizontal.y = (float)Width * 0.5f;
	vertical.x = (float)Height * -0.5f;
	vertical.y = (float)Height * 0.5f;

}
void DestroyScene()
{
	for (Scene* scene : scenes)
		SAFE_DELETE(scene);

	SAFE_DELETE(values->MainCamera);
	SAFE_DELETE(values);
}



void Update()
{
	values->MainCamera->Update();
	// ����ü ����
	float wheel = Mouse->Wheel();
	if (wheel != 0)
	{
		float speed = 0.25f;
		horizontal.x -= wheel * speed * Width / Height;
		horizontal.y += wheel * speed * Width / Height;
		vertical.x -= wheel * speed * Height / Height;
		vertical.y += wheel * speed * Height / Height;
	}
	
	// ���� ���콺 ��ǥ�� ���α׷� ȭ���� ���߾��� �߽���ǥ(0,0)�� �����߱� ������
	// ȭ�� ������ ���콺�� ��ǥ��� ��ġ ��Ű�� ���� �߽��� 0,0���� �ǵ��� �����Ѵ�?

	D3DXMatrixOrthoOffCenterLH(&values->Projection, horizontal.x, horizontal.y, vertical.x, vertical.y, -10, 10);
	//D3DXMatrixOrthoOffCenterLH(&values->Projection, 0, (float)Width, 0, (float)Height, -10, 10);
	
	
	/*D3DXMatrixOrthoOffCenterLH(&values->Projection,
		(float)Width * -0.5f,
		(float)Width * 0.5f,
		(float)Height * -0.5f,
		(float)Height * 0.5f,
		-10, 10);*/



	for (Scene* scene : scenes)
		scene->Update();

}

void Render()
{
	D3DXCOLOR bgColor = D3DXCOLOR(0, 0, 0, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{
		ImGui::SliderFloat2("Horizontal", (float*)&horizontal, -5000, 5000);
		ImGui::SliderFloat2("Vertical", (float*)&vertical, -1000, 1000);
		

		for (Scene* scene : scenes)
			scene->Render();
	}
	ImGui::Render();

	DirectWrite::GetDC()->BeginDraw();
	{
		wstring text = L"";

		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = 600;
		rect.bottom = 20;

		text = L"Frame Per Second : " + to_wstring((int)ImGui::GetIO().Framerate);
		DirectWrite::RenderText(text, rect);

		rect.top += 20;
		rect.bottom += 20;


		text = L"Camera Position : ";
		text += to_wstring((int)values->MainCamera->Position().x);
		text += L", ";
		text += to_wstring((int)values->MainCamera->Position().y);


		DirectWrite::RenderText(text, rect);
	}

	DirectWrite::GetDC()->EndDraw();

	SwapChain->Present(0, 0);
}
