#include "../Configuration.h"
#include <Graphics\LightManager.h>

//-------------------------------------------------------------------------//

bool le::Configuration::IsWireframeRender = false;

//-------------------------------------------------------------------------//

le::Configuration::Configuration() :
	WindowSize( 800, 600 ),
	FrameLimit( 0 ),
	AntialiasingLevel( 0 ),
	RenderDistance( 1000 ),
	QualityShadows( LightManager::Low ),
	FOV( 75 ),
	SensitivityMouse( 0.15f ),
	Time( 0 )
{}

//-------------------------------------------------------------------------//