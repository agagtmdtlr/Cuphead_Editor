#pragma once

#include <windows.h>
#include <assert.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
using namespace std;

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dX10.h>
#include <d3dx10math.h>
#include <d3dx11effect.h>
#include <d3dcompiler.h>

#include <mmsystem.h>
#include <dsound.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "effects11d.lib")
#pragma comment(lib, "d3dcompiler.lib")

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//DirectWrite
/*
d2d1_2.h direct2d에 사용되는 헤더
interface:
	ID2D1Device : 개체 및 장치 컨텍스트를 함께 사용할 수 있는 리소스 도메인을 나타냅니다.
	ID2D1DeviceContext : 지오메트리 실현 객체를 생성하고 그릴수 있다.
	ID2D1Factory : direct2d 리소르를 생성합니다.

*/
#include <d2d1_2.h> 
/*
dwrite.h DirectWrite에서 사용됩니다.
interface:
	 WriteBitmapRenderTarget
	 WriteFactory : DirectWrite 개체를 만드는데 사용된다.
	 WriteFont : 글꼴 컬렉션의 실제 글꼴을 나타낸다.
	 WriteFontColletion : 시스템에 설치된 글꼴 집합이나 특정 디렉터리의 글꼴 집합과 같은
	 글꼴 집합을 캡슐화하는 개체
*/
#include <dwrite.h> // 
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


//ImGui
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

#include "Systems/Keyboard.h"
#include "Systems/CMouse.h"
#include "Systems/Time.h"
#include "Systems/DirectWrite.h"
#include "Systems/Grid.h"
#include "Systems/Line.h"

#include "Renders/Shader.h"
#include "Renders/Sprite.h"
#include "Renders/Rect.h"
#include "Renders/Clip.h"
#include "Renders//Animation.h"

#include "Utilities/Math.h"	
#include "Utilities/Path.h"
#include "Utilities/String.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

#include "Sounds/Sound.h"
#include "Sounds/soundclass.h"

#include "Scenes/Scene.h"

#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p){ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p); (p) = NULL; } } 

#include "Objects/Object.h" // 모든 객체는 object의 자손이다. (editor 와 모드에서 이 모든걸 관리한다)
#include "Objects/Player.h"
#include "Objects/Enemy.h"
#include "Objects/Marker.h"
#include "Objects/Liner.h"
#include "Objects/StaticObject.h"

#include "Objects/Duck.h"

#include "PlayerState/PlayerState.h"
#include "PlayerState/AimState.h"
#include "PlayerState/DashState.h"
#include "PlayerState/DuckState.h"
#include "PlayerState/IdleState.h"
#include "PlayerState/JumpState.h"
#include "PlayerState/OnAirState.h"
#include "PlayerState/OnGroundState.h"
#include "PlayerState/ParryState.h"
#include "PlayerState/RunState.h"

#include "Objects/Boss.h"
#include "Objects/Pipe_Phase1.h"

#include "BossState/BossState.h"
#include "BossState/Phase1_IdleState.h"
#include "BossState/Phase1_IntroState.h"
#include "BossState/Phase1_AttackState.h"
#include "BossState/Phase1_DeathState.h"
#include "BossState/Phase2_IdleState.h"
#include "BossState/Phase2_IntroState.h"
#include "BossState/Phase2_AttackState.h"
#include "BossState/Phase2_DeathState.h"
#include "BossState/Phase3_IdleState.h"
#include "BossState/Phase3_IntroState.h"
#include "BossState/Phase3_AttackState.h"
#include "BossState/Phase3_DeathState.h"
#include "BossState/Phase4_IdleState.h"
#include "BossState/Phase4_IntroState.h"
#include "BossState/Phase4_AttackState.h"
#include "BossState/Phase4_DeathState.h"

enum Edit_Mode
{
	Mode_Create,
	Mode_Delete,
	Mode_Move,
	Mode_Draw
};

enum class Edit_Target
{
	BackGround,
	Marker
};


const wstring Shaders = L"../_Shaders/";
const wstring Textures = L"../_Textures/";

// 변경가능
extern UINT Width;
extern UINT Height;

extern HWND Hwnd;
extern wstring Title;

extern IDXGISwapChain* SwapChain;
extern ID3D11Device* Device;
extern ID3D11DeviceContext* DeviceContext;
extern ID3D11RenderTargetView* RTV;

extern Keyboard* Key;
extern CMouse* Mouse;
extern Time* Timer;

extern D3DXVECTOR2 horizontal;
extern D3DXVECTOR2 vertical;






