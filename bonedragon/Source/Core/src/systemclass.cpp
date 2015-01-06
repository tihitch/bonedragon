#include "systemclass.h"

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_Timer = 0;
	m_Position = 0;
	m_SpcObj_Position = 0;
	m_GravityMode = 0;
}

SystemClass::SystemClass(const SystemClass& other){
}

SystemClass::~SystemClass(){
}

bool SystemClass::Initialize(){
	int screenWidth, screenHeight;
	bool result;

	// Initialize the width and height of the screen to zero before sending the bariable into the function
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the widows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object. This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if(!m_Input){
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this applications.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics){
		return false;
	}

	// Initialize the graphics object
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if(!result){
		return false;
	}

	// Create the fps object.
	m_Fps = new FpsClass;
	if(!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();

	// Create the cpu object.
	m_Cpu = new CpuClass;
	if(!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();

	// Create the timer object.
	m_Timer = new TimerClass;
	if(!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if(!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object for the camera.
	m_Position = new PositionClass;
	if(!m_Position)
	{
		return false;
	}

	// Set the initial position of the viewer.
	m_Position->SetPosition(0.0f, 0.0f, -20.0f);

	// Create the position object.
	m_SpcObj_Position = new PositionClass;
	if(!m_SpcObj_Position)
	{
		return false;
	}

	// Set the initial position of the viewer.
	m_SpcObj_Position->SetPosition(0.0f, 3.0f, 0.0f);

	m_GravityMode = GRAV_DIR_NONE;
	return true;

}

void SystemClass::Shutdown()
{
	// Release the position object.
	if(m_SpcObj_Position)
	{
		delete m_SpcObj_Position;
		m_SpcObj_Position = 0;
	}
	// Release the position object.
	if(m_Position)
	{
		delete m_Position;
		m_Position = 0;
	}
	// Release the timer object.
	if(m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the cpu object.
	if(m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if(m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}
	// Release the hraphics object.
	if(m_Graphics){
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	//Release the input object.
	if(m_Input){
		m_Input->Shutdown();
		delete m_Input;
		m_Input = 0;
	}

	//Shutdown the window.
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user
	done = false;
	while(!done)
	{

		//Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT){
			done = true;
		}
		else {
			//Otherwuse do the frame processing
			result = Frame();
			if(!result){
				done = true;
			}
		}
		// Check if the user pressed escape and wants to quit.
		if(m_Input->IsEscapePressed() == true)
		{
			done = true;
		}
	}
	return;
}

bool SystemClass::Frame()
{
	bool result;
	int mouseX, mouseY;
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	float position2X, position2Y, position2Z;
	// Update the system stats.
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	// Check what relevant keys are pressed, and make changes if necessary before rendering
	result = ProcessFrameInput(); 
	if(!result){
		return false;
	}

	// Get the current view point position/rotation.
	m_Position->GetPosition(positionX, positionY, positionZ);
	m_Position->GetRotation(rotationX, rotationY, rotationZ);
	// Get the position of the special object
	m_SpcObj_Position->GetPosition(position2X, position2Y, position2Z);
	// Get the location of the mouse from the input object,
	m_Input->GetMouseLocation(mouseX, mouseY);

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(mouseX, mouseY, m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetTime(), 
								positionX, positionY, positionZ,
								rotationX, rotationY, rotationZ,
								position2X, position2Y, position2Z,
								m_GravityMode);
	if(!result)
	{
		return false;
	}

	// Finally render the graphics to the screen.
	result = m_Graphics->Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool SystemClass::ProcessFrameInput()
{
	bool result, keyDown;

	//Check if the user pressed escape and wants to exit the application
	result = m_Input->Frame(); 
	if(!result){
		return false;
	}
	// Set the frame time for calculating the updated position.
	m_Position->SetFrameTime(m_Timer->GetTime());
	m_SpcObj_Position->SetFrameTime(m_Timer->GetTime());

	// Check if translation any keys are pressed, if so translate camera accordingly.
	// A-axis (camera space; left-right) translation
	keyDown = m_Input->IsAPressed();
	m_Position->TranslateLeft(keyDown, false);
	keyDown = m_Input->IsDPressed();
	m_Position->TranslateRight(keyDown, false);
	// B-axis (camera space; up-down) translation 
	keyDown = m_Input->IsRPressed();
	m_Position->TranslateUp(keyDown, false);
	keyDown = m_Input->IsFPressed();
	m_Position->TranslateDown(keyDown, false);
	// View Direction translation
	keyDown = m_Input->IsWPressed();
	m_Position->TranslateForward(keyDown, false);
	keyDown = m_Input->IsSPressed();
	m_Position->TranslateBackward(keyDown, false);

	// Check if rotation keys have are pressed, if so rotate the camera accordingly.
	// Y-Axis rotation
	keyDown = m_Input->IsLeftArrowPressed();
	m_Position->TurnLeft(keyDown);
	keyDown = m_Input->IsRightArrowPressed();
	m_Position->TurnRight(keyDown);
	// X-Axis rotation
	keyDown = m_Input->IsUpArrowPressed();
	m_Position->TurnUp(keyDown);
	keyDown = m_Input->IsDownArrowPressed();
	m_Position->TurnDown(keyDown);
	// Z-Axis rotation
	keyDown = m_Input->IsPeriodPressed();
	m_Position->RollClockwise(keyDown);
	keyDown = m_Input->IsCommaPressed();
	m_Position->RollCCWise(keyDown);
	
	// Check translations on the Special Object if so translate camera accordingly.
	//  These translations are independant on the current view
	// X-axis translation
	keyDown = m_Input->IsJPressed();
	m_SpcObj_Position->TranslateLeft(keyDown, true);
	keyDown = m_Input->IsLPressed();
	m_SpcObj_Position->TranslateRight(keyDown, true);
	// Y-axis (camera space; up-down) translation 
	keyDown = m_Input->IsYPressed();
	m_SpcObj_Position->TranslateUp(keyDown, true);
	keyDown = m_Input->IsHPressed();
	m_SpcObj_Position->TranslateDown(keyDown, true);
	// Z-axis  translation
	keyDown = m_Input->IsIPressed();
	m_SpcObj_Position->TranslateForward(keyDown, true);
	keyDown = m_Input->IsKPressed();
	m_SpcObj_Position->TranslateBackward(keyDown, true);
	// Gravity Modes
	//keyDown = m_Input->IsSpacePressed();
	//ToggleGravityMode(keyDown);
	keyDown = m_Input->Is1Pressed();
	if(keyDown)
		m_GravityMode = GRAV_DIR_ATTRACT; 
	keyDown = m_Input->Is3Pressed();
	if(keyDown)
		m_GravityMode = GRAV_DIR_REPEL; 
	keyDown = m_Input->Is2Pressed();
	if(keyDown)
		m_GravityMode = GRAV_DIR_NONE; 

	return true;
}
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//Get an external pointer to this object.
	ApplicationHandle = this;

	//Get the instance of this application
	m_hinstance = GetModuleHandle(NULL);

	//Give the applicatipn a name.
	m_applicationName = L"ParticleEdit";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register the window class.
	RegisterClassEx(&wc);

	//Determine the resolution of the clients desktop screen.
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	// Setuo the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN){
		// If full screen set the scree to maximum size of the users desktop and 32 bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display setting to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else {
		// If Windowed then set it to 800x600 resulution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}
		// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
				WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
				posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	//Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN){
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	//Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


void SystemClass::ToggleGravityMode(bool keyPressed)
{
	if(keyPressed)
	{
		m_GravityMode++;
		if(m_GravityMode > GRAV_DIR_REPEL)
			m_GravityMode = GRAV_DIR_ATTRACT;
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// All other message pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}