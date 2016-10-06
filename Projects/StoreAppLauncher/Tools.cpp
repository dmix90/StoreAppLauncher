#include "Tools.h"

unique_ptr<Tools> Tools::m_pInstance = nullptr;

typedef int( __stdcall *MSGBOXWAPI )( IN HWND hWnd,
									  IN LPCWSTR lpText, IN LPCWSTR lpCaption,
									  IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds );

Tools::Tools( )
	:m_bConsoleActive( false )
{

}
Tools* Tools::Instance( )
{
	if( !m_pInstance )
	{
		m_pInstance.reset( new Tools( ) );
	}
	return m_pInstance.get( );
}
void Tools::OpenConsole( )
{
	if( !m_bConsoleActive )
	{
		AllocConsole( );

		void* handle_out = GetStdHandle( STD_OUTPUT_HANDLE );
		int hCrt = _open_osfhandle( reinterpret_cast<int64_t>( handle_out ), _O_TEXT );
		FILE* hf_out = _fdopen( hCrt, "w" );
		setvbuf( hf_out, NULL, _IONBF, 1 );
		*stdout = *hf_out;

		void* handle_in = GetStdHandle( STD_INPUT_HANDLE );
		hCrt = _open_osfhandle( reinterpret_cast<int64_t>( handle_in ), _O_TEXT );
		FILE* hf_in = _fdopen( hCrt, "r" );
		setvbuf( hf_in, NULL, _IONBF, 128 );
		*stdin = *hf_in;

		m_bConsoleActive = true;
	}
}

void Tools::CloseConsole( )
{
	if( m_bConsoleActive )
	{
		FreeConsole( );
	}
}

wstring Tools::ByteToWideByte( string input )
{
	std::wstring_convert < std::codecvt_utf8<wchar_t> > converter;
	wstring output = converter.from_bytes( input );
	return output;
}

int Tools::MessageBoxTimeout( HWND hWnd, LPCWSTR message, LPCWSTR title, UINT type, WORD langId, DWORD ms )
{
	HMODULE hUser32 = LoadLibrary( _T( "user32.dll" ) );
	static MSGBOXWAPI MsgBoxTOW = NULL;

	if( !MsgBoxTOW )
	{
		HMODULE hUser32t = GetModuleHandle( _T( "user32.dll" ) );
		if( hUser32t )
		{
			MsgBoxTOW = (MSGBOXWAPI)GetProcAddress( hUser32t,
													"MessageBoxTimeoutW" );
		}
		else
		{
			FreeLibrary( hUser32 );
			return 0;
		}
	}

	if( MsgBoxTOW )
	{
		return MsgBoxTOW( hWnd, message, title,
						  type, langId, ms );
	}
	FreeLibrary( hUser32 );
	return 0;
}

BOOL Tools::AreElevatedRightsAvailable( )
{
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if( OpenProcessToken( GetCurrentProcess( ), TOKEN_QUERY, &hToken ) )
	{
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof( TOKEN_ELEVATION );
		if( GetTokenInformation( hToken, TokenElevation, &Elevation, sizeof( Elevation ), &cbSize ) )
		{
			fRet = Elevation.TokenIsElevated;
		}
	}
	if( hToken )
	{
		CloseHandle( hToken );
	}
	return fRet;
}

string Tools::WideByteToByte( wstring input )
{
	std::wstring_convert < std::codecvt_utf8<wchar_t> > converter;
	string output = converter.to_bytes( input );
	return output;
}
