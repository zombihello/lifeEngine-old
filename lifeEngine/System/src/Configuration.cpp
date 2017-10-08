#include "../Configuration.h"

//-------------------------------------------------------------------------//

bool le::Configuration::IsWireframeRender = false;

//-------------------------------------------------------------------------//

le::Configuration::Configuration() :
	WindowSize( 800, 600 ),
	FrameLimit( 0 ),
	AntialiasingLevel( 0 ),
	FOV( 75 ),
	Time( 0 )
{}

//-------------------------------------------------------------------------//

