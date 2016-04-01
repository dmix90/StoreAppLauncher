#include "Application.h"

Application::Application( )
{
	system.reset( new System( ) );
	controller.reset( new Controller( ) );
	keyboard.reset( new Keyboard( ) );
	updater.reset( new Updater( ) );
}
Application::~Application( )
{
}
void Application::OpenConsole( )
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
int Application::Run( )
{
#ifdef _DEBUG
	OpenConsole( );
#endif
	updater->Launch( );
	if( system->Init( ) )
	{
		if( system->UseController( ) )
		{	
			thread t1( &System::Update, system.get( ), 1500 );
			t1.detach( );
			while( system->StillRunning( ) )
			{
				controller->Update( 100 );
				if( controller->IsPressed( XINPUT_GAMEPAD_GUIDE ) )
				{
					Beep( 250, 400 );
					keyboard->VirtualAltTab( );
					Sleep( 200 );
				}
			}
		}
		else
		{
			system->Update( 1500 );
		}
		keyboard->VirtualAltTab( );
		controller->Shutdown( );
		system->Shutdown( );	
	}
#ifdef _DEBUG
	FreeConsole( );
#endif
	return 0;
}
