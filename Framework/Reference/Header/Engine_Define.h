#ifndef Engine_Define_h__
#define Engine_Define_h__


#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <queue>
#include <ctime>

#include <Ole2.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <Vfw.h>
#pragma comment (lib, "Vfw32.lib")
#include <io.h>





#include "Engine_Enum.h"
#include "Engine_EnumString.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"


#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#pragma warning(disable : 4251)

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
// #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
// #define new DBG_NEW
#endif

using namespace std;
using namespace Engine;
#endif
#endif



