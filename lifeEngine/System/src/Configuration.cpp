#include "../Configuration.h"
#include <System\ShaderManager.h>

//-------------------------------------------------------------------------//

int le::Configuration::ShadowMapSize = 1024;
bool le::Configuration::IsWireframeRender = false;

//-------------------------------------------------------------------------//

le::Configuration::Configuration() :
	WindowSize( 800, 600 ),
	FrameLimit( 0 ),
	AntialiasingLevel( 0 ),
	RenderDistance( 1000 ),
	QualityShadows( le::QualityShadows::High ),
	FOV( 75 ),
	SensitivityMouse( 0.15f ),
	Time( 0 ),
	Fullscreen( false ),
	VSinc( false )
{}

//-------------------------------------------------------------------------//