#pragma once
#include "Common.h"
#include <atlbase.h>
#include <ShObjIdl.h>
#include <strsafe.h>
#include "Shlwapi.h"

class Updater
{
public:
	Updater( );
	~Updater( );
private:
	vector<wstring> m_wsFileList;
	wstring m_wsExe;
	wstring m_wsDir;
private:
	void GetCurrentDir( );
	void GetCurrentDirFiles( );
	void ReplaceCurrentDirFiles( );
public:
	void Launch( );
};