#include "Keyboard.h"

void Keyboard::VirtualAltTab( )
{
	keybd_event( VK_MENU, 0, 0, 0 );
	keybd_event( VK_TAB, 0, 0, 0 );
	keybd_event( VK_TAB, 0, KEYEVENTF_KEYUP, 0 );
	keybd_event( VK_MENU, 0, KEYEVENTF_KEYUP, 0 );
}

void Keyboard::VirtualAltF4( )
{
	keybd_event( VK_MENU, 0, 0, 0 );
	keybd_event( VK_F4, 0, 0, 0 );
	keybd_event( VK_F4, 0, KEYEVENTF_KEYUP, 0 );
	keybd_event( VK_MENU, 0, KEYEVENTF_KEYUP, 0 );
}

void Keyboard::VirtualEnter( )
{
	keybd_event( VK_EXECUTE, 0, 0, 0 );
	keybd_event( VK_EXECUTE, 0, KEYEVENTF_KEYUP, 0 );
}

void Keyboard::VirtualGameDVR( )
{
	keybd_event( VK_LWIN, 0, 0, 0 );
	keybd_event( 0x47, 0, 0, 0 );
	keybd_event( 0x47, 0, KEYEVENTF_KEYUP, 0 );
	keybd_event( VK_LWIN, 0, KEYEVENTF_KEYUP, 0 );
}
