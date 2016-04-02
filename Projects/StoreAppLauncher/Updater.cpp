#include "Updater.h"

Updater::Updater( )
{
	m_wsDir.resize( MAX_PATH, '\0' );
}

Updater::~Updater( )
{
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

		FindClose( hFind );
	}
}
void Updater::ListReadyFiles( )
{
	wcout << "Files that will be replaced : " << endl << endl;
	for( uint i = 0; i < m_wsFileList.size( ); i++ )
	{
		wcout << m_wsFileList[ i ].c_str( ) << endl;
	}
}
void Updater::ReplaceCurrentDirFiles( )
{
	for( uint i = 0; i < m_wsFileList.size( ); i++ )
	{
		CopyFile( m_wsExe.c_str( ), m_wsFileList[ i ].c_str( ), FALSE );
	}
}
void Updater::Launch( )
{
	wstring answer( 10, '\0' );
	GetCurrentDir( );
	GetCurrentDirFiles( );
	ListReadyFiles( );
	_tprintf( L"\nListed files are going to be updated/replaced with %s \nDo you want to continue? Y\\N \n>", m_wsExe.c_str( ) );
	std::wcin >> &answer[0];
	_wcslwr_s( &answer[0], answer.length() );
	answer.resize( wcslen( &answer[ 0 ] ) );
	if( !wcscmp( &answer[ 0 ], L"y" ) || !wcscmp( &answer[ 0 ], L"yes" ) )
	{
		ReplaceCurrentDirFiles( );
	}
}
