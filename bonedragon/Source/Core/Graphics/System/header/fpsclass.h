// Filename : fpsclass.h
#ifndef _FPSCLASS_H_
#define _FPSCLASS_H_


// Linking
#pragma comment(lib, "winmm.lib")

// Includes 
#include <windows.h>
#include <mmsystem.h>

// Class name: FpsClass
class FpsClass
{
	public:
		FpsClass();
		FpsClass(const FpsClass&);
		~FpsClass();

		void Initialize();
		void Frame();
		int GetFps();

	private:
		int m_fps, m_count;
		unsigned long m_startTime;
};


#endif