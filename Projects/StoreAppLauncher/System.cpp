#include "System.h"

System::System( )
	:m_uNumArgs( 0 ), m_ulAppStatus( STILL_ACTIVE ), m_ulProcessId( 0 ), m_bUseController( false ), m_bBootExplorer( false ), m_iControllerMode( 0 ),
	m_hProcess( nullptr ), m_hExplorer( nullptr ), m_bCheckParameterForId( true ), m_bUseDSR( false ), m_bLimitResolution( false )
{
	m_wsArgs.clear( );
	m_pUpdater.reset( new Updater( ) );
	m_pList.reset( new List( ) );
	m_pDialog.reset( new Dialog( ) );

	m_wsKnownArgs = { L"bigpicture0", L"-bigpicture", L"-downsampling", L"-maxres_" };

	m_dm.dmSize = sizeof( m_dm );
}
bool System::GetAppId( )
{
	GetExecutableName( );
	wchar_t** args = CommandLineToArgvW( GetCommandLine( ), &m_uNumArgs );
	for( uint i = 0; i < m_uNumArgs; i++ )
	{
		m_wsArgs.push_back( args[ i ] );
#ifdef _DEBUG
		wcout << i << ": " << m_wsArgs[ i ].c_str() << endl;
#endif
		if( !wcscmp( args[ i ], L"-bigpicture" ) )
		{
			m_bUseController = true;
			m_iControllerMode = 1;
		}
		if( !wcscmp( args[ i ], L"-bigpicture0" ) )
		{
			m_bUseController = true;
			m_iControllerMode = 0;
		}
		if( !wcscmp( args[ i ], L"-downsampling" ) )
		{
			m_bUseDSR = true;
		}
		if( m_bUseDSR && ( m_wsArgs[i].find( L"-maxres_" ) != wstring::npos ) )
		{
			m_bLimitResolution = true;
			wstring::size_type t = m_wsArgs[ i ].find( L"-maxres_" );
			if( t != wstring::npos )
			{
				m_wsArgs[ i ].erase( t, m_wsKnownArgs[ 3 ].size( ) );
				m_wsMaxRes = m_wsArgs[ i ];
			}			
		}
	}
	if( m_wsArgs.size( ) > 1 )
	{
		for( uint i = 0; i < m_wsKnownArgs.size( ); i++ )
		{
			if( !wcscmp( m_wsArgs[ 1 ].c_str( ), m_wsKnownArgs[ i ].c_str( ) ) )
			{
				m_bCheckParameterForId = false;
			}
		}
	}

	return true;
}

