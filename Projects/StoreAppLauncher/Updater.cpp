#include "Updater.h"

Updater::Updater( )
{
	m_wsDir.resize( MAX_PATH, '\0' );
}
void Updater::GetCurrentDir( )
{
	GetModuleFileName( 0, &m_wsDir[ 0 ], (ulong)m_wsDir.length( ) );
	size_t index = m_wsDir.find_last_of( L"\\" );
	m_wsExe = m_wsDir.substr( index + 1 );

	PathRemoveFileSpec( &m_wsDir[ 0 ] );

	m_wsDir.resize( wcslen( &m_wsDir[ 0 ] ) );

	m_wsDir += L"\\*";
}
void Updater::GetCurrentDirFiles( )
{
	HANDLE hFind;
	WIN32_FIND_DATA data;

	hFind = FindFirstFile( m_wsDir.c_str( ), &data );
	if( !hFind )
	{
		MessageBox( 0, L"FindFirstFile failure", L"ERROR: Directory", MB_OK );
	}
	else
	{
		do
		{
			if( ( data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
			{
				wstring tempFile = data.cFileName;
				if( wcscmp( &tempFile[ 0 ], &m_wsExe[0] ) )
				{
					if( tempFile.substr( tempFile.find_last_of( L"." ) + 1 ) == L"exe" )
					{
						m_wsFileList.push_back( tempFile );
					}
				}	
			}
		} while( FindNextFile( hFind, &data ) != 0 );
	}
	FindClose( hFind );
}
bool Updater::ListReadyFiles( )
{
	if( m_wsFileList.size( ) > 0 )
	{
		wcout << "Files that will be replaced : " << endl << endl;
		for( uint i = 0; i < m_wsFileList.size( ); i++ )
		{
			wcout << m_wsFileList[ i ].c_str( ) << endl;
		}
		return true;
	}
	return false;
}
void Updater::ReplaceCurrentDirFiles( )
{
	for( uint i = 0; i < m_wsFileList.size( ); i++ )
	{
		CopyFile( m_wsExe.c_str( ), m_wsFileList[ i ].c_str( ), FALSE );
	}
}
void Updater::GenerateExecutables( )
{
	wstring outDir = L"_Applications\\";
	CreateDirectory( outDir.c_str( ), nullptr );
	wstring finPathAndName;
	for( uint i = 0; i < m_wsAppId.size( ); i++ )
	{
		m_wsAppId[ i ] += L".exe";
		finPathAndName = outDir + m_wsAppId[ i ];
		CopyFile( m_wsExe.c_str( ), finPathAndName.c_str( ) , FALSE );
	}
}
void Updater::GetAppIdVec( vector<wstring> vec )
{
	m_wsAppId = vec;
}
void Updater::Launch( )
{
	//GetCurrentDir( );
	//GetCurrentDirFiles( );
	//if( ListReadyFiles( ) )
	//{
	//	if( MessageBox( 0, L"Executable files which are currently in directory with launcher are going to be updated/replaced. Do you want to continue?", L"UPDATE: Confirmation", MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON1 ) == IDYES )
	//	{
	//		ReplaceCurrentDirFiles( );
	//	}
	//}
	//else
	//{
	//	GenerateExecutables( );
	//	_tsystem( _T( "explorer.exe ""_Apps"" " ) );
	//}
	GetCurrentDir( );
	GenerateExecutables( );
	_tsystem( _T( "explorer.exe ""_Applications"" " ) );
}
