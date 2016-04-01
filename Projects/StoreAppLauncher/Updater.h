#pragma once
#include "Common.h"
#include <strsafe.h>
#include "Shlwapi.h"

class Updater
{
private:
	void GetCurrentDirectoryFiles( );
	vector<wstring> m_wsFileList;
public:
	Updater( );
	~Updater( );
public:
	void Launch( );
};