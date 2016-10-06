#pragma once
#include "Common.h"
#include "resource.h"

static INT_PTR CALLBACK staticDialogProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

class Dialog
{
private:
	bool filter;
	function<void( bool )> GenerateExecutables;
public:
	void GetFunc( function<void( bool )> );
	void DialogProc( HWND&, UINT&, WPARAM&, LPARAM& );
	void Init( );
};