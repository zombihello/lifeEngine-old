//////////////////
// LIFEENGINE
//////////////////
#include <System\System.h>
#include "..\BasicApplication.h"

//-------------------------------------------------------------------------//

le::BasicApplication::BasicApplication( le::System& System )
{
	Configuration = &System.GetConfiguration();
	this->System = &System;
}

//-------------------------------------------------------------------------//
