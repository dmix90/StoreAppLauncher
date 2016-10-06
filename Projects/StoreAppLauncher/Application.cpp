#include "Application.h"

Application::Application( )
	: m_wsUniqueMutex( L"StoreAppLauncher" ), m_hAppHandle( nullptr )
{
	system.reset( new System( ) );
	controller.reset( new Controller( ) );
	keyboard.reset( new Keyboard( ) );
}
bool Application::Run( )
{
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
				controller->Update( 150 );
				switch( system->GetControllerMode( ) )
				{
				case 0:
				{
					if( controller->IsPressed( XINPUT_GAMEPAD_BACK ) &&
						controller->IsPressed( XINPUT_GAMEPAD_START ) &&
						controller->m_LeftTrigger.Value > 0 &&
						controller->m_RightTrigger.Value > 0 )
					{
						Beep( 250, 400 );
						keyboard->VirtualAltF4( );
						Sleep( 200 );
						break;
					}
					if( controller->IsPressed( XINPUT_GAMEPAD_BACK ) &&
						controller->IsPressed( XINPUT_GAMEPAD_START ) &&
						controller->m_LeftTrigger.Value > 0 )
					{
						Beep( 250, 400 );
						keyboard->VirtualAltTab( );
						Sleep( 200 );
						break;
					}
					break;
				}
				case 1:
				{
					if( controller->IsPressed( XINPUT_GAMEPAD_GUIDE ) &&
						controller->IsPressed( XINPUT_GAMEPAD_BACK ) )
					{
						Beep( 250, 400 );
						keyboard->VirtualAltF4( );
						Sleep( 200 );
						break;
					}
					if( controller->IsPressed( XINPUT_GAMEPAD_GUIDE ) )
					{
						Beep( 250, 400 );
						keyboard->VirtualAltTab( );
						Sleep( 200 );
						break;
					}
					if( controller->IsPressed( XINPUT_GAMEPAD_BACK ) &&
						controller->IsPressed( XINPUT_GAMEPAD_RIGHT_THUMB ) )
					{
						Beep( 250, 400 );
						keyboard->VirtualGameDVR( );
						Sleep( 200 );
						break;
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
	}
	controller->Shutdown( );
	system->Shutdown( );

	ReleaseMutex( m_hAppHandle );
	CloseHandle( m_hAppHandle );
	return 0;
}
