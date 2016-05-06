#pragma once

#include <Windows.h>

#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <cassert>
#include <future>
#include <mutex>

#include <algorithm>

#include <cctype>

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <tlhelp32.h>

#include <stdlib.h>
#include <crtdbg.h>

#include <windows.foundation.h>
#include <Windows.ApplicationModel.h>
#include <windows.management.deployment.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>
#include <wrl\implements.h>
#include <comutil.h>

using std::wcout;
using std::endl;
using std::string;
using std::wstring;
using std::vector;
using std::unique_ptr;
using std::thread;
using std::async;
using std::future;
using std::mutex;

typedef unsigned long	ulong;
typedef unsigned short	uint;


using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Management;
using namespace ABI::Windows::Management::Deployment;
using namespace ABI::Windows::ApplicationModel;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;