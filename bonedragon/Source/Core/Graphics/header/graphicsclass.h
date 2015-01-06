// Filename: graphicsclass.h
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

// INCLUDES //
#include <windows.h>

#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightclass.h"
#include "bitmapclass.h"
#include "textclass.h"
#include "cursorclass.h"
#include "particleshaderclass.h"
#include "particlesystemclass.h"
#include "modellistclass.h"
#include "frustumclass.h"

#define  NUM_BOXES 7

// GLOBALS //
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

// Class name: GraphicsClass
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, int, int, float, float, float, float, float, float, float, float, float, float, int);
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightClass* m_Light;
	BitmapClass * m_Bitmap;
	TextClass * m_Text;
	CursorClass * m_Cursor;

	ModelListClass* m_ModelList;
	FrustumClass* m_Frustum;

	ParticleShaderClass* m_ParticleShader;
	ParticleSystemClass* m_ParticleSystem;
};

#endif
