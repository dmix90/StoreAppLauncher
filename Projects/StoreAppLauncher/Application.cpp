#include "Application.h"

Application::Application( )
	:m_wsUniqueMutex( L"StoreAppLauncher" ), m_hAppHandle( nullptr )
{
	system.reset( new System( ) );
	controller.reset( new Controller( ) );
	keyboard.reset( new Keyboard( ) );
}
Application::~Application( )
{
}
int Application::Run( )
{
#ifdef _DEBUG
	system->OpenConsole( );
#endif
	m_hAppHandle = CreateMutex( 0, TRUE, m_wsUniqueMutex.c_str( ) );
	if( GetLastError( ) == ERROR_ALREADY_EXISTS )
	{
		return 0;
	}

	if( system->Init( ) )
	{
		if( system->UseController( ) )
		{	
			thread t1( &System::Update, system.get( ), 1500 );
			t1.detach( );
			while( system->StillRunning( ) )
			{
				controller->Update( 100 );
				switch( system->GetControllerMode( ) )
				{
				case 0:
				{
					if( controller->IsPressed( XINPUT_GAMEPAD_BACK ) && controller->IsPressed( XINPUT_GAMEPAD_START ) && controller->m_LeftTrigger.Value > 0 )
					{
						Beep( 250, 400 );
						keyboard->VirtualAltTab( );
						Sleep( 200 );
					}
					break;
				}		
				case 1:
				{	
					if( controller->IsPressed( XINPUT_GAMEPAD_GUIDE ) )
					{
						Beep( 250, 400 );
						keyboard->VirtualAltTab( );
						Sleep( 200 );
					}
					break;
				}
				default:
					break;
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

		ReleaseMutex( m_hAppHandle );
		CloseHandle( m_hAppHandle );
	}
#ifdef _DEBUG
	FreeConsole( );
#endif
	return 0;
}
