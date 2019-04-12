#pragma once
#ifndef GAME_H_
#define GAME_H_

#include <limits>
#include <crtdbg.h>
#include "Utility.h"
#include "D3DInput.h"

//////////////////////////////////////////////////////////////////////////
//Main game and input functions
//////////////////////////////////////////////////////////////////////////
bool Game_Init();
void Game_Main(float fTimeDelta);
void Game_Shutdown();
bool Init_D3D(D3DDEVTYPE deviceType);
void D3D_Update(float fTimeDelta);
void D3D_Render(float fTimeDelta);

bool Init_inputSystem();
void Update_Input();


//////////////////////////////////////////////////////////////////////////
//Callback functions
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


///////////////////////////////////////////////////////////////////////////
//Global variables
//////////////////////////////////////////////////////////////////////////
extern IDirect3DDevice9			*g_Device;

extern HINSTANCE				 g_hInstance;

extern HWND						 g_hwnd;

extern	D3DXMATRIX				g_matWorld;

extern CInputInterface			*g_DInput;				//InputSystem

extern bool						g_LMBDown;				//Mouse's left

extern int						g_MouseX, g_MouseY;
#endif