// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


// TODO: reference additional headers your program requires here

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include "message.h"
#include <ws2tcpip.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fstream> 
#include <filesystem>
#include <vector>
#include <algorithm>
#include <cstdio>
#include "function.h"
using namespace std;
namespace fs = std::filesystem;



