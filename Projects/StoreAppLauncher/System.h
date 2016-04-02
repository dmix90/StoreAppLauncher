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
	int					m_iControllerMode;
	HWND				m_hWndTop;
	HWND				m_hWndBottom;
	HWND				m_hWndApp;
	HWND				m_hWndSteam;
private:
	bool GetAppId( );
	bool OpenAppById( );
	void FindAppWindowHandle( );
	HWND GetProcessWindow( DWORD );
public:
	void OpenConsole( );
	bool Init( );
	bool UseController( );
	bool StillRunning( );
	void Update( uint );
	void Shutdown( );
	int	 GetControllerMode( );
	void SwitchForegroundWindow( );
};