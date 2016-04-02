#include "System.h"

System::System( )
	:m_uNumArgs( 0 ), m_ulAppStatus( STILL_ACTIVE ), m_ulProcessId( 0 ), m_bUseController( false ), m_iControllerMode( 0 )
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
	wchar_t** args = CommandLineToArgvW( GetCommandLine( ), &m_uNumArgs );

	for( uint i = 0; i < m_uNumArgs; i++ )
	{
		m_wsArgs.push_back( args[ i ] );
#ifdef _DEBUG
		wcout << i << ": " << m_wsArgs[ i ].c_str() << endl;
#endif
		if( i == 2 )
		{
			if( !wcscmp( args[ 2 ], L"bp" ) )
			{
				m_bUseController = true;
				m_iControllerMode = 0;
			}
			if( !wcscmp( args[ 2 ], L"bp1" ) )
			{
				m_bUseController = true;
				m_iControllerMode = 1;
			}
		}
	}
	if( m_wsArgs.size( ) > 1 )
	{
		return true;
	}
	else
	{
		switch( MessageBox( 0, L"Launch parameter was not found. Do you want to update executables within this directory?", L"WARNING: Missing parameter/Update", MB_ICONINFORMATION | MB_OKCANCEL | MB_DEFBUTTON2 ) )
		{
		case IDOK:
		{
#ifndef _DEBUG
	OpenConsole( );
#endif
			m_updater->Launch( );
#ifndef _DEBUG
	FreeConsole( );
#endif
			break;
		}
		case IDCANCEL:
		{
			break;
		}		
		default:
			break;
		}
	}
	return false;
}

bool System::OpenAppById( )
{
	HRESULT hr = S_OK;

	hr = CoInitializeEx( nullptr, COINIT_APARTMENTTHREADED );
	assert( SUCCEEDED( hr ) );

	CComPtr<IApplicationActivationManager> aam = nullptr;
	hr = CoCreateInstance( CLSID_ApplicationActivationManager, nullptr, CLSCTX_LOCAL_SERVER, IID_PPV_ARGS( &aam ) );
	assert( SUCCEEDED( hr ) );

	hr = CoAllowSetForegroundWindow( aam, nullptr );
	assert( SUCCEEDED( hr ) );

	hr = aam->ActivateApplication( m_wsArgs[1].c_str(), nullptr, AO_NONE, &m_ulProcessId );
	if( SUCCEEDED( hr ) )
	{
		return true;
	}
	else
	{
		MessageBox( 0, L"Please double-check Application ID", L"ERROR: Application ID is probably not correct", MB_ICONERROR | MB_OK );
	}
	return false;
}
HWND System::GetProcessWindow( DWORD pId )
{
	bool bFound = false;
	HWND prevWindow = 0;

	while( !bFound )
	{
		HWND desktopWindow = GetDesktopWindow( );
		if( !desktopWindow )
			break;

		HWND nextWindow = FindWindowEx( desktopWindow, prevWindow, NULL, NULL );
		if( !nextWindow )
			break;

		// Check whether window belongs to the correct process.
		DWORD procId = -1;
		GetWindowThreadProcessId( nextWindow, &procId );

		if( procId == pId )
		{
			// Add additional checks. In my case, I had to bring the window to front so these checks were necessary.
			wchar_t windowText[ 1024 ];
			if( IsWindowVisible( nextWindow ) && !IsIconic( nextWindow ) && GetWindowText( nextWindow, (LPWSTR)windowText, sizeof( windowText ) / sizeof( wchar_t ) )
				&& !GetParent( nextWindow ) )
				return nextWindow;
		}

		prevWindow = nextWindow;
	}

	return 0;
}
void System::FindAppWindowHandle( )
{
	HWND hWnd = GetProcessWindow( m_ulProcessId );
}

bool System::Init( )
{
	bool status = false;
	if( status = GetAppId( ) )
	{
		if( status = OpenAppById( ) )
		{
			m_hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, m_ulProcessId );
			FindAppWindowHandle( );
		}
	}
	return status;
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
	TerminateProcess( m_hProcess, 0 );
	CloseHandle( m_hProcess );
	CoUninitialize( );
}

int System::GetControllerMode( )
{
	return m_iControllerMode;
}

void System::SwitchForegroundWindow( )
{
	m_hWndTop = GetForegroundWindow( );
	HWND hWnd = GetProcessWindow( m_ulProcessId );
	wstring className( MAX_PATH, '\0' );
	wstring windowText( MAX_PATH, '\0' );
	GetClassName( m_hWndTop, &className[ 0 ], MAX_PATH );
	GetWindowText( m_hWndTop, &windowText[ 0 ], MAX_PATH );
	wcout << "ForegrounWindow Class: " << className.c_str( ) << endl;
	wcout << "ForegrounWindow Text: " << windowText.c_str( ) << endl;

	GetClassName( hWnd, &className[ 0 ], MAX_PATH );
	GetWindowText( hWnd, &windowText[ 0 ], MAX_PATH );
	wcout << "ProcWindow Class: " << className.c_str( ) << endl;
	wcout << "ProcWindow Text: " << windowText.c_str( ) << endl;
}
