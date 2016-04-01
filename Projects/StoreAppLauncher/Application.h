#pragma once
#include "System.h"
#include "Controller.h"
#include "Keyboard.h"
//#include "Updater.h"

class Application
{
private:
	unique_ptr<System>		system;
	unique_ptr<Controller>	controller;
	unique_ptr<Keyboard>	keyboard;
	//unique_ptr<Updater>		updater;
public:
	Application( );
	~Application( );
	void OpenConsole( );
	int Run( );
};