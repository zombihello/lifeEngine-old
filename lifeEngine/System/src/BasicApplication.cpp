//////////////////
// LIFEENGINE
//////////////////
#include <System\System.h>
#include "..\BasicApplication.h"

//-------------------------------------------------------------------------//

le::BasicApplication::BasicApplication( le::System& System )
{
	this->System = &System;
	this->Configuration = &System::Configuration;
}

//-------------------------------------------------------------------------//
