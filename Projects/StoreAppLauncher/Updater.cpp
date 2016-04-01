#include "Updater.h"

Updater::Updater( )
{
}

Updater::~Updater( )
{
}

void Updater::GetCurrentDirectoryFiles( )
{
	wchar_t path[ MAX_PATH ];
	GetModuleFileName( 0, path, MAX_PATH );

	PathRemoveFileSpec( path );
	wstring dir = path;
	dir += L"\\*";

	HANDLE hFind;
	WIN32_FIND_DATA data;
	vector<wstring> allFiles;

	hFind = FindFirstFile( dir.c_str( ), &data );
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
				allFiles.push_back( data.cFileName );
			}
		} while( FindNextFile( hFind, &data ) != 0 );
	}
	for( uint i = 0; i < allFiles.size( ); i++ )
	{
		if( allFiles[ i ].substr( allFiles[ i ].find_last_of( L"." ) + 1 ) == L"exe" )
		{
			m_wsFileList.push_back( allFiles[ i ] );
		}
	}
}

void Updater::Launch( )
{
	GetCurrentDirectoryFiles( );
	system( "pause" );
}
