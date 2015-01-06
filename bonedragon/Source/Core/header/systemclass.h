#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

//PRE-PROCESSING DIRECTIVES
#define WIN32_LEAN_AND_MEAN

//Includes
#include <windows.h>
//My class Includes
#include "inputclass.h"
#include "graphicsclass.h"
#include "fpsclass.h"
#include "cpuclass.h"
#include "timerclass.h"
#include "positionclass.h"

class SystemClass
{
	public:
		SystemClass();
		SystemClass(const SystemClass&);
		~SystemClass();

		bool Initialize();
		void Shutdown();
		void Run();

		LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	private:
		bool Frame();
		bool ProcessFrameInput();
		void InitializeWindows(int&, int&);
		void ShutdownWindows();
		void ToggleGravityMode(bool);

	private:
		LPCWSTR   m_applicationName;
		HINSTANCE m_hinstance;
		HWND	  m_hwnd;

		InputClass * m_Input;
		GraphicsClass * m_Graphics;

		FpsClass* m_Fps;
		CpuClass* m_Cpu;
		TimerClass* m_Timer;
		PositionClass * m_Position;
		PositionClass * m_SpcObj_Position;

		int m_GravityMode;
};

//Function Prototypes
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Globals
static SystemClass * ApplicationHandle = 0;

#endif