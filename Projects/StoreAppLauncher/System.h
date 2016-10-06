#pragma once
#include "Updater.h"
#include "List.h"
#include "Dialog.h"

class System
{
public:
	System( );
	~System( ) { };
private:
	unique_ptr<Updater>	m_pUpdater;
	unique_ptr<List>	m_pList;
	unique_ptr<Dialog>	m_pDialog;
private:
	int					m_uNumArgs;
	vector<wstring>		m_wsArgs;
	vector<wstring>		m_wsKnownArgs;
	vector<wstring>		m_wsAppsList;
	ulong				m_ulProcessId;
	ulong				m_ulAppStatus;
	void*				m_hProcess;
	void*				m_hExplorer;
	bool				m_bUseController;
	bool				m_bBootExplorer;
	bool				m_bUseDSR;
	bool				m_bLimitResolution;
	bool				m_bCheckParameterForId;
	int					m_iControllerMode;
	PROCESS_INFORMATION m_pInfo;
	wstring				m_wsExeName;
	wstring				m_wsMaxRes;
	DEVMODE				m_dm;
private:
	bool GetAppId( );
	bool OpenAppById( );
	bool IsExplorerRunning( );
	void LaunchExplorer( );
	bool FindProcessWindow( HWND&, ulong );
	void GetExecutableName( );
	void SwitchResolution( bool restore = false );
	void GenerateExecutables( bool filter );
public:
	bool Init( );
	bool UseController( );
	bool StillRunning( );
	void Update( uint );
	void Shutdown( );
	int	 GetControllerMode( );
};