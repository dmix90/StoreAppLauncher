#pragma once
#include "System.h"
#include "Controller.h"
#include "Keyboard.h"

class Application
{
private:
	unique_ptr<System>		system;
	unique_ptr<Controller>	controller;
	unique_ptr<Keyboard>	keyboard;
private:
	wstring m_wsUniqueMutex;
	HANDLE	m_hAppHandle;
public:
	Application( );
	~Application( );
	int Run( );
};