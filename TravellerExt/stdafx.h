// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef STRICT
#define STRICT
#endif

// 如果必须将位于下面指定平台之前的平台作为目标，请修改下列定义。
// 有关不同平台对应值的最新信息，请参考 MSDN。
#ifndef WINVER                // 允许使用特定于 Windows XP 或更高版本的功能。
#define WINVER 0x0501         // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT          // 允许使用特定于 Windows XP 或更高版本的功能。
#define _WIN32_WINNT 0x0501   // 将此值更改为相应的值，以适用于 Windows 的其他版本。
#endif                        

#ifndef _WIN32_WINDOWS        // 允许使用特定于 Windows 98 或更高版本的功能。
#define _WIN32_WINDOWS 0x0410 // 将此值更改为适当的值，以指定将 Windows Me 或更高版本作为目标。
#endif

#ifndef _WIN32_IE             // 允许使用特定于 IE 6.0 或更高版本的功能。
#define _WIN32_IE 0x0600      // 将此值更改为相应的值，以适用于 IE 的其他版本。
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS    // 某些 CString 构造函数将是显式的

#define _ATL_NO_UUIDOF

#include <comsvcs.h>

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlstdthunk.h>
#include <atlapp.h>
#include <atlmisc.h>

using namespace ATL;