bool System::OpenAppById( )
{
	HRESULT hr = S_OK;
	CComPtr<IApplicationActivationManager> aam = nullptr;

	hr = CoInitializeEx( nullptr, COINIT_APARTMENTTHREADED );
	if( FAILED( hr ) )
	{
		for( uint i = 0; i < 5; i++ )
		{
			Sleep( 100 );
			hr = CoInitializeEx( nullptr, COINIT_APARTMENTTHREADED );
			if( SUCCEEDED( hr ) )
			{
				break;
			}
		}
		assert( SUCCEEDED( hr ) );
	}

	hr = CoCreateInstance( CLSID_ApplicationActivationManager, nullptr, CLSCTX_LOCAL_SERVER, IID_PPV_ARGS( &aam ) );
	if( FAILED( hr ) )
	{
		for( uint i = 0; i < 5; i++ )
		{
			Sleep( 100 );
			hr = CoCreateInstance( CLSID_ApplicationActivationManager, nullptr, CLSCTX_LOCAL_SERVER, IID_PPV_ARGS( &aam ) );
			if( SUCCEEDED( hr ) )
			{
				break;
			}
		}
		assert( SUCCEEDED( hr ) );
	}


	hr = CoAllowSetForegroundWindow( aam, nullptr );
	if( FAILED( hr ) )
	{
		for( uint i = 0; i < 5; i++ )
		{
			Sleep( 100 );
			hr = CoAllowSetForegroundWindow( aam, nullptr );
			if( SUCCEEDED( hr ) )
			{
				break;
			}
		}
		assert( SUCCEEDED( hr ) );
	}

	if( m_bUseDSR )
	{
		SwitchResolution( );
	}

	hr = aam->ActivateApplication( m_wsExeName.c_str( ) , nullptr, AO_NONE, &m_ulProcessId );
	if( FAILED( hr ) )
	{
		for( uint i = 0; i < 5; i++ )
		{
			Sleep( 100 );
			hr = aam->ActivateApplication( m_wsExeName.c_str( ), nullptr, AO_NONE, &m_ulProcessId );
			if( SUCCEEDED( hr ) )
			{
				return true;
			}
		}
		if( m_bCheckParameterForId && m_wsArgs.size( ) > 1 )
		{
			for( uint i = 0; i < 5; i++ )
			{
				Sleep( 100 );
				hr = aam->ActivateApplication( m_wsArgs[ 1 ].c_str( ), nullptr, AO_NONE, &m_ulProcessId );
				if( SUCCEEDED( hr ) )
				{
					return true;
				}
			}
		}
	}
	else
	{
		return true;
	}
	if( Tools::Instance( )->AreElevatedRightsAvailable( ) )
	{
		m_pDialog->GetFunc( bind( &System::GenerateExecutables, this, std::placeholders::_1 ) );
		m_pDialog->Init( );	
	}
	else
	{
		MessageBox( 0, L"To generate executables you should run this with admin rights: \nRight Click -> Run as Administrator",
					L"ERROR: Administrator rights required", MB_ICONERROR | MB_OK );
	}
	return false;
}

bool System::IsExplorerRunning( )
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof( PROCESSENTRY32 );

	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

	if( Process32First( snapshot, &entry ) )
	{
		while( Process32Next( snapshot, &entry ) )
		{
			if( !_wcsicmp( entry.szExeFile, L"explorer.exe" ) )
			{
				exists = true;
				break;
			}
		}
	}
	CloseHandle( snapshot );
	return exists;
}
void System::LaunchExplorer( )
{
	wstring buf( MAX_PATH, '\0' );
	GetWindowsDirectory( &buf[ 0 ], MAX_PATH );
	buf.resize( wcslen( &buf[ 0 ] ) );
	buf += L"\\explorer.exe";

	STARTUPINFO cif;
	ZeroMemory( &cif, sizeof( cif ) );

	if( !IsExplorerRunning( ) )
	{
		if( CreateProcess( buf.c_str( ), 0, 0, 0, FALSE, 0, 0, 0, &cif, &m_pInfo ) )
		{
			m_bBootExplorer = true;
		}
		else
		{
			MessageBox( 0, L"Explorer was not launched", L"ERROR: Unknown Error", MB_ICONERROR | MB_OK );
		}
	}
}

bool System::FindProcessWindow( HWND& hWnd, ulong pId )
{
	ulong pIdtemp = 0;
	for( HWND hWndTemp = GetTopWindow( 0 ); hWndTemp != 0; hWndTemp = GetNextWindow( hWndTemp, GW_HWNDNEXT ) )
	{
		GetWindowThreadProcessId( hWndTemp, &pIdtemp );
		if( pId == pIdtemp )
		{
			hWnd = hWndTemp;
			return true;
		}
	}
	return false;
}

void System::GetExecutableName( )
{
	wstring fileName( MAX_PATH, '\0' );
	GetModuleFileName( 0, &fileName[ 0 ], (ulong)fileName.length( ) );
	wstring out( MAX_PATH, '\0' );
	out = PathFindFileName( &fileName[ 0 ] );
	fileName.resize( wcslen( &fileName[ 0 ] ) );
	*( PathFindExtension( &out[ 0 ] ) ) = 0;
	out.resize( wcslen( &out[ 0 ] ) );
	wcout << out.c_str() << endl;
	m_wsExeName = out;
	wcout << m_wsExeName.c_str( ) << endl;
}

