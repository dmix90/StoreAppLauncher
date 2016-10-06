#include "Application.h"

#pragma comment(lib, "runtimeobject.lib")
#pragma comment(lib, "tinyxml2.lib")

int APIENTRY wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
#ifdef _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	unique_ptr<Application> app( new Application( ) );

	return app->Run( );
}