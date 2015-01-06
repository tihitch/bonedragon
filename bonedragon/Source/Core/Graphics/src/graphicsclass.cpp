///////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Light = 0;
	m_Bitmap = 0;
	m_Text = 0;
	m_Cursor = 0;
	m_ParticleShader = 0;
	m_ParticleSystem = 0;

	m_ModelList = 0;
	m_Frustum = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	// Creat the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D){
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result){
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	// Create the camera pbject
	m_Camera = new CameraClass;
	if(!m_Camera){
		return false;
	}
	// Set the initial position of the camera
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the particle shader object.
	m_ParticleShader = new ParticleShaderClass;
	if(!m_ParticleShader)
	{
		return false;
	}

	// Initialize the particle shader object.
	result = m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the particle shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the particle system object.
	m_ParticleSystem = new ParticleSystemClass;
	if(!m_ParticleSystem)
	{
		return false;
	}

	// Initialize the particle system object.
	result = m_ParticleSystem->Initialize(m_D3D->GetDevice(), "data/ParticleData.txt");
	if(!result)
	{
		return false;
	}
	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the cursor object.
	m_Cursor = new CursorClass;
	if(!m_Cursor)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Cursor->Initialize(m_D3D->GetDevice(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	
	


	// Create the model object
	m_Model = new ModelClass;
	if(!m_Model){
		return false;
	}
	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "data/Cube.txt", L"data/Sand.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	
	//Create the light object
	m_Light = new LightClass;
	if(!m_Light){
		return false;
	}
	// Initialize the light object
	//m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	//m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	//m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	//m_Light->SetSpecularPower(32.0f);

	// Create the model list object.
	m_ModelList = new ModelListClass;
	if(!m_ModelList)
	{
		return false;
	}

	// Initialize the model list object.
	result = m_ModelList->Initialize(NUM_BOXES, 1.0f);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if(!m_Frustum)
	{
		return false;
	}

	return true;
	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"data/Sand.dds", 256, 256);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{

	// Release the frustum object.
	if(m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the model list object.
	if(m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}

	// Release the particle system object.
	if(m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	// Release the particle shader object.
	if(m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}
	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the cursor object.
	if(m_Cursor)
	{
		m_Cursor->Shutdown();
		delete m_Cursor;
		m_Cursor = 0;
	}

	// Release the bitmap object.
	if(m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	if(m_D3D){
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
	return;
}


bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime, 
							float positionX, float positionY, float positionZ,
							float rotationX, float rotationY, float rotationZ,
							float position2X, float position2Y, float position2Z,
							int   gravityMode)
{
	bool result;
	D3DXMATRIX view;
	AABBClass * killbox;
	AABBClass **AABBs;
	// Set the position of the camera.
	m_Camera->SetPosition(positionX, positionY, positionZ);
	m_Camera->SetRotation(rotationX, rotationY, rotationZ);
	// Generate the view matrix based on the camera's position.
	m_Camera->Render();
	m_Camera->GetViewMatrix(view);
	m_ModelList->SetPosition(0, position2X, position2Y, position2Z);
	// Get the AABB of the special obj
	killbox = m_ModelList->GetAABB(0);
	AABBs = m_ModelList->GetAABBs();
	// Run the frame processing for the particle system. Pass it the view matrix for billboarding
	m_ParticleSystem->Frame(frameTime, view, gravityMode, position2X, position2Y, position2Z, AABBs, m_ModelList->GetModelCount());

	// Set the location of the mouse
	result = m_Text->SetMousePosition(mouseX, mouseY);
	if(!result)
	{
		return false;
	}
	// Set the location of the mouse
	result = m_Text->SetFps(fps);
	if(!result)
	{
		return false;
	}
	// Set the cpu usage.
	result = m_Text->SetCpu(m_ParticleSystem->GetParticleCount());
	if(!result)
	{
		return false;
	}

	result = m_Cursor->SetMousePosition(mouseX, mouseY);
	if(!result)
	{
		return false;
	}


	return true;
}


bool GraphicsClass::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	D3DXVECTOR4 color;
	bool renderModel, result;

	//Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Construct the frustum.
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Get the number of models that will be rendered.
	modelCount = m_ModelList->GetModelCount();

	// Initialize the count of models that have been rendered.
	renderCount = 0;

	// Go through all the models and render them only if they can be seen by the camera view
	for(index=0; index<modelCount; index++)
	{
		// Get the position and color of the sphere model at this index.
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		// Set the radius of the collision to test for
		radius = 1.0f;

		// Check if the sphere model is in the view frustrum
		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);

		// If it can be seen then render it, if not skip this model
		if(renderModel)
		{
			// Move the model to the location it should be renderd at
			D3DXMatrixTranslation(&worldMatrix, positionX, positionY, positionZ);

			// Put the model vertex and index bufferss on the graphics pipeline to prepare them for drawing
			m_Model->Render(m_D3D->GetDevice());

			//Render the model
			m_ParticleShader->Render(m_D3D->GetDevice(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Model->GetTexture());
			// Reset to the original world matrix
			m_D3D->GetWorldMatrix(worldMatrix);

			//Since this model was rendered then increase the count for this frame
			renderCount++;
		}
	}

	result = m_Text->SetRenderCount(renderCount);
	if(!result)
	{
		return false;
	}
	// Turn on alpha blending.
	m_D3D->EnableAlphaBlending();
	m_D3D->TurnZBufferOff();
	// Put the particle system vertex and index buffes on the graphics pipeline to perare them for drawing
	m_ParticleSystem->Render(m_D3D->GetDevice());

	// Render the particles using the particle shader.
	m_ParticleShader->Render(m_D3D->GetDevice(), m_ParticleSystem->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
				 m_ParticleSystem->GetTexture());
	// Turn off alpha blending
	m_D3D->DisableAlphaBlending();
	// Turn off the Z buffer to begin all 2D rendering
	
	//Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//result = m_Bitmap->Render(m_D3D->GetDevice(), 100, 100);
	//if(!result){
	//	return false;
	//}
	// Render the cursor
	m_Cursor->Render(m_D3D->GetDevice(), worldMatrix, orthoMatrix);
	// Render the text strings.
	m_Text->Render(m_D3D->GetDevice(), worldMatrix, orthoMatrix);
	
	
	//Render the bitmap using the texture shader 
	//m_TextureShader->Render(m_D3D->GetDevice(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, 
	//						orthoMatrix, m_Bitmap->GetTexture() );
	// Turn on the Z buffer again affter 2D rendering is done.
	m_D3D->TurnZBufferOn();
	//Present the rendered scene
	m_D3D->EndScene();
	return true;
}