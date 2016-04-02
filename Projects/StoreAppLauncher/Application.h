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
public:
	Application( );
	~Application( );
	void OpenConsole( );
	int Run( );
};