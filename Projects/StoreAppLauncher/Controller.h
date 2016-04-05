#pragma once

#include "Common.h"
#include <Xinput.h>

#define XINPUT_GAMEPAD_GUIDE 0x400

struct Button
{
	bool Pressed;
	bool Held;
	bool Released;
	uint Id;
	uint HeldFor;
	uint WasHeldFor;
	uint WasPressed;
};
struct Deadzone
{
	float X;
	float Y;
};
struct ThumbStick
{
	float X;
	float Y;
};
struct Trigger
{
	byte Value;
};
class Controller
{
public:
	Controller( float x = 0.05f, float y = 0.02f );
	~Controller( );
private:
	bool			m_Status;
	int				m_Id;
	uint			m_PoolRate;
	XINPUT_STATE	m_State;
	XINPUT_STATE	m_PrevState;
	Deadzone		m_Deadzone;
	HINSTANCE		m_XInputLib;
private:
	bool	GetXInputLibrary( HINSTANCE& );
	bool	Init( float, float );
	void	ButtonStateUpdate( );
public:
	vector<Button>	m_Buttons;
	ThumbStick		m_LeftStick;
	ThumbStick		m_RightStick;
	Trigger			m_LeftTrigger;
	Trigger			m_RightTrigger;
public:
	int		GetPort( ); 
	XINPUT_GAMEPAD* GetState( );
	bool	CheckConnection( );
	bool	Update( uint );
	bool	IsPressed( uint );
	bool	WasPressed( uint );
	bool	IsHeldFor( uint, uint );
	void	Shutdown( );
};