#pragma once
#include "Tools.h"
#include <atlbase.h>
#include <ShObjIdl.h>
#include <strsafe.h>
#include "Shlwapi.h"
#include <ctype.h>

class Updater
{
public:
	Updater( );
	~Updater( ) { };
private:
	vector<wstring> m_wsFileList;
	vector<wstring> m_wsAppId;
	wstring m_wsExe;
	wstring m_wsDir;
private:
	void GetCurrentDir( );
	void GetCurrentDirFiles( );
	bool ListReadyFiles( );
	void ReplaceCurrentDirFiles( );
	void GenerateExecutables( );
public:
	void GetAppIdVec( vector<wstring> );
	void Launch( );
};