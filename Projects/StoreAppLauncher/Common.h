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