// Filename ; inputclass.h
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

// Defines
#define DIRECTINPUT_VERSION 0x0800

#define INPUT_BORDER_RIGH 10

//Linking 
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// Includes
#include <dinput.h>

class InputClass
{
	public:
		InputClass();
		InputClass(const InputClass&);
		~InputClass();

		bool Initialize(HINSTANCE, HWND, int, int);
		void Shutdown();
		bool Frame();

		bool IsEscapePressed();
		// For camera movement
		bool IsLeftArrowPressed();
		bool IsRightArrowPressed();
		bool IsUpArrowPressed();
		bool IsDownArrowPressed();
		bool IsPeriodPressed();
		bool IsCommaPressed();
		bool IsAPressed();
		bool IsSPressed();
		bool IsWPressed();
		bool IsDPressed();
		bool IsFPressed();
		bool IsRPressed();

		// For Special Object movement
		bool IsJPressed();
		bool IsLPressed();
		bool IsKPressed();
		bool IsIPressed();
		bool IsYPressed();
		bool IsHPressed();

		// Options Toggling
		bool IsSpacePressed();
		bool IsSpaceReleased();
		bool Is1Pressed();
		bool Is2Pressed();
		bool Is3Pressed();

		void GetMouseLocation(int&, int&);

	private:
		bool ReadKeyboard();
		bool ReadMouse();
		void ProcessInput();

	private:
		IDirectInput8* m_directInput;
		IDirectInputDevice8* m_keyboard;
		IDirectInputDevice8* m_mouse;

		unsigned char m_keyboardState[256];
		DIMOUSESTATE m_mouseState;

		int m_screenWidth, m_screenHeight;
		int m_mouseX, m_mouseY;
		bool m_spaceDown;
};


#endif