// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"


#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>


extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

#include <d3d9.h>
#include <d3dx9.h>

#include <list>
#include <vector>
#include <map>
#include <string>

#include <algorithm>

#include <process.h>

#include <fstream>

#ifdef _DEBUG

//#ifdef UNICODE                                                                                      
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
//#else                                                                                                    
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;
