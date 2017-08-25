//////////////////
// LIFEENGINE
//////////////////
#include <System\System.h>
#include "..\BasicApplication.h"

//-------------------------------------------------------------------------//

le::BasicApplication::BasicApplication( System& System )
{
	Root = System.GetOgreRoot();
	Window = System.GetWindow();
	Scene = System.GetScene();
}

//-------------------------------------------------------------------------//
