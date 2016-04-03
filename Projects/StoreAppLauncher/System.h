#pragma once
#include "Updater.h"

class System
{
public:
	System( );
	~System( );
private:
	unique_ptr<Updater>	m_updater;
private:
	int					m_uNumArgs;
	vector<wstring>		m_wsArgs;
	ulong				m_ulProcessId;
	ulong				m_ulAppStatus;
	void*				m_hProcess;
	bool				m_bUseController;
	bool				m_bBootExplorer;
	int					m_iControllerMode;
	PROCESS_INFORMATION m_pInfo;
private:
	bool GetAppId( );
	bool OpenAppById( );
public:
	void OpenConsole( );
	bool Init( );
	bool UseController( );
	bool StillRunning( );
	void Update( uint );
	void Shutdown( );
	int	 GetControllerMode( );
};