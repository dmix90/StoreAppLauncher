#pragma once

#include <Windows.h>
#include <commctrl.h>

#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <cassert>
#include <future>
#include <mutex>
#include <sstream>
#include <tchar.h>
#include <functional>

#include <map>
#include <list>

#include <algorithm>

#include <cctype>

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <tlhelp32.h>
#include <fstream>

#include <stdlib.h>
#include <crtdbg.h>

#include <windows.foundation.h>
#include <Windows.ApplicationModel.h>
#include <windows.management.deployment.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>
#include <wrl\implements.h>
#include <comutil.h>

#include <codecvt>

#include <tinyxml2.h>

using std::wcout;
using std::endl;
using std::string;
using std::wstring;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::thread;
using std::async;
using std::future;
using std::mutex;
using std::map;
using std::function;
using std::bind;

typedef unsigned long	ulong;
typedef unsigned short	uint;

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Management;
using namespace ABI::Windows::Management::Deployment;
using namespace ABI::Windows::ApplicationModel;
using namespace ABI::Windows::Storage;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

using namespace tinyxml2;

#define XMLCheckResult(x) if (x != XML_SUCCESS) { wcout << L"xmlError: " << x << endl; }
#define THROW_ON_ERR( expr ) { HRESULT hr = expr;  if( FAILED( hr ) ) { throw; }; }