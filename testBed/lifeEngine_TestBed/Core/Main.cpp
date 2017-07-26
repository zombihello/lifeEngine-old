// lifeEngine
#include <System\leSystem.h>
using namespace le;

//Game
#include "GameStages.h"

//---------------------------------------------------------

int main( int argc , char** argv )
{
	System System( "../config.cfg", "../Shaders", argc, argv );
	System.SetWindowTitle( "TestEngine" , ENGINE " " ENGINE_VERSION );

	GameStages GameStages( System );

	System.MainLoop( GameStages );
	return 0;
}

//---------------------------------------------------------