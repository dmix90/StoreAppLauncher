#include "System.h"

System::System( )
	:m_uNumArgs( 0 ), m_ulAppStatus( STILL_ACTIVE ), m_ulProcessId( 0 ), m_bUseController( false ), m_bBootExplorer( false ), m_iControllerMode( 0 )
{
	m_wsArgs.clear( );
	m_updater.reset( new Updater( ) );
}

System::~System( )
{
}
void System::OpenConsole( )
{
	AllocConsole( );

	HANDLE handle_out = GetStdHandle( STD_OUTPUT_HANDLE );
	int hCrt = _open_osfhandle( (long)handle_out, _O_TEXT );
	FILE* hf_out = _fdopen( hCrt, "w" );
	setvbuf( hf_out, NULL, _IONBF, 1 );
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle( STD_INPUT_HANDLE );
	hCrt = _open_osfhandle( (long)handle_in, _O_TEXT );
	FILE* hf_in = _fdopen( hCrt, "r" );
	setvbuf( hf_in, NULL, _IONBF, 128 );
	*stdin = *hf_in;
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
		if( !wcscmp( args[ i ], L"bp" ) )
		{
			m_bUseController = true;
			m_iControllerMode = 0;
		}
		if( !wcscmp( args[ i ], L"bp1" ) )
		{
			m_bUseController = true;
			m_iControllerMode = 1;
		}
	}
//	if( m_wsArgs.size( ) > 1 )
//	{
//		return true;
//	}
//	else
//	{
//
//		if( MessageBox( 0, L"Launch parameter was not found. Do you want to update executables within this directory?", L"WARNING: Missing parameter/Update", MB_ICONINFORMATION | MB_OKCANCEL | MB_DEFBUTTON2 ) == IDOK )
//		{
//#ifndef _DEBUG
//			OpenConsole( );
//#endif
//			m_updater->Launch( );
//#ifndef _DEBUG
//			FreeConsole( );
//#endif
//		}
//	}
//	return false;
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
		MessageBox( 0, L"Please double-check Application ID", L"ERROR: Application ID is probably not correct", MB_ICONERROR | MB_OK );
		assert( SUCCEEDED( hr ) );
	}
	else
	{
		return true;
	}
	hr = aam->ActivateApplication( m_wsArgs[ 1 ].c_str( ), nullptr, AO_NONE, &m_ulProcessId );
	if( FAILED( hr ) )
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
		MessageBox( 0, L"Please double-check Application ID", L"ERROR: Application ID is probably not correct", MB_ICONERROR | MB_OK );
		assert( SUCCEEDED( hr ) );
	}
	return true;
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
	TerminateProcess( m_hProcess, 0 );
	CloseHandle( m_hProcess );
	CoUninitialize( );
}

int System::GetControllerMode( )
{
	return m_iControllerMode;
}
