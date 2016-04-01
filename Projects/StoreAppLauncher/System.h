#pragma once
#include "Common.h"
#include <atlbase.h>
#include <ShObjIdl.h>

class System
{
public:
	System( );
	~System( );
private:
	int					m_uNumArgs;
	vector<wstring>		m_wsArgs;
	ulong				m_ulProcessId;
	ulong				m_ulAppStatus;
	void*				m_hProcess;
	bool				m_bUseController;
private:
	bool GetAppId( );
	bool OpenAppById( );
public:
	bool Init( );
	bool UseController( );
	bool StillRunning( );
	void Update( uint );
	void Shutdown( );
};