void System::SwitchResolution( bool restore )
{
	if( !restore )
	{
		EnumDisplaySettings( 0, ENUM_CURRENT_SETTINGS, &m_dm );
		ulong curResX = m_dm.dmPelsWidth;
		ulong curResY = m_dm.dmPelsHeight;

		DEVMODE dmTemp = m_dm;

		vector<ulong> tempResX;
		vector<ulong> tempResY;

		ulong maxResX = 0;
		ulong maxResY = 0;

		for( uint i = 0; EnumDisplaySettings( 0, i, &dmTemp ) != 0; i++ )
		{
			tempResX.push_back( dmTemp.dmPelsWidth );
			tempResY.push_back( dmTemp.dmPelsHeight );
		}

		if( !m_bLimitResolution )
		{
			maxResX = *std::max_element( tempResX.begin( ), tempResX.end( ) );
			maxResY = *std::max_element( tempResY.begin( ), tempResY.end( ) );
		}
		else
		{
			vector<uint> vecTemp;
			uint temp;
			std::wstringstream ss( m_wsMaxRes );
			while( ss >> temp )
			{
				vecTemp.push_back( temp );
				if( ss.peek( ) == L'x' )
				{
					ss.ignore( );
				}
			}

			maxResX = vecTemp[ 0 ];
			maxResY = vecTemp[ 1 ];
		}

		dmTemp.dmPelsWidth = maxResX;
		dmTemp.dmPelsHeight = maxResY;
		dmTemp.dmDisplayFrequency = m_dm.dmDisplayFrequency;

		if( ( curResX != maxResX ) && ( curResY != maxResY ) )
		{
			if( ChangeDisplaySettings( &dmTemp, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
			{
				SwitchResolution( true );

				Tools::Instance( )->MessageBoxTimeout( nullptr, L"Selected maximum resolution is not supported",
													   L"ERROR: Display resolution switch failure",
													   MB_ICONERROR, 0, 3000 );
			}
			Sleep( 2500 );
		}
	}
	else
	{
		DEVMODE dmTemp;
		EnumDisplaySettings( 0, ENUM_CURRENT_SETTINGS, &dmTemp );
		if( ( m_dm.dmPelsWidth != dmTemp.dmPelsWidth ) && ( m_dm.dmPelsHeight != dmTemp.dmPelsHeight ) )
		{
			if( ChangeDisplaySettings( &m_dm, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
			{
				Tools::Instance( )->MessageBoxTimeout( nullptr, L"Could not restore original display resolution",
													   L"ERROR: Display resolution switch failure",
													   MB_ICONERROR, 0, 3000 );
			}
			Sleep( 2500 );
		}
	}
}

void System::GenerateExecutables( bool filter )
{
	m_pList->Launch( filter );
	m_pUpdater->GetAppIdVec( m_pList->FinalIdVec( ) );
	m_pUpdater->Launch( );
}

bool System::Init( )
{
	if( GetAppId( ) )
	{
		LaunchExplorer( );

		if( OpenAppById( ) )
		{
			m_hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, m_ulProcessId );
			return true;
		}
	}
	return false;
}

bool System::UseController( )
{
	return m_bUseController;
}

bool System::StillRunning( )
{
	if( m_ulAppStatus == STILL_ACTIVE )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void System::Update( uint timeout )
{
	while( m_ulAppStatus == STILL_ACTIVE )
	{
		GetExitCodeProcess( m_hProcess, &m_ulAppStatus );
		if( timeout > 0 )
		{
			Sleep( timeout );
		}
	}
}

void System::Shutdown( )
{
	if( m_bBootExplorer )
	{
		_tsystem( _T( "taskkill /F /T /IM explorer.exe" ) );
		TerminateProcess( m_hExplorer, 0 );
		CloseHandle( m_hExplorer );
	}
	if( m_hProcess != nullptr )
	{
		TerminateProcess( m_hProcess, 0 );
		CloseHandle( m_hProcess );
	}
	if( m_bUseDSR )
	{
		SwitchResolution( true );
	}
	CoUninitialize( );
}

int System::GetControllerMode( )
{
	return m_iControllerMode;
}
