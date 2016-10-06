#pragma once
#include "Common.h"

class Tools
{
private:
	static unique_ptr<Tools> m_pInstance;
	Tools( const Tools& ) { };
	Tools& operator=( Tools& ) { };
public:
	Tools( );
	~Tools( ) { };
	static Tools* Instance( );
private:
	bool m_bConsoleActive;
public:
	void OpenConsole( );
	void CloseConsole( );
	bool IsConsoleActive( ) { return m_bConsoleActive; };
	string WideByteToByte( wstring );
	wstring ByteToWideByte( string );
	int MessageBoxTimeout( HWND, LPCWSTR, LPCWSTR, UINT, WORD, DWORD );
	BOOL AreElevatedRightsAvailable( );
};