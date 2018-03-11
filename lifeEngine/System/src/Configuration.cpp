#include "../Configuration.h"
#include <Graphics\LightManager.h>

//-------------------------------------------------------------------------//

bool le::Configuration::IsWireframeRender = false;

//-------------------------------------------------------------------------//

le::Configuration::Configuration() :
	WindowSize( 800, 600 ),
	FrameLimit( 0 ),
	AntialiasingLevel( 0 ),
	RenderDistance( 1500 ),
	QualityShadows( LightManager::Low ),
	FOV( 75 ),
	Time( 0 )
{}

//-------------------------------------------------------------------------//