#include "Application.h"

int APIENTRY wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
	unique_ptr<Application> app( new Application( ) );

	return app->Run( );
}