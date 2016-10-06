#include "Dialog.h"

void Dialog::GetFunc( function<void( bool )>f )
{
	GenerateExecutables = f;
}

void Dialog::DialogProc( HWND& hWnd, UINT& msg, WPARAM& wParam, LPARAM& lParam )
{
	switch( msg )
	{
	case WM_COMMAND:
		switch( LOWORD( wParam ) )
		{
		case IDGEN:
			GenerateExecutables( filter );
			EndDialog( hWnd, IDOK );
			break;
		case IDEXIT:
			EndDialog( hWnd, IDOK );
			break;
		}
	}
	if( IsDlgButtonChecked( hWnd, IDCHECKFILTER ) == BST_CHECKED )
	{
		filter = true;
	}
	else
	{
		filter = false;
	}
}
void Dialog::Init( )
{
	DialogBoxParam( GetModuleHandle( 0 ), (LPCTSTR)IDD_DIALOG1, 0, staticDialogProc, ( LPARAM ) this );
}
INT_PTR CALLBACK staticDialogProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_INITDIALOG:
		SetProp( hWnd, L"Companion", (HANDLE)lParam );
		CheckRadioButton( hWnd, IDCHECKFILTER, IDCHECKFILTER, IDCHECKFILTER );
		break;
	case WM_NCDESTROY:
		RemoveProp( hWnd, L"Companion" );
		break;
	}

	Dialog* tmp = (Dialog*)GetProp( hWnd, L"Companion" );

	if( tmp )
	{
		tmp->DialogProc( hWnd, msg, wParam, lParam );
	}
	return FALSE;
}