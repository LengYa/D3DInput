#include "Game.h"

//Variables
IDirect3DDevice9			*g_Device = NULL;

HINSTANCE					 g_hInstance = NULL;

HWND						 g_hwnd = NULL;

D3DXMATRIX					 g_matWorld;					//世界坐标

CInputInterface				*g_DInput = NULL;				//InputSystem

bool						 g_LMBDown = FALSE;				//Mouse's left

int							 g_MouseX = 0, g_MouseY = 0;


D3DXMATRIX g_projection;			//投影矩阵
D3DXMATRIX g_ViewMatrix;			//观察，相机坐标

//////////////////////////////////////////////////////////////////////////
//Name:bool Game_Init()
//Describe:Initialize the Game
//////////////////////////////////////////////////////////////////////////
bool Game_Init()
{
	//Initialize the Window
	if (!Init_D3D(D3DDEVTYPE_HAL))
	{
		::MessageBox(NULL, L"Initialize Direct3D Failed", L"Tips", MB_OK);
		return FALSE;
	}

	if (!Init_inputSystem())
	{
		::MessageBox(NULL, L"Failed to initialize the input system", L"Tips", S_OK);
		return FALSE;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
//Name:void Game_Main(float fTimeDelta)
//Describe:the Game's main loop
//////////////////////////////////////////////////////////////////////////
void Game_Main(float fTimeDelta)
{
	D3D_Update(fTimeDelta);
	D3D_Render(fTimeDelta);
}


//////////////////////////////////////////////////////////////////////////
//Name:void Game_Shutdown()
//Describe:the game is over ,so release the all sources
//////////////////////////////////////////////////////////////////////////
void Game_Shutdown()
{
	SAFE_DELETE(g_DInput);
}


//////////////////////////////////////////////////////////////////////////
//Name:void D3D_Update(float fTimeDelta)
//Describe:on-time Update the game's date
//////////////////////////////////////////////////////////////////////////
void D3D_Update(float fTimeDelta)
{
	Update_Input();
	//Set the default world's matrix
	D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, 0.0f);
	g_Device->SetTransform(D3DTS_WORLD, &g_matWorld);

	//Set the default projection's matrix
	D3DXMatrixPerspectiveFovLH(&g_projection, D3DX_PI / 4.0f,
		WINDOW_WIDTH / WINDOW_HEIGHT, 0.1F, 300000.0F);
	g_Device->SetTransform(D3DTS_PROJECTION, &g_projection);

}


//////////////////////////////////////////////////////////////////////////
//Name:void D3D_Render(float fTimeDelta)
//Describe:on-time Render the Game
//////////////////////////////////////////////////////////////////////////
void D3D_Render(float fTimeDelta)
{
	//1.Clear the Screen
	POINT pos;
	pos.x = 0;
	pos.y = 0;
	g_Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100, 255, 255), 1.0f, 0);


	//2.Start BeginScene

	//3.End Draw

	g_Device->EndScene();

	g_Device->Present(NULL, NULL, NULL, NULL);  // 翻转与显示
}

bool Init_inputSystem()
{
	//create the input system's object
	if (!CreateDIInput(&g_DInput, g_hwnd, g_hInstance, false))
	{
		::MessageBox(NULL, L"Failed to create the InputSystem", L"Error", MB_OK);
		return false;
	}

	if (!g_DInput->Initialize())
	{
		::MessageBox(NULL, L"Failed to initialize the InputSystem's object", L"Error", MB_OK);
		return false;
	}
	return true;
}

void Update_Input()
{
	if (!g_DInput) return;

	// Update all devices.
	g_DInput->UpdateDevices();

	// Keyboard Input.
	if (g_DInput->KeyDown(DIK_W))
	{
		::MessageBox(NULL, L"It is W", L"Tips", MB_OK);
	}
	else if (g_DInput->KeyDown(DIK_S))
	{
		::MessageBox(NULL, L"It is S", L"Tips", MB_OK);
	}
	else if (g_DInput->KeyDown(DIK_D))
	{
		::MessageBox(NULL, L"It is D", L"Tips", MB_OK);
	}
	else if (g_DInput->KeyDown(DIK_A))
	{
		::MessageBox(NULL, L"It is A", L"Tips", MB_OK);
	}
	
	POINT ps{};
	GetCursorPos(&ps);		//Get the position of the mouse

	//check the mouse
	if (g_DInput->MouseButtonDown(LEFT_BUTTON))
	{
		::MessageBox(NULL, L"It is the left button of mouse", L"Tips", MB_OK);
	}
	else if (g_DInput->MouseButtonDown(RIGHT_BUTTON))
	{
		::MessageBox(NULL, L"It is the right button of mouse", L"Tips", MB_OK);
	}

}


//////////////////////////////////////////////////////////////////////////
// Name:Init_D3D()
// Describe:Initialize D3D window
//////////////////////////////////////////////////////////////////////////
bool Init_D3D(D3DDEVTYPE deviceType)
{
	HWND hwnd;

	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInstance;
	wc.hIcon = (HICON)LoadImage(0, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(g_hInstance, NULL);
	wc.hIconSm = (HICON)LoadImage(0, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = WINDOW_TITLE;

	if (!RegisterClassEx(&wc))
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}

	hwnd = ::CreateWindowEx(NULL, wc.lpszClassName, wc.lpszClassName,
		WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
		0 /*parent hwnd*/, 0 /* menu */, g_hInstance, 0 /*extra*/);

	if (!hwnd)
	{
		::MessageBox(0, L"CreateWindow() - FAILED", 0, 0);
		return false;
	}
	g_hwnd = hwnd;

	::ShowWindow(hwnd, SW_SHOW);

	::UpdateWindow(hwnd);

	// disable CTRL-ALT_DEL, ALT_TAB, comment this line out 
	// if it causes your system to crash
	SystemParametersInfo(SPI_SCREENSAVERRUNNING, TRUE, NULL, 0);

	//
	// Init D3D: 
	//

	HRESULT hr = 0;

	// Step 1: Create the IDirect3D9 object.
	int mode;					//Set the full screen or windowed

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		::MessageBox(0, L"Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}

	// Step 2: Check for hardware vp.

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	mode = MessageBoxW(NULL, L"Use FullScreen mode?", L"Tips", MB_YESNO | MB_ICONQUESTION);

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

	D3DPRESENT_PARAMETERS d3dpp;

	if (mode == IDYES)
	{
		d3dpp.BackBufferWidth = WINDOW_WIDTH;
		d3dpp.BackBufferHeight = WINDOW_HEIGHT;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount = 2;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = FALSE;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.Flags = 0;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
	{
		d3dpp.BackBufferWidth = WINDOW_WIDTH;
		d3dpp.BackBufferHeight = WINDOW_HEIGHT;
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount = 2;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = TRUE;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.Flags = 0;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	// Step 4: Create the device.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		hwnd,               // window associated with device
		vp,                 // vertex processing
		&d3dpp,             // present parameters
		&g_Device);            // return created device

	if (FAILED(hr))
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			&g_Device);

		if (FAILED(hr))
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, L"CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	d3d9->Release(); // done with d3d9 object

	return true;
}

